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
#include "include/FImage.h"

using namespace uac;

FImage:: FImage(int i, Mat m):
        mat(m), id(i)
{
}

FImage:: ~FImage(){

    //Nothing to do
}

Mat FImage:: getMat(){

    return mat;
}

void FImage:: rotate(int degrees){

    Mat dst;

    Size src_sz = mat.size();
    Size dst_sz(src_sz.height, src_sz.width);

    int len = std::max(mat.cols, mat.rows);
    Point2f center(len/2.0, len/2.0);
    Mat rot_mat = cv::getRotationMatrix2D(center, degrees, 1.0);
    warpAffine(mat, dst, rot_mat, dst_sz);

    mat = dst;
}

