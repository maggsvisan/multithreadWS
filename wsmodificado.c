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
clock_t start,end;

struct myStruct{
  int thRow;
  int index;
  int end;
  int thId;
  int r;
  int g;
  int b;
}thStruct[NUM_THREADS];

Mat image= imread("imagen2.png");
Mat newreg(image.rows,image.cols,CV_8UC3,Scalar(255,255,255));
int baseIntervalo;
 
void *myThreadRoutine(void *thStr){

queue<Point> coords; //guarda los vecinos de objetos
queue<Point> temp;
Point p, ptemp;

  int xp=0;
  int yp=0;
  int bb=0;
  int gg=0;
  int rr=0;

  struct thStruct *th =(struct thStruct *)thStr;

  int rowNum = th->thRow;
  int tid = th->thId;
  int inicio = th->index;
  int final = th->end;
  int rr = th->r;
  int gg = th->g;
  int bb = th->b;


pthread_mutex_lock(&mutexc);
for (int j=inicio; j <= final ; j++)
{//las columnas son el eje de las x
    for (int i=1; i<rowNum; i++)
      {//las filas son el eje de las y  
        //if (tid ==9)       
            // cout<< j<<","<<i<< endl;  
           if ((image.at<Vec3b>(i,j)== Vec3b(0,0,0) && (newreg.at<Vec3b>(i,j)== Vec3b(255,255,255))))// de la semilla
           {
            p.x=j;
            p.y=i;
            coords.push(p); //se guardanlas coordenadas
            c++; //contador de objetos
            //creamos rgb randoms
            
            newreg.at<Vec3b>(i, j)= Vec3b(bb,gg,rr); //pintamos pixel
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
   pthread_mutex_unlock(&mutexc);
}   

void watershed(Mat image, Mat newreg){
    int row=0;
    row= image.cols;
    //erode(image, image, Mat(), Point(-1,-1),2);
    pthread_t threads[NUM_THREADS];
    int rc;
    long t;
    void *status;
    
    baseIntervalo= row / NUM_THREADS; //intervalo por thread, base
    start=clock();

    for(t=0; t<NUM_THREADS; t++){
      thStruct[t].thRow = baseIntervalo;
      thStruct[t].thId = t;
      thStruct[t].index = t*baseIntervalo;
      thStruct[t].r = rand()%256;
      thStruct[t].g = rand()%256;
      thStruct[t].b = rand()%256;
      if(i==NUM_THREADS-1){
        thStruct[i].end = image.cols;
      }
      else{
        thStruct[t].end = (t+1)*baseIntervalo;
      }
      pthread_create(&threads[t], NULL , myThreadRoutine,&thStruct[i]);
    }
    for(t=0; t<NUM_THREADS; t++){
      pthread_join(threads[t], &status);
    }
    end=clock();

}

int main (int argc, char *argv[])
{
/*
    cout<<"cols"<< image.cols<<endl;
    cout<<"rows"<< image.rows<<endl;
*/
  imshow("Original",image);
  watershed(image, newreg);
  printf("time: %ld ", end-start);
  imshow("newreg segmentado", newreg);
  cout<<"el numero de objetos es "<< c <<endl; //aqui vamos a poner el total de la suma de cada thread
  waitKey(0);
  pthread_mutex_destroy(&mutexc);
  /* Last thing that main() should do */
  return 0;
}