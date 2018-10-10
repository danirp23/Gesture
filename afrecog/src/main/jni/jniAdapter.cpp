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

#include <jni.h>
#include <opencv2/opencv.hpp>
#include <android/log.h>
#include <android/bitmap.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <map>
#include "../cpp/include/FImageContext.h"
#include "../cpp/include/AFContext.h"
#include "../cpp/include/HaarDetector.h"
#include "../cpp/include/HoGDetector.h"
#include "../cpp/include/LBPHRecognizer.h"
#include "../cpp/include/Puntos.h"


using namespace cv;
using namespace uac;
using namespace std;


// Detector configuration params
map<std::string, std::string> detection_config_map;

// Recognizer configuration params
map<std::string, std::string> recog_config_map;


/*
 * Creates a new image from a region of given image
 */
extern "C"
jint Java_co_edu_fuac_afrecog_FImageContext__1createImageRect(JNIEnv *env, jobject instance,
                                                              jint id, jint x, jint y,
                                                              jint w, jint h) {

    try{


        FImage& img = FImageContext::getInstance().getImage(id);
        FImageRect& imgRect = FImageContext::getInstance().createImageRect(img,x,y,w,h);
        return imgRect.getId();
    }
    catch(...){

        return -1;
    }
}

/*
 * Recognize a given face image based on trained images. Set result of recognition in
 * first array elements.
 */
extern "C"
void Java_co_edu_fuac_afrecog_FaceContext__1recognize(JNIEnv *env, jobject instance,
                                                      jint id, jdoubleArray c_, jintArray cat_) {
    jdouble *c = env->GetDoubleArrayElements(c_, NULL);
    jint *cat = env->GetIntArrayElements(cat_, NULL);

    int category;
    double confidence;
    FImage& img = FImageContext::getInstance().getImage( id );
    confidence = AFContext::getInstance().recognize(img,category);
    if( confidence >= 0 ){

        c[0] = confidence;
        cat[0] = category;
    }
    else{

        c[0] = -1;
        cat[0] = -1;
    }

    env->ReleaseDoubleArrayElements(c_, c, 0);
    env->ReleaseIntArrayElements(cat_, cat, 0);
}

/*
 * Trains recognition with given images and corresponding labels
 */
extern "C"
void Java_co_edu_fuac_afrecog_FaceContext__1trainRecog(JNIEnv *env, jobject instance,
                                                       jintArray ids_, jintArray labels_) {
    jint *ids = env->GetIntArrayElements(ids_, NULL);
    jint *labels = env->GetIntArrayElements(labels_, NULL);

    int size = env->GetArrayLength(ids_);
    if( size > 0 ){

        vector<FImage> vecImgs;
        vector<int> vecLabels;
        for( int i=0; i<size; i++ ){
            //__android_log_print(ANDROID_LOG_ERROR, "xxxxxx*", "Recogintion : image id: %d", ids[i]);
            //__android_log_print(ANDROID_LOG_ERROR, "xxxxxx*", "Recogintion : label: %d", labels[i]);
            vecImgs.push_back( FImageContext::getInstance().getImage( ids[i] ) );
            vecLabels.push_back( labels[i] );
        }

        int res = AFContext::getInstance().trainRecog(vecImgs, vecLabels, false);
    }

    env->ReleaseIntArrayElements(ids_, ids, 0);
    env->ReleaseIntArrayElements(labels_, labels, 0);
}

/*
 * Close current detector and recognizer
 */
extern "C"
void Java_co_edu_fuac_afrecog_FaceContext__1close(JNIEnv *env, jobject instance) {

    AFContext::getInstance().close();
}

/*
 * Get height of given image
 */
extern "C"
jint Java_co_edu_fuac_afrecog_FImageContext__1getHeight(JNIEnv *env, jobject instance, jint id) {

    FImageContext& context = FImageContext::getInstance();
    FImage& img = context.getImage(id);
    int height = img.getMat().rows;
    return height;
}

/*
 * Get with of given image
 */
extern "C"
jint Java_co_edu_fuac_afrecog_FImageContext__1getWidth(JNIEnv *env, jobject instance, jint id) {

    FImageContext& context = FImageContext::getInstance();
    FImage& img = context.getImage(id);
    int width = img.getMat().cols;
    return width;
}

/*
 * Initialize default detector and recognizer
 */
extern "C"
void Java_co_edu_fuac_afrecog_FaceContext__1init(JNIEnv *env, jobject instance) {

    AFContext::getInstance().init();
}

/*
 * Detect faces in given image using current detector
 */
extern "C"
jintArray Java_co_edu_fuac_afrecog_FaceContext__1detectFaces(JNIEnv *env, jobject instance, jint id) {

    vector<Rect> faces;
    FImage &img = FImageContext::getInstance().getImage(id);

    AFContext::getInstance().detectFaces(img,faces);
    AFContext::getInstance().adjustRects(img,faces);

    int size = faces.size() * 4;
    jintArray result;
    result = env->NewIntArray(size);
    if (result == NULL) {

        //debug message
#ifdef __ANDROID__
        __android_log_print(ANDROID_LOG_ERROR, "xxxxx*", "JNI detectFaces: Out of memory error");
#else
        cout << "JNI detectFaces: Out of memory error" << endl;
#endif
        return NULL; /* out of memory error thrown */
    }
    int i;
    // fill a temp structure to use to populate the java int array
    jint fill[size];
    for (i = 0; i < faces.size(); i++) {
        fill[i * 4] = faces[i].x;
        fill[i * 4 + 1] = faces[i].y;
        fill[i * 4 + 2] = faces[i].width;
        fill[i * 4 + 3] = faces[i].height;
    }
    // move from the temp structure to the java structure
    env->SetIntArrayRegion(result, 0, size, fill);
    return result;
}

/*
 * Configures resource directories
 */
extern "C"
void Java_co_edu_fuac_afrecog_FaceContext__1config(JNIEnv *env, jobject instance, jstring path_){

    const char *path = env->GetStringUTFChars(path_, 0);
    AFContext::getInstance().setRootPath(path);
    env->ReleaseStringUTFChars(path_, path);
}

/*
 * Test method
 */
/*
extern "C"
void Java_co_edu_fuac_afrecog_FImageContext__1test(JNIEnv *env, jobject instance, jint id, jobject bmp) {

    AndroidBitmapInfo  info;
    void* pixels = 0;

    FImageContext& context = FImageContext::getInstance();
    FImage& img = context.getImage(id);
    context.test(img);

    try {
        //validate
        CV_Assert(AndroidBitmap_getInfo(env, bmp, &info) >= 0);
        CV_Assert(AndroidBitmap_lockPixels(env, bmp, &pixels) >= 0);
        CV_Assert(pixels);

        //type mat
        if(info.format == ANDROID_BITMAP_FORMAT_RGBA_8888){
            Mat tmp(info.height, info.width, CV_8UC4, pixels);
            //__android_log_print(ANDROID_LOG_ERROR,"xxxxx*","Copy to RGBA_8888");

            Mat cvImg = img.getMat();
            cvtColor(cvImg, tmp, cv::COLOR_BGR2RGBA, 4);

        } else{
            Mat tmp(info.height, info.width, CV_8UC2, pixels);
            //__android_log_print(ANDROID_LOG_ERROR,"xxxxx*","Copy to RGBA_555");
            img.getMat().copyTo(tmp);
        }

        AndroidBitmap_unlockPixels(env, bmp);

    } catch(cv::Exception e){

        AndroidBitmap_unlockPixels(env, bmp);
        jclass je = env->FindClass("java/lang/Exception");
        env->ThrowNew(je, "OpenCv Exception in JNI code");
        __android_log_print(ANDROID_LOG_ERROR,"xxxxx*","OpenCv Exception in JNI code");

    } catch (...){
        AndroidBitmap_unlockPixels(env, bmp);
        jclass je = env->FindClass("java/lang/Exception");
        env->ThrowNew(je, "Unknown exception in JNI code");
        __android_log_print(ANDROID_LOG_ERROR,"xxxxx*","Exception in JNI code");
    }
}
*/

/*
 * Removes image from set
 */
extern "C"
void Java_co_edu_fuac_afrecog_FImageContext__1removeImage(JNIEnv *env, jobject instance, jint id) {

    FImageContext& context = FImageContext::getInstance();
    context.removeImage(id);
}

/*
 * Adds a new image to set managed by FImageContext. Internally the image is a copy of the original
 * image, but in gray format.
 */
extern "C"
jint Java_co_edu_fuac_afrecog_FImageContext__1addImage(JNIEnv *env, jobject instance, jobject bmp,
                                                       jint degrees) {

    AndroidBitmapInfo  info;
    void* pixels = 0;
    int id = -1;

    try {
        //validate
        CV_Assert(AndroidBitmap_getInfo(env, bmp, &info) >= 0);
        CV_Assert(AndroidBitmap_lockPixels(env, bmp, &pixels) >= 0);
        CV_Assert(pixels);

        //type mat
        if(info.format == ANDROID_BITMAP_FORMAT_RGBA_8888){
            Mat tmp(info.height, info.width, CV_8UC4, pixels);
            Mat cvImg;
            //guarda imagen gris o a color!!!!
            //cv::cvtColor(tmp, cvImg, cv::COLOR_RGBA2BGR, 3);
            cv::cvtColor(tmp, cvImg, cv::COLOR_RGBA2GRAY);
            FImage& img = FImageContext::getInstance().createImage(cvImg);
            id = img.getId();
            if( degrees != 0 ){
                img.rotate(degrees);
            }

        } else{
            Mat tmp(info.height, info.width, CV_8UC2, pixels);
            Mat cvImg;
            //cv::cvtColor(tmp, cvImg, cv::COLOR_BGR5552BGR, 3);
            cv::cvtColor(tmp, cvImg, cv::COLOR_BGR5552GRAY);
            FImage& img = FImageContext::getInstance().createImage(cvImg);
            id = img.getId();
            if( degrees != 0 ){
                img.rotate(degrees);
            }
        }
        AndroidBitmap_unlockPixels(env, bmp);

    } catch(cv::Exception e){

        AndroidBitmap_unlockPixels(env, bmp);
        jclass je = env->FindClass("java/lang/Exception");
        env->ThrowNew(je, "OpenCV exception in JNI code");

    } catch (...){
        AndroidBitmap_unlockPixels(env, bmp);
        jclass je = env->FindClass("java/lang/Exception");
        env->ThrowNew(je, "Unknown exception in JNI code");
    }

    return id;
}

/* Saves current recognition model to disk */
extern "C"
void Java_co_edu_fuac_afrecog_FaceContext__1saveRecogModel(JNIEnv *env, jobject instance) {

    AFContext::getInstance().saveRecogModel();
}

/* Loads recognition model from disk and set it as default */
extern "C"
void Java_co_edu_fuac_afrecog_FaceContext__1loadRecogModel(JNIEnv *env, jobject instance) {

    AFContext::getInstance().loadRecogModel();
}

/* Initialize a new configure for detector */
extern "C"
void Java_co_edu_fuac_afrecog_FaceContext__1createDetectorParams(JNIEnv *env, jobject instance) {

    detection_config_map.clear();
}

/* add a configuration item value for future detector creation */
extern "C"
void Java_co_edu_fuac_afrecog_FaceContext__1addDetectorParam(JNIEnv *env, jobject instance, jstring key_,
                                                             jstring value_) {
    const char *key = env->GetStringUTFChars(key_, 0);
    const char *value = env->GetStringUTFChars(value_, 0);

    detection_config_map[key] = value;

    env->ReleaseStringUTFChars(key_, key);
    env->ReleaseStringUTFChars(value_, value);
}

/* creates a new default detector*/
extern "C"
void Java_co_edu_fuac_afrecog_FaceContext__1createDetector(JNIEnv *env, jobject instance, jstring name_) {
    const char *name = env->GetStringUTFChars(name_, 0);

    // add detector name to params map
    detection_config_map["DETECTOR_NAME"] = name;

    // creates detector
    AFContext::getInstance().createDetector( detection_config_map );

    env->ReleaseStringUTFChars(name_, name);
}


/* Initialize a new configure for recognizer */
extern "C"
void Java_co_edu_fuac_afrecog_FaceContext__1createRecognizerParams(JNIEnv *env, jobject instance) {

    recog_config_map.clear();
}

/* add a configuration item value for future recognizer creation */
extern "C"
void Java_co_edu_fuac_afrecog_FaceContext__1addRecognizerParam(JNIEnv *env, jobject instance, jstring key_,
                                                               jstring value_) {
    const char *key = env->GetStringUTFChars(key_, 0);
    const char *value = env->GetStringUTFChars(value_, 0);

    recog_config_map[key] = value;

    env->ReleaseStringUTFChars(key_, key);
    env->ReleaseStringUTFChars(value_, value);
}

/* create a new recognizer and set it as default */
extern "C"
void Java_co_edu_fuac_afrecog_FaceContext__1createRecognizer(JNIEnv *env, jobject instance, jstring name_) {
    const char *name = env->GetStringUTFChars(name_, 0);

    // add detector name to params map
    recog_config_map["RECOGNIZER_NAME"] = name;

    // creates detector
    AFContext::getInstance().createRecognizer( recog_config_map );

    env->ReleaseStringUTFChars(name_, name);
}

AAssetManager* pAssetManager;
// Open file
AAsset* pFile;
// Get the file length
size_t fileLength;
// Allocate memory
char* pFileContent;

extern "C"
jintArray Java_co_edu_fuac_afrecog_puntos__1puntillos(JNIEnv *env, jobject instance, jint img1, jobject mng, jint x, jint y, jint width, jint height) {
    pAssetManager = AAssetManager_fromJava(env, mng);
    // Open file
    pFile = AAssetManager_open(pAssetManager, "shape_predictor_68_face_landmarks.dat", AASSET_MODE_BUFFER);
    // Get the file length
    fileLength = AAsset_getLength(pFile);
    // Allocate memory
    pFileContent = new char[fileLength];
    // Read file
    AAsset_read(pFile, pFileContent, reinterpret_cast<size_t>(fileLength));

    FImage &img = FImageContext::getInstance().getImage(img1);

    Puntos nose;
    vector<cv::Point> puntos;
    nose.puntillos(img, puntos,pFileContent,fileLength,x,y,width,height);

    int size = puntos.size() * 2;

    jintArray result1;
    result1 = env->NewIntArray(size);

    if (result1 == NULL) {

        //debug message
#ifdef __ANDROID__
        __android_log_print(ANDROID_LOG_ERROR, "xxxxx*", "JNI error deteccion de puntos");
#else
        cout << "JNI detectFaces: Out of memory error" << endl;
#endif
        return NULL; // out of memory error thrown
    }
    int i;
    // fill a temp structure to use to populate the java int array
    jint fill1[size];
    for (i = 0; i < puntos.size(); i++) {
        fill1[i * 2] = puntos[i].x;
        fill1[i * 2 + 1] = puntos[i].y;
    }

    // move from the temp structure to the java structure
    env->SetIntArrayRegion(result1, 0, size, fill1);

    return result1;

    return result1;
}
