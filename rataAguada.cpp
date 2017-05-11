#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "SDL/SDL.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <iostream>
#include <queue>
#include <pthread.h>

#define numThreads 8


using namespace std;
using namespace cv;

void gota_aceite(int x, int y,int r, int g, int b);
Mat image;
Mat result;
Point p;
queue<Point> qpoints; 
int max_width, max_height;

clock_t start,end;

pthread_mutex_t cs;

struct params{
	int rowPerTh;
	int RowIndx;
	int ThNumber;
	int r,g,b;
}thPar[numThreads];

void *thRtn(void * p){
	int yy,xx;
	int px,py;
	struct params *q = (struct params *)p;

	cout<<"thread#: "<<q->ThNumber<<"/"<<q->RowIndx<<"/"<<q->rowPerTh<<endl;
	
	for(yy = q->RowIndx; yy<(q->RowIndx+q->rowPerTh); yy++)
	{
        for(xx = 0; xx< max_width; xx++)
        {
			px = xx; 
			py = yy; 

			if ((image.at<Vec3b>(Point(px,py))[0]) == 255 )  
			{
                if(result.at<Vec3b>(py,px)==Vec3b(255,255,255))
                {
                    pthread_mutex_lock(&cs);
					gota_aceite(px, py,q->r,q->g,q->b);
					pthread_mutex_unlock(&cs);  		
                }
			}

        }
	}
}


void pintar(Mat &region, int x, int y,int r, int g, int b)
{
	region.at<Vec3b>(y,x) = Vec3b(r,g,b); 
}
 
void gota_aceite(int x, int y,int r, int g, int b)//x,y
{
    Point aux_p; //Para checar vecinos 
	int pxx, pyy; // x y para vecinos
	p.x = x; 
	p.y = y; 
	qpoints.push(p); 
	pintar(result, p.x, p.y,r,g,b);
 
	while (!qpoints.empty()) 
	{
        p = qpoints.front();
 
		for (int i = 0; i<2; i++)
		{
			if (i==2){ pxx=p.x;     pyy=p.y+1;} //arriba
			if (i==0){ pxx=p.x+1;   pyy=p.y;}   //der
			if (i==1){ pxx=p.x-1;   pyy=p.y;}   //izq
			if (i==3){ pxx=p.x;     pyy=p.y-1;} //abajo

			if (image.at<Vec3b>(pyy,pxx) == Vec3b(255,255,255))  //objeto?
			{ 
		        if (result.at<Vec3b>(pyy,pxx)==Vec3b(255,255,255)) //pintado?
		        {
					aux_p.x = pxx; 
					aux_p.y = pyy;
					qpoints.push(aux_p); //guarda en fila
					pintar(result, aux_p.x, aux_p.y,r,g,b);
		        }
			}
		}
		qpoints.pop();
	}
}

int main( int argc, char** argv )
{
	int i;
	int r,g,b;
	pthread_t th[numThreads];

	if( argc != 2)
	{
	cout <<" Usage: display_image image.png" << endl;
	return -1;
	}

	image = imread(argv[1], CV_LOAD_IMAGE_COLOR);   // Read the file

	if(! image.data )                              // Check for invalid input
	{
	cout <<  "Could not open or find the image" << std::endl ;
	return -1;
	}

	imshow( "Imagen", image );  

	max_width = image.cols;
    max_height = image.rows;

	Mat region(image.rows,image.cols,CV_8UC3,Scalar(255,255,255));
	result = region;
//-------------
	cout<<"Rows: "<<max_height<<" Cols: "<<max_width<<endl;
	start=time(NULL);
	pthread_mutex_init(&cs,NULL);

 	for(i=0;i<numThreads;i++){
 		b= theRNG().uniform(0,255)*i;
		g= theRNG().uniform(0,255)*i;
		r= theRNG().uniform(0,255)*i;
		thPar[i].r=r;
		thPar[i].g=g;
		thPar[i].b=b;
 		thPar[i].ThNumber = i;
 		thPar[i].RowIndx= i * (max_height/numThreads);

 		if(i == numThreads -1)
 			thPar[i].rowPerTh= (max_height/numThreads)+(max_height%numThreads);
 		else
 			thPar[i].rowPerTh= max_height/numThreads;

 		pthread_create(&th[i],NULL,&thRtn,&thPar[i]);
 	}

 	for(i=0;i<numThreads;i++){
 		pthread_join(th[i],NULL);
 	}
//------------
    namedWindow("Segmented Image", CV_WINDOW_AUTOSIZE);
	imshow("Segmented Image", result);
	pthread_mutex_destroy(&cs);
	end=time(NULL);

	cout <<"Clock cycles: " << " ("<<end-start<<")"<<endl; 
	waitKey();
}
