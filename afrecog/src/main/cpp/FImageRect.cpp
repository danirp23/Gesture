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
#include "include/FImageRect.h"

using namespace cv;
using namespace uac;

FImageRect::FImageRect(int id,Mat mat, Mat parent, int x, int y, int width, int height )
    :FImage(id, mat), parentMat(parent), x(x), y(y), width(width), height(height)
{

}

FImageRect::~FImageRect(){

    //Nothing to do
}

