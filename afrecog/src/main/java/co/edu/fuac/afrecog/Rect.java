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

package co.edu.fuac.afrecog;


/* Wrapper for Rect class used by native library */
public class Rect {

    public int x;
    public int y;
    public int width;
    public int height;

    public Rect( int x, int y, int width, int height ){

        this.x      = x;
        this.y      = y;
        this.width  = width;
        this.height = height;
    }
}
