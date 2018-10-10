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

/*
 * Represents an image or a fragment of it. It is a wrapper for the class with same name into the
 * library
 */
public class FImage {

    private int id;
    private boolean isActive;

    private FImage(){
        // avoid creation without id
    }

    public FImage( int id ){
        this.id  = id;
        isActive = true;
    }

    /* The image is not longer needed */
    public void close(){

        isActive = false;
    }

    public int getWidth(){

        if( isActive ){
            return FImageContext.getInstance().getWidth(this);
        }

        return 0;
    }

    public int getHeight(){

        if( isActive ){
            return FImageContext.getInstance().getHeight(this);
        }

        return 0;
    }

    public int getId(){
        return id;
    }

    /* If image is destroyed, remove image from library */
    @Override
    protected void finalize() throws Throwable {

        if( isActive ){
            FImageContext.getInstance().removeImage(this);
        }

        super.finalize();
    }
}
