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
#include "include/HaarDetector.h"
#include "include/AFContext.h"
#include "include/FImageContext.h"
#include <android/log.h>


using namespace cv;
using namespace uac;

// keys for initialization
const char* HaarDetector:: DETECTOR_NAME        = "HAAR";
const char* HaarDetector:: HAAR_SCALE_FACTOR    = "HAAR_SCALE_FACTOR";
const char* HaarDetector:: HAAR_MIN_NEIGHBORS   = "HAAR_MIN_NEIGHBORS";
const char* HaarDetector:: HAAR_MIN_SIZE        = "HAAR_MIN_SIZE";
const char* HaarDetector:: IMG_PROC_SIZE        = "IMG_PROC_SIZE";

Detector& HaarDetector:: create( map<std::string,std::string>& params ){

    // get params
    double scaleFactor = 1.2;
    if( params.find(HAAR_SCALE_FACTOR) != params.end() ) {
        string factor = params[HAAR_SCALE_FACTOR];
        scaleFactor = atof( factor.c_str() );
    }
    int minNeighbors = 3;
    if( params.find(HAAR_MIN_NEIGHBORS) != params.end() ){
        string neighbors = params[HAAR_MIN_NEIGHBORS];
        minNeighbors = atoi( neighbors.c_str() );
    }
    cv::Size minSize(48,48);
    if( params.find(HAAR_MIN_SIZE) != params.end() ){

        string size = params[HAAR_MIN_SIZE];
        istringstream is(size);
        string coordx;
        string coordy;
        getline(is, coordx, ',');
        getline(is, coordy, ',');

        cv::Size tmp( atoi(coordx.c_str()), atoi(coordy.c_str()));
        minSize = tmp;
    }
    int maxImgSize = 480;
    if( params.find(IMG_PROC_SIZE) != params.end() ){
        string imgSize = params[IMG_PROC_SIZE];
        maxImgSize = atoi( imgSize.c_str() );
    }

    HaarDetector* pDet = new HaarDetector(scaleFactor,minNeighbors,minSize,maxImgSize);
    return *pDet;
}

void HaarDetector::release(Detector& det){

    HaarDetector* pDet = (HaarDetector*)&det;
    delete pDet;
}

HaarDetector:: HaarDetector( double scaleFactor, int minNeighbors, cv::Size minSize, int maxImgSize ){

    this->name = DETECTOR_NAME;

    this->scaleFactor  = scaleFactor;
    this->minNeighbors = minNeighbors;
    this->minSize      = minSize;
    this->maxImgSide   = maxImgSize;

    bInit = false;
    AFContext& ctx = AFContext::getInstance();
    const string& rootPath = ctx.getRootPath();
    string face_cascade_name = "haarcascade_frontalface_alt.xml";
    string descriptor = rootPath + "/" + face_cascade_name;
    if( !faceCascade.load( descriptor ) ){
        bInit = false;
        return;
    };
    bInit = true;
}

HaarDetector:: ~HaarDetector(){

}

void HaarDetector:: detect(FImage& img, vector<cv::Rect>& faces ) {

    if (!bInit) {
        return;
    }

    //get image
    Mat cvImg = img.getMat();

    //convet to gray
    Mat imgGray;
    Mat scImg;
    if (cvImg.channels() == 1) {
        imgGray = cvImg;
    }
    else{
        cvtColor(cvImg, imgGray, CV_BGR2GRAY);
    }

    float scale = 1;
    int min = imgGray.rows;
    if( min > maxImgSide ){

        scale =  maxImgSide / (float)min;
        scale =  floor(scale/0.00005)*0.00005;

        //resize
        cv::resize(imgGray, scImg, cv::Size( (int)round(imgGray.cols * scale), (int)round(imgGray.rows * scale)) );
    }
    else{
        scImg = imgGray;
    }

    //equalize
    equalizeHist( scImg, scImg );

    // detect faces
    faceCascade.detectMultiScale( scImg, faces, scaleFactor, minNeighbors, 0|CV_HAAR_SCALE_IMAGE, minSize );

    // scale result if necessary
    if( scale <  1  && faces.size() > 0 ){

        for( int i=0; i<faces.size(); i++ ){

            faces[i].x = (int)(faces[i].x / scale);
            faces[i].y = (int)(faces[i].y / scale);
            faces[i].width  = (int)(faces[i].width / scale);
            faces[i].height = (int)(faces[i].height / scale);
        }
    }
}

