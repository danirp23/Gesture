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
#ifndef GECOFACES_LBPHRECOGNIZER_H
#define GECOFACES_LBPHRECOGNIZER_H

#include "Recognizer.h"
#include <opencv2/face.hpp>
#include <map>
#include <string>

namespace uac {

    /*
     * Implements LBPH (local binary partition histograms) recognizer provided by OpenCV.
     * This method is not included on standard distribution of OpenCV. Actually, it is a
     * contribution module compiled independently.
     */
    class LBPHRecognizer : public Recognizer{

    public:
        // keys used for initialization
        static const char* RECOGNIZER_NAME;
        static const char* LBPH_RADIUS;
        static const char* LBPH_NEIGHBORS;
        static const char* LBPH_GRID_X;
        static const char* LBPH_GRID_Y;
        static const char* LBPH_IMG_HEIGHT;

    protected:
        /* OpenCV LBPH class */
        cv::Ptr<cv::face::LBPHFaceRecognizer> recognizer;

        // method parameters
        int radius;
        int neighbors;
        int gridX;
        int gridY;

        // input image is scaled to meet this height
        int imgHeight;

    public:
        /* creates an instance of this class */
        static Recognizer &create(std::map<std::string, std::string>& params);
        /* releases a previously created instance of this class */
        static void release(Recognizer& rec);

        /* trains recognizer with the given images and its corresponding labels */
        virtual void train(vector<FImage>& imgs, vector<int>& labels);
        /*
         * recognize given image. It tries to classify image in one of the existing
         * categories (training phase)
         */
        virtual double recognize(FImage& img, int& label);

        /* saves current training model */
        virtual void save(string& filename);
        /* load training model */
        virtual void load(string& filename);

    protected:
        LBPHRecognizer( int radius, int neighbors, int gridX, int gridY, int imgHeight );
        virtual ~LBPHRecognizer();
    };
}

#endif //GECOFACES_LBPHRECOGNIZER_H
