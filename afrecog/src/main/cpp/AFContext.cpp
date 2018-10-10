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
#include "include/AFContext.h"
#include "include/HoGDetector.h"
#include "include/LBPHRecognizer.h"
#include "include/HaarDetector.h"
#include "include/FImageContext.h"

//TODO: FOR TESTING... REMOVE
#include <android/log.h>

using namespace uac;

AFContext AFContext::mInstance;

const char* AFContext:: DETECTOR_NAME   = "DETECTOR_NAME";
const char* AFContext:: RECOGNIZER_NAME = "RECOGNIZER_NAME";

AFContext::  AFContext(){

    currentDetector   = NULL;
    currentRecognizer = NULL;
    userDetector      = NULL;
    userRecognizer    = NULL;
    percIncX          = 0.1;
    percIncY          = 0.3;
    recogFilename     = "recog_model.yaml";
}

AFContext:: ~AFContext(){

}

void AFContext:: setRootPath( const char* path ){

    rootPath = path;
}

const string& AFContext:: getRootPath(){

    return rootPath;
}

double  AFContext:: recognize(FImage& img, int& category){

    if( currentRecognizer != NULL ){

        double confidence = -1;
        try {
            confidence = currentRecognizer->recognize(img, category);
        }
        catch( const std::exception& ex ) {

#ifdef __ANDROID__
            __android_log_print(ANDROID_LOG_ERROR, "xxxxxx*", "Recognition error: %s", ex.what() );
#else
            cout << "Recognition error: " << ex.what() << endl;
#endif
        }
        catch(...){

#ifdef __ANDROID__
            __android_log_print(ANDROID_LOG_ERROR, "xxxxxx*", "Recognition: Unknown error" );
#else
            cout << "Recognition unknown error" << endl;
#endif
        }

        return confidence;
    }
}

int AFContext:: trainRecog( vector<FImage>& vecImgs, vector<int>& vecLabels, bool randomNeg )
{
    if( vecImgs.size() != vecLabels.size() ){
        return -1;
    }

    // random negatives?
    vector<int> negImgs;
    if( randomNeg ){

        // creates random negative samples

        int negIdx = 0;
        RNG rng( 0x000000FF );

        // solid image
        Mat solid( 128, 128, CV_8UC1, Scalar(255) );
        FImage& img1 = FImageContext::getInstance().createImage(solid);
        negImgs.push_back(img1.getId());
        vecImgs.push_back(img1);
        vecLabels.push_back(-1);

        // random image
        Mat random( 128, 128, CV_8UC1 );
        for( int i=0; i<128; i++ ){
            for( int j=0; j<128; j++){
                random.at<uchar>(i,j) = rng.uniform(0,255);
            }
        }
        FImage& img2 = FImageContext::getInstance().createImage(random);
        negImgs.push_back(img2.getId());
        vecImgs.push_back(img2);
        vecLabels.push_back(-1);

        // random normal distribution
        Mat normal( 128, 128, CV_8UC1 );
        for( int i=0; i<128; i++ ){
            for( int j=0; j<128; j++){
                normal.at<uchar>(i,j) = (int)rng.gaussian(100);
            }
        }
        FImage& img3 = FImageContext::getInstance().createImage(normal);
        negImgs.push_back(img3.getId());
        vecImgs.push_back(img3);
        vecLabels.push_back(-1);

        // circles
        Mat circ( 128, 128, CV_8UC1, Scalar(255) );
        for( int i=0; i<10; i++ ){
            int color = rng.uniform(0,255);
            int x = rng.uniform(10,110);
            int y = rng.uniform(10,110);
            int r = rng.uniform(10,60);
            circle(circ, Point(x,y), r, Scalar(color));
        }
        FImage& img4 = FImageContext::getInstance().createImage(circ);
        negImgs.push_back(img4.getId());
        vecImgs.push_back(img4);
        vecLabels.push_back(-1);
    }

    int res = 0;
    int size = vecImgs.size();
    if( size > 0 && currentRecognizer != NULL ) {

        currentRecognizer->train( vecImgs, vecLabels );
        res = 1;
    }

    if( randomNeg ){
        for( int i=0; i<negImgs.size(); i++ ){

            int img = negImgs[i];
            FImageContext::getInstance().removeImage(img);
        }
    }

    return res;
}

void  AFContext:: setDetector( Detector& detector ){

    currentDetector = &detector;
    userDetector    = currentDetector;
}

void AFContext::createDetector(map<string, string>& params){

    // check if detector name was given
    if( params.find( DETECTOR_NAME ) == params.end() ){

#ifdef __ANDROID__
        __android_log_print(ANDROID_LOG_ERROR, "xxxxxx*", "AFContext- detector key name doesn't exists" );
#else
        cout << "AFContext- detector key name doesn't exists" << endl;
#endif
        return;
    }

    // hog?
    string name = params[DETECTOR_NAME];
    if (name == HoGDetector::DETECTOR_NAME ) {

        // release old detector if exits and is not given by user
        if( currentDetector != NULL && currentDetector != userDetector ) {

            if( currentDetector->getName() == HoGDetector::DETECTOR_NAME ){
                HoGDetector::release(*currentDetector);
            }
            // remove reference to user detector
            userDetector = NULL;
        }
        // creates new detector
        currentDetector = &(HoGDetector::create(params));
    }
    else if( name == HaarDetector::DETECTOR_NAME ) {

        // release old detector if exits and is not given by user
        if( currentDetector != NULL && currentDetector != userDetector ) {

            if( currentDetector->getName() == HaarDetector::DETECTOR_NAME ){
                HaarDetector::release(*currentDetector);
            }

            // remove reference to user detector
            userDetector = NULL;
        }

        // creates new detector
        currentDetector = &(HaarDetector::create(params));
    }
    else{
#ifdef __ANDROID__
        __android_log_print(ANDROID_LOG_ERROR, "xxxxxx*", "AFContext- invalid detector name" );
#else
        cout << "AFContext- invalid detector name" << endl;
#endif
    }
}

void  AFContext:: setRecognizer( Recognizer& recog ){

    currentRecognizer = &recog;
    userRecognizer    = currentRecognizer;
}

void AFContext::createRecognizer(map<string, string> &params) {

    // check if detector name was given
    if( params.find( RECOGNIZER_NAME ) == params.end() ){

#ifdef __ANDROID__
        __android_log_print(ANDROID_LOG_ERROR, "xxxxxx*", "AFContext- recognizer key name doesn't exists" );
#else
        cout << "AFContext- recognizer key name doesn't exists" << endl;
#endif
        return;
    }

    // hog?
    string name = params[RECOGNIZER_NAME];
    if (name == LBPHRecognizer::RECOGNIZER_NAME ) {

        // release old recognizer if exits and is not given by user
        if( currentRecognizer != NULL && currentRecognizer != userRecognizer ) {

            if( currentRecognizer->getName() == LBPHRecognizer::RECOGNIZER_NAME ){
                LBPHRecognizer::release(*currentRecognizer);
            }
            // remove reference to user detector
            userRecognizer = NULL;
        }
        // creates new detector
        currentRecognizer = &(LBPHRecognizer::create(params));
    }
    else{
#ifdef __ANDROID__
        __android_log_print(ANDROID_LOG_ERROR, "xxxxxx*", "AFContext- invalid recognizer name" );
#else
        cout << "AFContext- no valid recognizer name" << endl;
#endif
    }
}

void AFContext:: init(){

    if( currentDetector == NULL ) {

        map<string,string> params;
        currentDetector = &(HoGDetector::create(params));
    }

    if( currentRecognizer == NULL ){

        map<string,string> params;
        currentRecognizer = &(LBPHRecognizer::create(params));
    }

    recogFilename = "recog_model.yaml";
}

void AFContext:: close(){

    if( currentDetector != NULL && currentDetector != userDetector ) {

        if( currentDetector->getName() == HoGDetector::DETECTOR_NAME ){
            HoGDetector::release(*currentDetector);
        }
        else if( currentDetector->getName() == HaarDetector::DETECTOR_NAME ){
            HaarDetector::release(*currentDetector);
        }
    }
    if( currentRecognizer != NULL && currentRecognizer != userRecognizer ){

        if( currentRecognizer->getName() == LBPHRecognizer::RECOGNIZER_NAME )
        {
            LBPHRecognizer::release(*currentRecognizer);
        }
    }
}

void AFContext:: detectFaces(FImage& img, vector<Rect>& facesRects) {

    if (currentDetector != NULL) {

        Detector &detection = *currentDetector;
        detection.detect(img, facesRects);
    }
    else{
#ifdef __ANDROID__
        __android_log_print(ANDROID_LOG_ERROR, "xxxxxx*", "AFContext- Detector not found" );
#else
      cout << "AFContext- Detector not found" << endl;
#endif
    }
}

void AFContext:: adjustRects( FImage& img, vector<Rect>& facesRects){

    int marginX = 0;
    int marginY = 0;

    for( int i=0; i<facesRects.size(); i++ )
    {
        Rect r = facesRects[i];
        marginX = (int)(r.width * percIncX);
        marginY = (int)(r.height * percIncY);
        if( marginX > 0 ){

            r.x = r.x - marginX;
            if( r.x < 0 )
                r.x = 0;

            r.width = r.width + (2*marginX);
            if( r.x + r.width > img.getMat().cols ){
                r.width = img.getMat().cols - r.x;
            }
        }
        if( marginY > 0 ){

            r.y = r.y - marginY;
            if( r.y < 0 )
                r.y = 0;

            r.height = r.height + (2*marginY);
            if( r.y + r.height > img.getMat().rows ){
                r.height = img.getMat().rows - r.y;
            }
        }

        facesRects[i] = r;
    }
}

void AFContext:: saveRecogModel(){

    if( currentRecognizer != NULL ){

        currentRecognizer->save(recogFilename);
    }
}

void AFContext:: loadRecogModel(){

    if( currentRecognizer != NULL ){

        currentRecognizer->load(recogFilename);
    }
}
