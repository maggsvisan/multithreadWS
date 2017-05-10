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
#define NUM_THREADS     5

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

 void *PrintHello(void *threadid)
 {
    long tid;
    tid = (long)threadid;
    printf("Hello World! It's me, thread #%ld!\n", tid);
    pthread_exit(NULL);
 }


void watershed(){

    pthread_t threads[NUM_THREADS];
    int rc;
    long t;

    for(t=0; t<NUM_THREADS; t++){
       printf("In main: creating thread %ld\n", t);
       rc = pthread_create(&threads[t], NULL, PrintHello, (void *)t);
    
    }

    
    
}

 int main (int argc, char *argv[])
 {
    clock_t start,end;
    Mat image= imread("bin.jpg");
    imshow("Original",image);
    Mat newreg(image.rows,image.cols,CV_8UC3,Scalar(255,255,255));
     
    start=clock();

   // watershed(image, newreg);
   
    watershed();

    end=clock();

     printf("time: %ld ", end-start);


    imshow("newreg segmentado", newreg);

    cout<<"el numero de objetos es "<< c <<endl; //aqui vamos a poner el total de la suma de cada thread
    waitKey(0);

    /* Last thing that main() should do */
    pthread_exit(NULL);
    
    return 0;
 }