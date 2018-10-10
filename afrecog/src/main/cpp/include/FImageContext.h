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
#ifndef GECOFACES_FIMAGECONTEXT_H
#define GECOFACES_FIMAGECONTEXT_H

#include "FImage.h"
#include "FImageRect.h"
#include <map>

using namespace std;
using namespace cv;

namespace uac{

    /*
     * Manages the images used by algorithms. It allows create FImages from raw pixels (array) or
     * from OpenCV images. It also allows to create new images from regions of existing images.
     *
     */
    class FImageContext{

    private:
        static FImageContext instance;

        /* all images current active on the library*/
        map<int, FImage*> imgsMap;
        /* all internal opencv images */
        map<int, Mat>     cvMap;
        /* next id to be assigned */
        int               lastId;

    private:
        FImageContext();
        ~FImageContext();

    public:
        /* creates an image from given raw pixels. Format parameter uses same values as
         * format parameter of OpenCV (Mat constructor) */
        FImage& createImage( unsigned char* data, int width, int height, int format );
        /* creates an image and uses given OpenCV image as internal image */
        FImage& createImage( Mat mat );
        /* creates a new image from a sub-region of given image */
        FImageRect& createImageRect( FImage& parent, int x, int y, int w, int h );
        /* removes a previously created image */
        void removeImage( FImage& img );
        /* removes a previously created image */
        void removeImage( int id );
        /* remove all images */
        void release();

        // getters and setters
        FImage& getImage( int id );

        static FImageContext& getInstance(){
            return instance;
        }
    };

};

#endif //GECOFACES_FIMAGECONTEXT_H
