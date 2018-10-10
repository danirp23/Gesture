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
#include "include/HoGDetector.h"
#include "include/AFContext.h"
#include <dlib/opencv.h>
#include <dlib/image_processing.h>
#include <android/log.h>
#include <map>
#include <string>

using namespace cv;
using namespace uac;
using namespace dlib;

const char* HoGDetector:: DETECTOR_NAME   = "HoG";
const char* HoGDetector:: HOG_LEVELS      = "HOG_LEVELS";
const char* HoGDetector:: HOG_NUM_PARTS   = "HOG_NUM_PARTS";
const char* HoGDetector:: IMG_PROC_SIZE   = "IMG_PROC_SIZE";


Detector& HoGDetector:: create(std::map<std::string, std::string>& params){

    // get params
    int levels = 4;
    if( params.find(HOG_LEVELS) != params.end() ) {
        string levels = params[HOG_LEVELS];
        levels = atoi( levels.c_str() );
    }

    int numParts = 3;
    if( params.find(HOG_NUM_PARTS) != params.end() ) {
        string num = params[HOG_NUM_PARTS];
        numParts = atoi( num.c_str() );
    }

    int imgSize = 384;
    if( params.find(IMG_PROC_SIZE) != params.end() ){
        string img_proc_size = params[IMG_PROC_SIZE];
        imgSize = atoi( img_proc_size.c_str() );
    }

    HoGDetector* pDet = new HoGDetector(numParts,levels,imgSize);
    return *pDet;
}

void HoGDetector::release(Detector& det){

    HoGDetector* pDet = (HoGDetector*)&det;
    delete pDet;
}

HoGDetector:: HoGDetector( int numParts, int pyramidLevels, int maxImgSize ){

    name           = DETECTOR_NAME;

    this->maxSide        = maxImgSize;
    this->pyramidLevels  = pyramidLevels;
    this->numParts       = numParts;

    bInit = false;
    AFContext& ctx = AFContext::getInstance();
    const string& rootPath = ctx.getRootPath();

    // instead of used original face detector it will be used to build
    // a clone with less number of patterns
    dlib::frontal_face_detector detectorTmp = get_frontal_face_detector();
    std::vector<frontal_face_detector> parts;

    // Split into parts and serialize to disk
    for (unsigned long i = 0; i < detectorTmp.num_detectors() && i < numParts; ++i)
    {
        dlib::frontal_face_detector part(detectorTmp.get_scanner(), detectorTmp.get_overlap_tester(), detectorTmp.get_w(i));
        parts.push_back(part);
    }

    // Reconstruct original detector
    dlib::frontal_face_detector reconstructed(parts);
    image_scanner_type new_scanner;
    new_scanner.copy_configuration(reconstructed.get_scanner());
    new_scanner.set_max_pyramid_levels(pyramidLevels);
    detector_type new_d(new_scanner, reconstructed.get_overlap_tester(), reconstructed.get_w());
    detector = new_d;
    bInit = true;
}

HoGDetector:: ~HoGDetector(){

}

void HoGDetector:: detect(FImage& img, std::vector<cv::Rect>& faces ) {

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

    //scale
    float scale = 1;
    int min = imgGray.rows;
    if( min > maxSide ){

        scale =  maxSide / (float)min;
        scale =  floor(scale/0.00005)*0.00005;

        //resize
        cv::resize(imgGray, scImg, cv::Size( (int)round(imgGray.cols * scale), (int)round(imgGray.rows * scale)) );
    }
    else{
        scImg = imgGray;
    }

    //equalize
    equalizeHist( scImg, scImg );

    // Turn OpenCV's Mat into something dlib can deal with.  Note that this just
    // wraps the Mat object, it doesn't copy anything.
    dlib::cv_image<unsigned char> dlImg(scImg);

    std::vector<dlib::rectangle> rects = detector(dlImg,0);

    // scale and convert result
    if( rects.size() > 0 ){

        for( int i=0; i<rects.size(); i++ ){
            cv::Rect r;

            r.x = (int)(rects[i].left() / scale);
            r.y = (int)(rects[i].top() / scale);
            r.width  = (int)(rects[i].width() / scale);
            r.height = (int)(rects[i].height() / scale);

            faces.push_back(r);
        }
    }

}
