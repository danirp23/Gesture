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
import android.hardware.camera2.params.Face;
import android.util.Log;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Hashtable;

/*
 * Main access point to face detection and recognition algorithms. It initializes default algorithms
 * and makes calls to native library.
 */
public class FaceContext {

    private static FaceContext instance = new FaceContext();

    // Keep a register of images and labels used for training phase of recognition.
    private ArrayList<FImage> trainRecogImgs = new ArrayList<FImage>();
    private ArrayList<Integer> trainRecogLabels = new ArrayList<Integer>();

    private boolean initialized;

    public static FaceContext getInstance(){

        return instance;
    }

    public FaceContext(){

        initialized = false;
    }

    /* Initializes algorithms, defines configuration files and resource path. Requires context
    of Android app. Should be called before any call to library */
    public void init( Context ctx ){//privado

        if( !initialized ) {
            initialized = true;

            // move descriptors to internal storage
            moveToInternal(ctx, "haarcascade_frontalface_alt.xml");

            // pass internal storage dir to library
            File rootDir = ctx.getFilesDir();

            //Log.d("xxxxxxxxxxxxx", "resource dir: " + rootDir.getAbsolutePath());
            _config(rootDir.getAbsolutePath());
            _init();

            trainRecogImgs.clear();
            trainRecogLabels.clear();
        }
    }

    /* Free memory. Library should not be used after this */
    public void close(){

        // remove images
        for( int i=0; i< trainRecogImgs.size(); i++ )
        {
            FImage img = trainRecogImgs.get(i);
            FImageContext.getInstance().removeImage(img);
        }

        trainRecogImgs.clear();
        trainRecogLabels.clear();
        _close();
    }

    /* creates a new detector of the given type and set it as default */
    public void createDetector(String name, HashMap<String, String> params){

        assert( name.trim().length() > 0 );

        _createDetectorParams();
        for ( String key : params.keySet() ) {

            String value = params.get(key);
            if( value != null && value.trim().length() > 0 ) {
                _addDetectorParam(key, value);
            }
        }
        _createDetector( name );
    }

    /* creates a new recognizer of the given type and set it as default */
    public void createRecognizer(String name, HashMap<String, String> params){

        assert( name.trim().length() > 0 );

        _createRecognizerParams();
        for ( String key : params.keySet() ) {

            String value = params.get(key);
            if( value != null && value.trim().length() > 0 ) {
                _addRecognizerParam(key, value);
            }
        }
        _createRecognizer( name );
    }

    /* Moves a file from assets to disk. It facilitates access of files from native library */
    public void moveToInternal( Context ctx, String filename ){

        // get asset manager
        AssetManager am = ctx.getAssets();
        File rootDir = ctx.getFilesDir();

        try {

            File outFile = new File(rootDir, filename);
            if( !outFile.exists() ){

                // open stream from asset
                InputStream is = am.open(filename);
                BufferedInputStream bis = new BufferedInputStream(is);

                // create stream to write on internal storage
                BufferedOutputStream bos = new BufferedOutputStream(new FileOutputStream(outFile));
                byte[] contents = new byte[1024];

                // write contents
                int n = 0;
                while ((n = bis.read(contents)) != -1) {
                    bos.write(contents, 0, n);
                }
                bos.close();
                bis.close();
            }

        } catch (IOException e) {
            e.printStackTrace();
            Log.e( "FaceContext", e.toString() );
        }
    }

    /* clean training images. Needed to start a new training phase*/
    public void startTrainRecog(){

        trainRecogImgs.clear();
        trainRecogLabels.clear();
    }

    /* adds a new training image to training samples (for recognition) */
    public boolean addTrainSample(FImage img, Rect r, int label ){

        FImage region = FImageContext.getInstance().getRegion( img, r );
        if( region != null ) {
            trainRecogImgs.add(region);
            trainRecogLabels.add(label);
            return true;
        }

        return false;
    }

    /* calls to detection method and converts results to Java Rect objects */
    public ArrayList<Rect> detectFaces(FImage img){

        int[] detArray = _detectFaces(img.getId());

        ArrayList<Rect> det = new ArrayList<Rect>();
        for( int i=0; i<detArray.length/4; i++ ){
            int x      = detArray[4*i];
            int y      = detArray[4*i+1];
            int width  = detArray[4*i+2];
            int height = detArray[4*i+3];
            Rect d = new Rect(x,y,width,height);
            det.add(d);
        }

        return det;
    }

    /* train recognition algorithm with given training samples and labels */
    public void trainRecog(ArrayList<FImage> imgs, ArrayList<Integer> imgLabels){

        assert( imgs.size() == imgLabels.size() );

        int size = imgs.size();
        if( size > 0 ) {
            int ids[] = new int[size];
            int labels[] = new int[size];

            for( int i=0; i<size; i++ ){
                ids[i] = imgs.get(i).getId();
                labels[i] = imgLabels.get(i);
            }
            _trainRecog(ids, labels);
        }
    }

    /* train recognition algorithm with current training samples and labels */
    public void trainRecog(){

        assert( trainRecogImgs.size() == trainRecogLabels.size() );

        if( trainRecogImgs.size() == 0 ) {
            //TODO: log
            return;
        }

        int size = trainRecogImgs.size();
        if( size > 0 ) {
            int ids[] = new int[size];
            int labels[] = new int[size];

            for( int i=0; i<size; i++ ){
                ids[i] = trainRecogImgs.get(i).getId();
                labels[i] = trainRecogLabels.get(i);
            }

            _trainRecog(ids, labels);
        }
    }

    /* recognize given image based on training */
    public int recognize( FImage img, RecogData result ){

        double[] confidence = new double[1];
        int[]    cat = new int[1];
        _recognize(img.getId(), confidence, cat);
        result.category = cat[0];
        result.confidence = confidence[0];

        return result.category;
    }

    /* recognize given image based on training */
    public int recognize( FImage img, Rect r, RecogData result ){

        FImage region = FImageContext.getInstance().getRegion( img, r );
        if( region != null ) {

            double[] confidence = new double[1];
            int[] cat = new int[1];
            _recognize(region.getId(), confidence, cat);
            result.category = cat[0];
            result.confidence = confidence[0];

            FImageContext.getInstance().removeImage(region);
            return result.category;
        }

        return -1;
    }

    /* ensure release of native resources */
    protected void finalize() {
        close();
    }

    /* saves recognition model to disk */
    public void saveRecogModel(){
        _saveRecogModel();
    }

    /* loads previously saved model */
    public void loadRecogModel(){
        _loadRecogModel();
    }

    // getters and setters
    public ArrayList<FImage> getTrainRecogImgs() {
        return trainRecogImgs;
    }

    public ArrayList<Integer> getTrainRecogLabels() {
        return trainRecogLabels;
    }

    // native methods
    protected native void _config(String path);
    protected native void _init();
    protected native int[] _detectFaces(int id);
    protected native void _close();
    protected native void _trainRecog(int[] ids, int[] labels);
    protected native void _recognize(int id, double[] c, int[] cat);
    protected native void _saveRecogModel();
    protected native void _loadRecogModel();

    protected native void _createDetectorParams();
    protected native void _addDetectorParam(String key, String value);
    protected native void _createDetector(String name);
    protected native void _createRecognizerParams();
    protected native void _addRecognizerParam(String key, String value);
    protected native void _createRecognizer(String name);

    static {
        System.loadLibrary("afrecog");
    }

}
