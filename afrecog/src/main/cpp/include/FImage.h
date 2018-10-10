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

#ifndef GECOFACES_FIMAGE_H
#define GECOFACES_FIMAGE_H


#include <opencv2/opencv.hpp>

using namespace cv;

namespace uac {

    /* Wrapper for OpenCV mat class. It represents an Image registered into the library */
    class FImage {

    private:
        /* id assigned to this image by FImageContext. Must be unique */
        int id;
        /* internal OpenCV image */
        Mat mat;

    public:
        /* Don't use it directly. Should only be called by FImageContext */
        FImage(int id, Mat mat);
        virtual ~FImage();

        /* rotates the image */
        virtual void rotate(int degrees);
        /* returns interanl OpenCV mat */
        virtual Mat getMat();

        /* returns unique id */
        inline int getId(){ return id; }
    };
}

#endif //GECOFACES_FIMAGE_H
