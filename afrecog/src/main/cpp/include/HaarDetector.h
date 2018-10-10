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


#ifndef GECOFACES_HAARDETECTOR_H
#define GECOFACES_HAARDETECTOR_H

#include "Detector.h"
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/opencv.hpp>
#include <map>

namespace uac {

    /*
     * Face detector based on OpenCV implementation of Haar filters.
     */
    class HaarDetector : public Detector {

    public:
        // keys used for initialization
        static const char* DETECTOR_NAME;
        static const char* HAAR_SCALE_FACTOR;
        static const char* HAAR_MIN_NEIGHBORS;
        static const char* HAAR_MIN_SIZE;
        static const char* IMG_PROC_SIZE;

    protected:
        /* Opencv class */
        cv::CascadeClassifier faceCascade;
        /* Method is already initialized? */
        bool bInit;

        // parameters used by HAAR method
        double scaleFactor;
        int  minNeighbors;
        cv::Size minSize;

        // input image is scaled to meet this height
        int  maxImgSide;

    public:
        /* Creates an instance of this detector */
        static Detector &create(map<std::string,std::string>& params);
        /* Release and instance of this detector */
        static void release(Detector& det);

        /* detects faces locations on given image */
        virtual void detect(FImage &img, vector<cv::Rect> &det);

        // getters and setters
        inline void setScaleFactor( double s ){
            scaleFactor = s;
        }

        inline double getScaleFactor(){
            return scaleFactor;
        }

        inline void setMinNeigbors( int n ){
            minNeighbors = n;
        }

        inline int getMinNeigbors(){
            return minNeighbors;
        }

        inline void setMinSize( cv::Size& size ){
            minSize = size;
        }

        inline cv::Size& getMinSize(){
            return minSize;
        }

        inline void setMaxImgSize( int n ){
            maxImgSide = n;
        }

        inline int getMaxImgSide(){
            return maxImgSide;
        }

    protected:
        HaarDetector( double scaleFactor, int minNeighbors, cv::Size minSize, int maxImgSize );
        virtual ~HaarDetector();

    };
}

#endif //GECOFACES_HAARDETECTOR_H
