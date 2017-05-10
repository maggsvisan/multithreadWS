#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <opencv/cv.h>
#include <opencv2/contrib/contrib.hpp>
#include <string>
#include <fstream>
#include "opencv2/opencv.hpp"
#include <iostream>
#include <string>
#include <queue>
#include <time.h>

using namespace cv;
using namespace std;


queue<Point> coords; //guarda los vecinos de objetos
queue<Point> temp;
Point p, ptemp;
int c = 0; //num de objetos
int xp=0;
int yp=0;
int r=0;
int g=0;
int b=0;
int bb=0;
int gg=0;
int rr=0;


//generate random colors
void colors(int &bb, int &gg, int &rr){

  bb= theRNG().uniform(0,255);
  gg= theRNG().uniform(0,255);
  rr= theRNG().uniform(0,255);

}

void watershead(Mat image, Mat newreg){

  erode(image,image, Mat(), Point(-1,-1),2);
  //imshow("fg", image);

  for (int j=1; j < image.cols; j++){//las columnas son el eje de las x
  for (int i=1; i< image.rows; i++){//las filas son el eje de las y
 
           if ((image.at<Vec3b>(i,j)== Vec3b(0,0,0) && (newreg.at<Vec3b>(i,j)== Vec3b(255,255,255))))// de la semilla
           {
            p.x=j;
            p.y=i;
            coords.push(p); //se guardanlas coordenadas
           
            c++; //contador de objetos
            
            colors(bb, gg, rr);  //generate random colors

            newreg.at<Vec3b>(i, j)= Vec3b(bb,gg,rr);

            while(!coords.empty())
                            {  
                               ptemp=coords.front();
                             
                              xp= ptemp.x;
                              yp= ptemp.y;
                             
                                  //y , x   
              if ( (image.at<Vec3b>(yp,xp+1)== Vec3b(0,0,0)) && (newreg.at<Vec3b>(yp,xp+1)== Vec3b(255,255,255)))//right
              {
                      newreg.at<Vec3b>(yp, xp+1)= Vec3b(bb,rr,gg);//pintamos el punto 
                      ptemp.x= xp+1;
                                  ptemp.y= yp;
                                  coords.push(ptemp);
              }
                
              if ( (image.at<Vec3b>(yp+1,xp)== Vec3b(0,0,0)) && (newreg.at<Vec3b>(yp+1, xp) == Vec3b(255,255,255))) //up
              {
              newreg.at<Vec3b>(yp+1,xp)= Vec3b(bb,rr,gg);//pintamos el punto
                        ptemp.x= xp;
                                  ptemp.y= yp+1;
                                  coords.push(ptemp);
              }

              if ( (image.at<Vec3b>(yp,xp-1)== Vec3b(0,0,0)) && (newreg.at<Vec3b>(yp, xp-1) == Vec3b(255,255,255))) //left
              {
              newreg.at<Vec3b>(yp,xp-1)= Vec3b(bb,rr,gg);//pintamos el punto

                      ptemp.x= xp-1;
                                  ptemp.y= yp;
                                  coords.push(ptemp);


              }

              if ( (image.at<Vec3b>(yp-1,xp)== Vec3b(0,0,0)) && (newreg.at<Vec3b>(yp-1, xp) == Vec3b(255,255,255))) //Down
              {
              newreg.at<Vec3b>(yp-1,xp)= Vec3b(bb,rr,gg);//pintamos el punto
                      ptemp.x= xp;
                                  ptemp.y= yp-1;
                                  coords.push(ptemp);
              }
                   coords.pop();

              }//del while */

                          } //del if semilla/*
                     }// for /*
                     
              }//for/*

}


int main(int argc, char* argv[]){

clock_t start,end;
Mat image= imread("bin.jpg");
imshow("Original",image);
Mat newreg(image.rows,image.cols,CV_8UC3,Scalar(255,255,255));
 
start=clock();

watershead(image, newreg);

end=clock();

 printf("time: %ld ", end-start);


imshow("newreg segmentado", newreg);
cout<<"el numero de objetos es "<< c <<endl;
    waitKey(0);
    return 0;

}// del  main 
