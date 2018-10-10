package co.edu.fuac.afrecog;

import android.content.res.AssetManager;
import android.graphics.Bitmap;

import java.util.ArrayList;

public class puntos {

    //private AssetManager asset;
    public puntos(){

    }
    public ArrayList<Rect> puntillos(int nob,int img,AssetManager asset,int x1, int y1, int width, int height){
        int[] detArray = _puntillos(nob,img,asset,x1,y1,width,height);

        ArrayList<Rect> det = new ArrayList<Rect>();
        for( int i=0; i<detArray.length/2; i++ ){
            int x      = detArray[2*i];
            int y      = detArray[2*i+1];
            Rect d = new Rect(x,y,0,0);
            det.add(d);
        }

        return det;
    }

    protected native int[] _puntillos(int nob,int id, AssetManager mng, int x, int y, int width, int height);



}
