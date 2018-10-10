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

import android.content.Context;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.os.Debug;
import android.util.Log;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

/*
 * Manages images used by library. An image registered in this context is converted to a FImage. An
 * FImage is needed by the detection and recognition algorithms. In the case of Android, original
 * Image is of type Bitmap, when converted, a internal copy of it is mantained by this context.
 */
public class FImageContext {

    private static FImageContext instance = new FImageContext();

    public static FImageContext getInstance(){
        return instance;
    }

    public FImageContext(){
    }

    /*
     * Creates a FImage from a Bitmap. The FImage is a copy, it is a gray version of the pixels
     * from original bitmap.
     */
    public FImage addImage(Bitmap bmp, int degrees){

        int id = _addImage(bmp, degrees);
        if( id == -1 ){
            return null;
        }

        FImage f = new FImage(id);
        return f;
    }

    /* Removes an already registered image */
    public void removeImage(FImage img){

        _removeImage(img.getId());
        img.close();
    }

    /* Gets a new Image from a sub-section of the given image */
    public FImage getRegion(FImage img, Rect rect){

        int id = _createImageRect( img.getId(), rect.x, rect.y, rect.width, rect.height );
        if( id == -1 ){
            return null;
        }
        FImage f = new FImage(id);
        return f;
    }

    // getters and setters
    public int getWidth(FImage img){
        return _getWidth(img.getId());
    }

    public int getHeight(FImage img){
        return _getHeight(img.getId());
    }

    // native methods
    protected native int  _addImage(Bitmap bmp, int degrees);
    protected native int  _createImageRect(int id, int x, int y, int w, int h);
    protected native void _removeImage(int id);
    protected native int  _getWidth(int id);
    protected native int  _getHeight(int id);
    //protected native void _test(int id, Bitmap bmp);

    static {
        System.loadLibrary("afrecog");
    }
}
