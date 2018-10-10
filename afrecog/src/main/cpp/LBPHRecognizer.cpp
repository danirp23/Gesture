/*
 * Universidad Autónoma de Colombia. 2017. All Rights Reserved.
 * Alexander Cardona
 *
 *
 * This software is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "include/LBPHRecognizer.h"
#include "include/AFContext.h"

//TODO: FOR TESTING... REMOVE
#include <android/log.h>

using namespace uac;
using namespace cv;
using namespace cv::face;

const char* LBPHRecognizer:: RECOGNIZER_NAME  = "LBPH";
const char* LBPHRecognizer:: LBPH_RADIUS      = "LBPH_RADIUS";
const char* LBPHRecognizer:: LBPH_NEIGHBORS   = "LBPH_NEIGHBORS";
const char* LBPHRecognizer:: LBPH_GRID_X      = "LBPH_GRID_X";
const char* LBPHRecognizer:: LBPH_GRID_Y      = "LBPH_GRID_Y";
const char* LBPHRecognizer:: LBPH_IMG_HEIGHT  = "LBPH_IMG_HEIGHT";


//static methods
Recognizer& LBPHRecognizer:: create( std::map<std::string, std::string>& params ){

    // get params
    int radius = 1;
    if( params.find(LBPH_RADIUS) != params.end() ) {
        string r = params[LBPH_RADIUS];
        radius = atoi( r.c_str() );
    }
    int neighbors = 8;
    if( params.find(LBPH_NEIGHBORS) != params.end() ) {
        string n = params[LBPH_NEIGHBORS];
        neighbors = atoi( n.c_str() );
    }
    int gridX = 8;
    if( params.find(LBPH_GRID_X) != params.end() ) {
        string n = params[LBPH_GRID_X];
        gridX = atoi( n.c_str() );
    }
    int gridY = 8;
    if( params.find(LBPH_GRID_Y) != params.end() ) {
        string n = params[LBPH_GRID_Y];
        gridY = atoi( n.c_str() );
    }
    int height = 128;
    if( params.find(LBPH_IMG_HEIGHT) != params.end() ) {
        string n = params[LBPH_IMG_HEIGHT];
        height = atoi( n.c_str() );
    }

    // instance recognizer with given parameters
    LBPHRecognizer* pRec = new LBPHRecognizer(radius, neighbors, gridX, gridY, height);
    return *pRec;
}

void LBPHRecognizer::release(Recognizer& rec){

    LBPHRecognizer* pRec = (LBPHRecognizer*)&rec;
    delete pRec;
}

LBPHRecognizer:: LBPHRecognizer(int radius, int neighbors, int gridX, int gridY, int imgHeight){

    name = RECOGNIZER_NAME;

    this->radius    = radius;
    this->neighbors = neighbors;
    this->gridX     = gridX;
    this->gridY     = gridY;
    this->imgHeight = imgHeight;

    recognizer = cv::face::createLBPHFaceRecognizer(radius, neighbors, gridX, gridY);
}

LBPHRecognizer:: ~LBPHRecognizer(){

}

void LBPHRecognizer:: train(vector<FImage>& imgs, vector<int>& labels){

    vector<Mat> cvImages;
    for( int i=0; i<imgs.size(); i++ ){

        Mat cvImg = imgs[i].getMat();

        // convet to gray
        Mat imgGray;
        if (cvImg.channels() == 1) {
            imgGray = cvImg;
        }
        else{
            cvtColor(cvImg, imgGray, CV_BGR2GRAY);
        }

        //scale
        Mat scImg;
        float scale = 1;

        scale =  imgHeight / (float)imgGray.rows;
        scale =  floor(scale/0.00005)*0.00005;

        if( scale != 1 ){

            //resize
            cv::resize(imgGray, scImg, cv::Size( (int)round(imgGray.cols * scale), (int)round(imgGray.rows * scale)) );
        }
        else{
            scImg = imgGray;
        }

        //equalize
        equalizeHist( scImg, scImg );

        //add to train set
        cvImages.push_back(scImg);
    }
    recognizer->train(cvImages, labels);
}

double LBPHRecognizer:: recognize(FImage& img, int& label){

    if( recognizer->empty() ){

#ifdef __ANDROID__
        __android_log_print(ANDROID_LOG_ERROR, "xxxxxx*", "LBPHRecognizer- model empty" );
#else
      cout << "LBPHRecognizer- model empty" << endl;
#endif

        return -1;
    }

    Mat cvImg = img.getMat();

    // convet to gray
    Mat imgGray;
    if (cvImg.channels() == 1) {
        imgGray = cvImg;
    }
    else{
        cvtColor(cvImg, imgGray, CV_BGR2GRAY);
    }

    //scale
    Mat scImg;
    float scale = 1;

    scale =  imgHeight / (float)imgGray.rows;
    scale =  floor(scale/0.00005)*0.00005;

    if( scale != 1 ){

        //resize
        cv::resize(imgGray, scImg, cv::Size( (int)round(imgGray.cols * scale), (int)round(imgGray.rows * scale)) );
    }
    else{
        scImg = imgGray;
    }

    equalizeHist( scImg, scImg );

    double confidence;
    recognizer->predict(scImg,label,confidence);
    return confidence;
}

void LBPHRecognizer:: save(string& filename){

    AFContext& ctx = AFContext::getInstance();
    const string& rootPath = ctx.getRootPath();
    string descriptor = rootPath + "/" + filename;

    recognizer->save(descriptor);
}


void LBPHRecognizer:: load(string& filename){

    AFContext& ctx = AFContext::getInstance();
    const string& rootPath = ctx.getRootPath();
    string descriptor = rootPath + "/" + filename;

    // check if file exists
    std::ifstream file(descriptor);
    if(file){
        //close stream
        file.close();

        //load descriptor
        recognizer->load(descriptor);
    }
    else{
#ifdef __ANDROID__
        __android_log_print(ANDROID_LOG_ERROR, "xxxxxx*", "LBPH recognizer: model file not loaded: %s", descriptor.c_str() );
#else
        cout << "LBPH recognizer: model file not loaded:" << descriptor.c_str() << endl;
#endif
    }
}
