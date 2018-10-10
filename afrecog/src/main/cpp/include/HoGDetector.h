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
#ifndef GECOFACES_HOGDETECTOR_H
#define GECOFACES_HOGDETECTOR_H

#include <dlib/image_processing/frontal_face_detector.h>
#include "Detector.h"

namespace uac {

    // dlib types used for build HoG face detector.
    typedef dlib::scan_fhog_pyramid<dlib::pyramid_down<6>> image_scanner_type;
    typedef dlib::object_detector<image_scanner_type> detector_type;

    /* Face detector based on HoG method provided by DLib */
    class HoGDetector : public Detector {

    public:
        // keys for initialization
        static const char* DETECTOR_NAME;
        static const char* HOG_LEVELS;
        static const char* HOG_NUM_PARTS;
        static const char* IMG_PROC_SIZE;

    private:
        /* dlib detector class */
        dlib::frontal_face_detector detector;

        /* is detector already initialized? */
        bool bInit;

        // method parameters
        int pyramidLevels;
        int numParts;

        // input image is scaled to meet this height
        int maxSide;

    public:
        /* creates an instance of this detector */
        static Detector &create(std::map<std::string, std::string>& params);
        /* releases an instance of this detector */
        static void release(Detector& det);

        /* detect faces locations on given image */
        virtual void detect(FImage &img, vector<cv::Rect> &det);

        // getters and setters
        inline void setMaxSize( int n ){
            maxSide = n;
        }

        inline int getMaxSide(){
            return maxSide;
        }

        inline void setPyramidLevels( int n ){
            pyramidLevels = n;
        }

        inline void setNumParts( int n ){
            numParts = n;
        }

    protected:
        HoGDetector( int numParts, int pyramidLevels, int maxImgSize );
        virtual ~HoGDetector();
    };

}


#endif //GECOFACES_HOGDETECTOR_H
