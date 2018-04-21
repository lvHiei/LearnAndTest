#include <opencv2/opencv.hpp>

#include <iostream>
#include <stdint.h>
#include <stdio.h>
using namespace cv;
using namespace std;

void printMat(Mat mat, int row, int col){
	printf("mat[%d][%d]=%d\n", row, col, mat.ptr(row)[col]);
	// cout << "mat[" << endl;
}

int main( int argc, char** argv )
{
 char* imageName = argv[1];
 Mat image;
 image = imread( imageName, IMREAD_COLOR );
 if( argc != 2 || !image.data )
 {
   printf( " No image data \n " );
   return -1;
 }
 // Mat gray_image;
 // cvtColor( image, gray_image, COLOR_BGR2GRAY );
 // imwrite( "./Gray_Image.jpg", gray_image );
 // namedWindow( imageName, WINDOW_AUTOSIZE );
 // namedWindow( "Gray image", WINDOW_AUTOSIZE );
 // imshow( imageName, image );
 // imshow( "Gray image", gray_image );

 int width = image.cols;
 int height = image.rows;

 cout << width << "  " << height  << "  " << image.isContinuous() << endl;

 // if(image.isContinuous()){
 // 	FILE* pFILE = fopen("data.bgr", "w");
 // 	// fwrite(image.ptr(0), 1, width*height*3, pFILE);
 // 	fwrite(image.data, 1, width*height*3, pFILE);
 // 	fclose(pFILE);
 // }

  
 Mat ref = image;
 ref.ptr(0)[0] = 0;
 ref.at<Vec3b>(0, 0) = Vec3b(1, 2, 3);

 printMat(image, 0, 0);

 Mat clone = image.clone();
 clone.ptr(0)[1] = 1;
 printMat(image, 0, 1);
 printMat(clone, 0, 1);

 waitKey(0);
 return 0;
}