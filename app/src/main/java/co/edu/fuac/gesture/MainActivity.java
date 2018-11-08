package co.edu.fuac.gesture;

import android.content.Context;
import android.content.Intent;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Point;
import android.net.Uri;
import android.provider.MediaStore;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.RelativeLayout;
import android.widget.TextView;

import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Vector;

import co.edu.fuac.afrecog.FImage;
import co.edu.fuac.afrecog.FImageContext;
import co.edu.fuac.afrecog.FaceContext;
import co.edu.fuac.afrecog.Rect;
import co.edu.fuac.afrecog.puntos;

import android.widget.ImageView;

public class MainActivity extends AppCompatActivity {

    private static final int PICK_IMAGE = 100;
    Uri imageUri;
    TextView tv1;
    ImageView imagenGil;
    ImageView imagenGil2;
    public int numim;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        imagenGil= (ImageView)findViewById(R.id.imageView);
        imagenGil2= (ImageView)findViewById(R.id.imageView2);

        imagenGil.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                imagenGil.setImageResource(R.drawable.botonoprimido);
                numim=1;
                openGallery();
            }
        });
        imagenGil2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                imagenGil2.setImageResource(R.drawable.botonoprimido2);
                numim=2;
                openGallery();
            }
        });

        // Example of a call to a native method
        //TextView tv = (TextView) findViewById(R.id.sample_text);
        tv1= (TextView) findViewById(R.id.textView);
        //tv.setText(stringFromJNI());

        //bitmap= Bitmap.createScaledBitmap(bitmap, bitmap.getWidth()/2, bitmap.getHeight()/2, false);
        //tv.setText(llamado(bitmap));
        //llamado(bitmap2);


        //imagenGil.setImageBitmap(BitmapFactory.decodeResource(this.getResources(),R.drawable.dan));

    }
    public String llamado(Bitmap bitmap,int numi){
        //escalar
        String blabla = "wid original: "+bitmap.getWidth()+" hei original: "+bitmap.getHeight()+"\n";

        bitmap= Bitmap.createScaledBitmap(bitmap, bitmap.getWidth()/2,bitmap.getHeight()/2, false);
        ArrayList<Rect> det = new ArrayList<Rect>();
        FaceContext faceContext= FaceContext.getInstance();
        Context Concon = getApplicationContext();
        faceContext.init(Concon);
        faceContext.createDetector("HAAR",new HashMap<String, String>());
        FImage nose;
        FImageContext image = FImageContext.getInstance();
        nose= image.addImage(bitmap,0);

        det=faceContext.detectFaces(nose);
        for(int j=0;j<det.size();j++) {
            Rect lalala = det.get(j);

            //Log.d("xxxxxxx",""+det.size()+" "+lalala.x);

            blabla = blabla+ "Caras: " + det.size() + ". En x: " + bitmap.getWidth() / 2 + ". En y: " + bitmap.getHeight() / 2 + "\nWidth: " + lalala.width + ". Height: " + lalala.height
                    + "\nbitmap Height: " + bitmap.getWidth() + ". Bitmap Height: " + bitmap.getHeight();

            RelativeLayout layout1 = (RelativeLayout) findViewById(R.id.layout1);

            ArrayList<Rect> pun = new ArrayList<Rect>();
            ArrayList<Rect> contraste = new ArrayList<Rect>();
            ArrayList<Rect> brillo = new ArrayList<Rect>();
            puntos objP = new puntos();
            Log.d("id imagen ", "" + nose.getId());
            AssetManager asset = getBaseContext().getAssets();

            pun = objP.puntillos(numi,nose.getId(), asset, lalala.x, lalala.y, lalala.width + lalala.x, lalala.height + (lalala.y));

            Log.d("puntos ", "" + pun.size());
            imagenGil.setImageResource(R.drawable.boton);
            imagenGil2.setImageResource(R.drawable.boton2);
            Lienzo fondo = new Lienzo(this, bitmap, lalala.width, lalala.height, lalala.x, lalala.y, pun);
            layout1.addView(fondo);
        }
        return blabla;
    }

    class Lienzo extends View {
        Bitmap bitmap1;
        int wi,hei,xx,yy,xxp,yyp,numII;
        ArrayList<Rect> pun2;
        public Lienzo(Context context, Bitmap bitmap, int width, int height, int x, int y, ArrayList<Rect> points) {
            super(context);
            bitmap1=bitmap;
            wi=width;
            hei=height;
            xx=x;
            yy=y;
            pun2=points;
        }

        protected void onDraw(Canvas canvas) {
            canvas.drawBitmap(bitmap1, 0, 0, null);
            android.graphics.Rect rectan = new android.graphics.Rect();

            rectan.set(xx, yy, wi+xx, hei+yy);
            Paint fondo = new Paint();

            // a paint to describe how points are drawn
            Paint paint = new Paint(Paint.ANTI_ALIAS_FLAG);
            paint.setColor(Color.RED);
            float radius = 4f;

            int tot;
            int x1=0,y1 = 0;

            fondo.setColor(Color.GREEN);
            fondo.setStyle(Paint.Style.STROKE);
            canvas.drawRect(rectan, fondo);
            canvas.drawCircle(xxp,yyp,radius,paint);
            float t=pun2.size();
            for(int j=0;t>0;j++) {
                tot= (int) Math.sqrt((pun2.get(j).x*pun2.get(j).x) + (pun2.get(j).y * pun2.get(j).y));

                x1= (int) ((pun2.get(j).x)-(radius/2));
                    y1= (int) ((pun2.get(j).y)-(radius/2));
                Log.d("punto x & y"," "+tot+" x: "+pun2.get(j).x+" y: "+pun2.get(j).y);
                canvas.drawCircle(x1, y1, radius, paint);
                t--;
            }
        }
    }
    private void openGallery(){
        Intent gallery = new Intent(Intent.ACTION_PICK, MediaStore.Images.Media.INTERNAL_CONTENT_URI);
        gallery.setType("image/");
        startActivityForResult(gallery.createChooser(gallery,"Selecciones la app"),10);
    }
    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data){
        if(resultCode == RESULT_OK){
            imageUri = data.getData();
            //imagenGil.setImageURI(imageUri);
            Bitmap bitmap = null;
            try {
                bitmap = MediaStore.Images.Media.getBitmap(this.getContentResolver(), imageUri);
            } catch (IOException e) {
                e.printStackTrace();
            }
            tv1.setText(llamado(bitmap,numim));
        }
    }
    public void precision(ArrayList<Rect> imgInput,ArrayList<Rect> imgOriginal) {
        ArrayList<Float> precisionPuntos = new ArrayList<Float>();//Este guarda el porcentaje de error
        ArrayList<String> valorVectores = new ArrayList<String>(); // revisar el listado de puntos
        float precision = 0;
        for(int i = 0 ; i < imgInput.size() ; i ++ ){
            float vectorPosInputX = imgInput.get(i).x;
            float vectorPosOriX  = imgOriginal.get(i).x;
            float vectorPosInputY = imgInput.get(i).y;
            float vectorPosOriY  = imgOriginal.get(i).y;
            valorVectores.add("\n"+"Vector entrante" + vectorPosInputX+"\n"+"Vector original" + vectorPosOriX);
            precision = (((Math.abs(  vectorPosInputX - vectorPosOriX)) / Math.abs(vectorPosOriX)) * 100) ; // 100 - (|aproximado - exacto|/ exacto)*100
            precisionPuntos.add(precision);
            valorVectores.add("\n"+"Vector entrante" + vectorPosInputY+"\n"+"Vector original" + vectorPosOriY);
            precision = (((Math.abs(  vectorPosInputY - vectorPosOriY)) / Math.abs(vectorPosOriY)) * 100) ; // 100 - (|aproximado - exacto|/ exacto)*100
            precisionPuntos.add(precision);
        }
    }


    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

}
