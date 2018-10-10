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

#ifndef GECOFACES_AFCONTEXT_H
#define GECOFACES_AFCONTEXT_H

#include <string>
#include "Detector.h"
#include "Recognizer.h"

using namespace std;

namespace uac {

    /*
     * Main access point to current detection algorithms. It initializes the algorithms with default
     * values. It also initializes the resource folder.
     *
     * This Singleton class offers simple methods to make detection, training and recognition on FImages.
     */
    class AFContext {

    public:
        /* keys used for initialization */
        static const char* DETECTOR_NAME;
        static const char* RECOGNIZER_NAME;

    protected:
        static AFContext mInstance;

        /* path used to load configuration files, needed by detection or recognition algorithms */
        string rootPath;
        /* file name of recognition model */
        string recogFilename;

        /* user detector */
        Detector* userDetector;
        /* user recognizer */
        Recognizer* userRecognizer;

        /* current detector, by default: HoG */
        Detector* currentDetector;
        /* current recognizer, by default: LBPH */
        Recognizer* currentRecognizer;

        /* percentage of increment used when adjusting detection rects */
        float percIncX;
        float percIncY;

        AFContext();
        virtual ~AFContext();

    public:
        /* set path that can be used to load configuration files */
        void setRootPath( const char* path );
        /* get path used to load configuration files */
        const string& getRootPath();

        /* initializes default detector and recognizer and descriptors*/
        void   init();
        /* creates and configures detector*/
        void  createDetector( map<string, string>& params );
        /* sets detector to use*/
        void  setDetector( Detector& detector );
        /* creates and configures recognizer*/
        void   createRecognizer( map<string, string>& params );
        /* sets recognizer to use*/
        void  setRecognizer( Recognizer& recog );
        /* free memory. No operations should be used after this call. */
        void   close();
        /* detects faces on given image (img). It gives a collection of rects
         * with position of detected faces (faceRects) */
        void   detectFaces(FImage& img, vector<Rect>& facesRects);
        /* recognizes the given (img), and classifies it into one of the categories registered
         * previously on training phase. When applies (depends on recognition method) it returns
         * a confidence value for the result */
        double recognize(FImage& img, int& category);
        /* Use the given images and labels to train the recognition method. Each label must
         * correspond to an image */
        int trainRecog( vector<FImage>& vecImgs, vector<int>& vecLabels, bool bRandomNeg );
        /* It tries to adjust rectangle to fit whole face*/
        void adjustRects( FImage& img, vector<Rect>& facesRects );
        /* Saves recognition model */
        void saveRecogModel();
        /* Restores recognition model */
        void loadRecogModel();

        //getters and setters
        void setPercIncX( float val ){
            percIncX = val;
        }

        void setPercIncY( float val ){
            percIncY = val;
        }

        static AFContext& getInstance(){
            return mInstance;
        }
    };
}

#endif //GECOFACES_AFCONTEXT_H
