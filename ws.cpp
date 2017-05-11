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
#include <pthread.h>
#include <stdio.h>

#define NUM_THREADS 1

using namespace cv;
using namespace std;

int c = 0; //num de objetos locales por thread
pthread_mutex_t mutexc;

Mat image= imread("imagen2.png");
Mat newreg(image.rows,image.cols,CV_8UC3,Scalar(255,255,255));
int baseIntervalo;

void *myThreadRoutine(void *threadid){

queue<Point> coords; //guarda los vecinos de objetos
queue<Point> temp;
Point p, ptemp;

    int xp=0;
    int yp=0;

    int bb=0;
    int gg=0;
    int rr=0;
  
  long tid;
  tid = (long)threadid;

  int inicio= tid*baseIntervalo+1;
  int final= (tid+1)*baseIntervalo;

  
  for (int j=inicio; j <= final ; j++){//las columnas son el eje de las x
    for (int i=1; i<image.rows; i++){//las filas son el eje de las y
        
        //if (tid ==9)
            
            // cout<< j<<","<<i<< endl;  

           if ((image.at<Vec3b>(i,j)== Vec3b(0,0,0) && (newreg.at<Vec3b>(i,j)== Vec3b(255,255,255))))// de la semilla
           {
            p.x=j;
            p.y=i;
            coords.push(p); //se guardanlas coordenadas
           
            pthread_mutex_lock(&mutexc);
                c++; //contador de objetos
            pthread_mutex_unlock(&mutexc);

           // colors(bb, gg, rr);  //generate random colors

            //generate random colors
            /*  bb= theRNG().uniform(0,255);
              gg= theRNG().uniform(0,255);
              rr= theRNG().uniform(0,255);
            */

              int range= 255+1;
              bb= rand()%range;
              rr= rand()%range;
              gg= rand()%range;

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

              }

                    } 
    }
                     
   }
   
}   


void watershed(Mat image, Mat newreg){

    int row=0;
    row= image.cols;
    //erode(image, image, Mat(), Point(-1,-1),2);

    pthread_t threads[NUM_THREADS];
    int rc;
    long t;
    void *status;

    //joined threads
   // pthread_attr_t attr;

    /* Initialize and set thread detached attribute 
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);*/

    baseIntervalo= row / NUM_THREADS; //intervalo por thread, base

    for(t=0; t<NUM_THREADS; t++){
      // printf("In main: creating thread %ld\n", t);
       rc = pthread_create(&threads[t], NULL , myThreadRoutine, (void *)t);
    }

    /* Free attribute and wait for the other threads */
 //   pthread_attr_destroy(&attr);

    for(t=0; t<NUM_THREADS; t++) {
       rc = pthread_join(threads[t], &status);
    }

}

 int main (int argc, char *argv[])
 {
    clock_t start,end;
   
/*
    cout<<"cols"<< image.cols<<endl;
    cout<<"rows"<< image.rows<<endl;
*/
    imshow("Original",image);
    

    start=clock();

    watershed(image, newreg);

    end=clock();

     printf("time: %ld ", end-start);

    imshow("newreg segmentado", newreg);

    cout<<"el numero de objetos es "<< c <<endl; //aqui vamos a poner el total de la suma de cada thread
    waitKey(0);

    pthread_mutex_destroy(&mutexc);
    /* Last thing that main() should do */
  
    
    return 0;
 }
