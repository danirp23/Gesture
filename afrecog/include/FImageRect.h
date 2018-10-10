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

#ifndef GECOFACES_FIMAGERECT_H
#define GECOFACES_FIMAGERECT_H

#include "FImage.h"


namespace uac{

    /*
     * Represents an image that originates from a sub-section of another image
     */
    class FImageRect : public FImage {

    private:
        // position at parent image
        int   x;
        int   y;
        int   width;
        int   height;

        /* original image */
        Mat   parentMat;

    public:
        FImageRect(int id,Mat mat,Mat parentMat, int x, int y, int width, int height );
        virtual ~FImageRect();
    };

}
#endif //GECOFACES_FIMAGERECT_H
