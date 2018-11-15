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
import android.support.annotation.NonNull;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.RelativeLayout;
import android.widget.TextView;

import java.io.File;
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
import android.widget.Toast;

import com.bumptech.glide.Glide;
import com.bumptech.glide.load.engine.DiskCacheStrategy;
import com.bumptech.glide.request.RequestOptions;
import com.google.android.gms.tasks.OnSuccessListener;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;
import com.google.firebase.storage.FileDownloadTask;
import com.google.firebase.storage.FirebaseStorage;
import com.google.firebase.storage.StorageReference;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

public class MainActivity extends AppCompatActivity {

    private static final int PICK_IMAGE = 100;
    Uri imageUri;
    TextView tv1;
    // ARMANDO
    private static final String PATH_STARTPOR = "pointsPorcentaje";
    ArrayList<Float> precisionPorcentaje = new ArrayList<Float>();
    private TextView txt;
    private Button btn;
    private EditText edt;
    private ImageView imagen;
    JSONArray puntosJson  = new JSONArray();
    private StorageReference mStorageRef;
    private static final String PATH_START = "pointsImage";
    ArrayList<Rect> porcentajePrecision = new ArrayList<Rect>();
    ArrayList<Rect> pun = new ArrayList<Rect>();
    ArrayList<Rect> puntosGuardar = new ArrayList<Rect>();
    //ARMANDO
    ImageView imagenGil;
    ImageView imagenGil2;
    public int numim;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        imagenGil= (ImageView)findViewById(R.id.imageView);
        imagenGil2= (ImageView)findViewById(R.id.imageView2);
        //Imagenes por boton
        /*imagenGil.setOnClickListener(new View.OnClickListener() {
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
        });*/
        //ARMANDO
        txt = (TextView) findViewById(R.id.textView2);
        btn = (Button) findViewById(R.id.button);
        edt = (EditText) findViewById(R.id.editText);
        imagen = (ImageView) findViewById(R.id.imageView3);
        //ARMANDO

        // Example of a call to a native method
        //TextView tv = (TextView) findViewById(R.id.sample_text);
        tv1= (TextView) findViewById(R.id.textView);
        //tv.setText(stringFromJNI());

        //bitmap= Bitmap.createScaledBitmap(bitmap, bitmap.getWidth()/2, bitmap.getHeight()/2, false);
        //tv.setText(llamado(bitmap));
        //llamado(bitmap2);


        //imagenGil.setImageBitmap(BitmapFactory.decodeResource(this.getResources(),R.drawable.dan));

    }
    //ARMANDO
    //ARMANDO
    public void mostrarPuntos(View v)  {
        final String path = edt.getText().toString();
        FirebaseDatabase database = FirebaseDatabase.getInstance();
        DatabaseReference reference = database.getReference(PATH_START).child(path).child(path);
        reference.addListenerForSingleValueEvent(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                String n = dataSnapshot.getValue().toString();
                try {
                    JSONObject jsonPoints = new JSONObject(n);
                    String lectura = jsonPoints.getString("imgPuntos");
                    puntosJson = jsonPoints.getJSONArray("imgPuntos");
                    for (int j=0; j< puntosJson.length(); j++){
                        String xy = puntosJson.getJSONObject(j).get("xy").toString();// String que tiene xy
                        String xyy [] = xy.split("-"); // split para separar el x y y
                        String x = xyy[0] ;
                        String y = xyy[1];
                        float xF = Float.parseFloat(x);//float de la coordenada x del punto i
                        float yF = Float.parseFloat(y);// float de la coordenada y del punto i
                        puntosGuardar.add(new Rect(Math.round(xF), Math.round(yF),0,0)); // arreglo de recto con solo x y y con cada punto de la iamgen por id
                        txt.setText(puntosGuardar.get(j).x + "----" + puntosGuardar.get(j).y);
                        Log.d("puntos Armando: ", "" + puntosGuardar.size()+"x: "+puntosGuardar.get(j).x + "---- y: " + puntosGuardar.get(j).y);
                        //com.example.personal.proyecto.Rect rect = new com.example.personal.proyecto.Rect();
                    }
                    traerImgByName(path);
                    Toast.makeText(MainActivity.this,"puntosguadar tamaño" + puntosGuardar.size(), Toast.LENGTH_LONG).show();
                    //traerImgByName(path);
                } catch (JSONException e) {
                    e.printStackTrace();
                }

            }
            @Override
            public void onCancelled(@NonNull DatabaseError databaseError) {
                Toast.makeText(MainActivity.this,"No se puede cargar el cosiaso", Toast.LENGTH_LONG).show();
            }
        });

    }
    public void traerImgByName(String nombre) {
        String img = nombre + ".jpg";
        try {
            final File localFile = File.createTempFile(nombre, "jpg");
            mStorageRef = FirebaseStorage.getInstance().getReference();
            mStorageRef.child(img).getFile(localFile)
                    .addOnSuccessListener(new OnSuccessListener<FileDownloadTask.TaskSnapshot>() {
                        @Override
                        public void onSuccess(FileDownloadTask.TaskSnapshot taskSnapshot) {
                            Bitmap bitImg = BitmapFactory.decodeFile(localFile.getPath());
                            llamado(bitImg,1);
                            final RequestOptions options = new RequestOptions()
                                    .centerCrop()
                                    .diskCacheStrategy(DiskCacheStrategy.ALL);
                            Glide.with(MainActivity.this)
                                    .load(localFile)
                                    .apply(options)
                                    .into(imagen);

                            imagen.setVisibility(View.VISIBLE);
                        }
                    });
        } catch (Exception e) {
            Toast.makeText(MainActivity.this, "error" + e, Toast.LENGTH_LONG).show();
        }
    }

    //ARMANDO
    public void llamado(Bitmap bitmap,int numi){
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


            puntos objP = new puntos();
            Log.d("id imagen ", "" + nose.getId());
            AssetManager asset = getBaseContext().getAssets();

            pun = objP.puntillos(numi,nose.getId(), asset, lalala.x, lalala.y, lalala.width + lalala.x, lalala.height + (lalala.y));

            Log.d("puntos ", "" + pun.size());
            imagenGil.setImageResource(R.drawable.boton);
            imagenGil2.setImageResource(R.drawable.boton2);
            Lienzo fondo = new Lienzo(this, bitmap, lalala.width, lalala.height, lalala.x, lalala.y, pun);
            layout1.addView(fondo);
            precision(puntosGuardar,pun);
        }
        //return pun;
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
            //tv1.setText(llamado(bitmap,numim));
        }
    }
    public void guardarPorcentajes(ArrayList<Float> p){
        FirebaseDatabase database = FirebaseDatabase.getInstance();
        DatabaseReference reference = database.getReference(PATH_STARTPOR);
        for(int i = 0 ; i< p.size() ; i++){
            reference.push().child(i+"").setValue(p); // guarda en firebase un objeto con nombre i y valor p
        }
        Toast.makeText(this,"Cargo: "+ p.size(),Toast.LENGTH_LONG).show();
    }
    public void precision(ArrayList<Rect> armandoP,ArrayList<Rect> imgOriginal) {
        for(int i = 0 ; i < armandoP.size() ; i ++){
            float xA = armandoP.get(i).x; // posición x del objeto rect que solo tiene x y y
            float yA = armandoP.get(i).y;// posición y del objeto rect que solo tiene x y y
            float xD = imgOriginal.get(i).x;
            float yD = imgOriginal.get(i).y;
            float precisionX = (((Math.abs(  xA - xD)) / Math.abs(xD)) * 100) ; // 100 - (|aproximado - exacto|/ exacto)*100
            float precisionY = (((Math.abs(  yA - yD)) / Math.abs(yD)) * 100) ; // 100 - (|aproximado - exacto|/ exacto)*100
            float promedio  = (precisionX + precisionY) / 2;
            precisionPorcentaje.add(promedio); // se agrega el valor promedio del error en x y y

        }
        Toast.makeText(MainActivity.this,"precision porcentaje: " + armandoP.size(), Toast.LENGTH_LONG).show();
        guardarPorcentajes(precisionPorcentaje);
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
