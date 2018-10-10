//
// Created by Daniela R. Pedraza on 22/08/2018.
//


#include <dlib/opencv.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <android/log.h>
#include "include/Puntos.h"
#include <streambuf>
#include <math.h>
using namespace uac;
using namespace cv;
using namespace dlib;

dlib::shape_predictor pose;
void Puntos::puntillos(FImage& img ,std::vector<cv::Point>& puntos, char* pFileContent, size_t fileLength,int x, int y, int width, int height ) {
    //mat de la imagen
    cv::Mat imagenMat = img.getMat();
    cv::Mat imagenGris;
    //dlib::cv_image<unsigned char> cimg(imagenMat);
    if(imagenMat.channels()==1){
        imagenGris =imagenMat;

    }else{
        cvtColor(imagenMat,imagenGris, CV_BGR2GRAY);
        dlib::cv_image<bgr_pixel> cimg(imagenMat);
    }//Notas para mi:
    //creo que debes volver la imagen a gris
    //no olvides mirar https://www.pyimagesearch.com/2017/04/03/facial-landmarks-dlib-opencv-python/

    //equalizar

    double alpha = 1.3;
    int beta = 40;
    //equalizeHist( imagenGris, imagenGris );
    //dlib::shape_predictor<imagenGris,dlib::cv_image<unsigned char> dlImg(imagenGris);

    //dlib::cv_image<unsigned char> dlImg(new_image);
    //Mat scImg;
    //cv::resize(imagenMat, scImg, cv::Size(),1.0/4, 1.0/4);
    if(img.getId()<=1) {
        membuf sbuf(pFileContent, pFileContent + fileLength);
        std::istream testStream(&sbuf);

        try {
            dlib::deserialize(pose, testStream);
        } catch (serialization_error &e) {
            __android_log_print(ANDROID_LOG_ERROR, "error", "  JNI deteccion de puntos paila");
        }
    }
    equalizeHist( imagenGris, imagenGris );
    /*
    dlib::cv_image<unsigned char> imgBrillo (new_image);
    alpha = 0.3;
    beta = 30;
    for( int y = 0; y < imagenGris.rows; y++ ) {
        for( int x = 0; x < imagenGris.cols; x++ ) {
            for( int c = 0; c < 3; c++ ) {
                new_image.at<Vec3b>(y,x)[c] =
                        saturate_cast<uchar>( alpha*( imagenGris.at<Vec3b>(y,x)[c] ) + beta );
            }
        }
    }*/
    //dlib::cv_image<unsigned char> imgContraste (new_image);
    dlib::cv_image<unsigned char> normal(imagenGris);


    std::vector<full_object_detection> detects;
    //std::vector<full_object_detection> detectsBrillo;
    //std::vector<full_object_detection> detectsContraste;
    //FImageContext:: getImage(img.getId());
    frontal_face_detector jmjm;
    //std::vector<dlib::rectangle> cara = jmjm(dlImg,0);
    dlib::rectangle nose2(x,y,width,height);

    dlib::full_object_detection shape = pose(normal, nose2);
    //dlib::full_object_detection shapeBrillo = pose(imgBrillo, nose2);
    //dlib::full_object_detection shapeContraste = pose(imgContraste, nose2);
    detects.push_back(shape);
    //detects.push_back(shapeBrillo);
    //detects.push_back(shapeContraste);
    float tot;
    for (unsigned int n = 0; n < shape.num_parts(); n++) {
        // guardar las coordenadas de los puntillos
        puntos.push_back(cv::Point(shape.part(n).x(), shape.part(n).y()));
        //brillo.push_back(cv::Point(shapeBrillo.part(n).x(), shapeBrillo.part(n).y()));
        //contraste.push_back(cv::Point(shapeContraste.part(n).x(), shapeContraste.part(n).y()));
    }
}
void carga(char* pFileContent, size_t fileLength){


    membuf sbuf(pFileContent, pFileContent + fileLength);
    std::istream testStream(&sbuf);

    try{
        dlib::deserialize(pose, testStream);
    }catch(serialization_error& e)
    {
        __android_log_print(ANDROID_LOG_ERROR,"error", "  JNI deteccion de puntos paila");
    }
}
