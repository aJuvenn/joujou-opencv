/*
 * make-se.cpp
 *
 *  Created on: 19 nov. 2018
 *      Author: ajuven
 */



#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <math.h>
#include <opencv2/opencv.hpp>



using namespace cv;
using namespace std;


enum ShapeType {
	SQUARE = 0,
	DIAMOND,
	DISK,
	LINE_V,
	DIAG_R,
	LINE_H,
	DIAG_L,
	CROSS,
	PLUS
};



int norm1(int x, int y)
{
	return abs(x) + abs(y);
}

int norm2_squared(int x, int y)
{
	return x * x + y * y;
}


typedef int pixelConditionFunc(int x, int y, int halfsize);


int SQUARECond(int x, int y, int halfsize)
{
	(void) x;
	(void) y;
	(void) halfsize;

	return 1;
}

int DIAMONDCond(int x, int y, int halfsize)
{
	return norm1(x - halfsize, y - halfsize) <= halfsize;
}

int DISKCond(int x, int y, int halfsize)
{
	return norm2_squared(x - halfsize, y - halfsize) <= halfsize * halfsize;
}

int LINE_VCond(int x, int y, int halfsize)
{
	(void) y;
	return x == halfsize;
}

int DIAG_RCond(int x, int y, int halfsize)
{
	return x + y == 2 * halfsize;
}

int LINE_HCond(int x, int y, int halfsize)
{
	(void) x;
	return y == halfsize;
}

int DIAG_LCond(int x, int y, int halfsize)
{
	(void) halfsize;
	return x == y;
}


int CROSSCond(int x, int y, int halfsize)
{
	return DIAG_RCond(x, y, halfsize) || DIAG_LCond(x, y, halfsize);
}


int PLUSCond(int x, int y, int halfsize)
{
	return LINE_VCond(x, y, halfsize) || LINE_HCond(x, y, halfsize);
}




void fillPixelsWithCond(Mat & shape, int halfSize, pixelConditionFunc f)
{
	int i, j;

	for (i = 0 ; i < shape.rows ; ++i){
		for (j = 0 ; j < shape.cols ; ++j){
			if (f(j, i, halfSize)){
				shape.at<uchar>(i,j) = 255;
			}
		}
	}
}


#define FILL_IF_MATCH(shape, halfsize, shapeType)\
	case shapeType:\
		fillPixelsWithCond(shape, halfsize, shapeType##Cond);\
		break;\

void process(const enum ShapeType shapeType, const int halfsize, const char * const seName)
{
	const int edgeSize = 2 * halfsize + 1;
	Mat shape(edgeSize, edgeSize, CV_8UC1, Scalar(0));

	switch (shapeType){

		FILL_IF_MATCH(shape, halfsize, SQUARE)
		FILL_IF_MATCH(shape, halfsize, DIAMOND)
		FILL_IF_MATCH(shape, halfsize, DISK)
		FILL_IF_MATCH(shape, halfsize, LINE_V)
		FILL_IF_MATCH(shape, halfsize, DIAG_R)
		FILL_IF_MATCH(shape, halfsize, LINE_H)
		FILL_IF_MATCH(shape, halfsize, DIAG_L)
		FILL_IF_MATCH(shape, halfsize, CROSS)
		FILL_IF_MATCH(shape, halfsize, PLUS)

		default:
			cerr << "Invalid shape type\n";
			exit(EXIT_FAILURE);
			break;
	}


	imwrite(seName, shape);
}



int main(int argc, char * argv[])
{

	if (argc != 4){
		cerr << "Usage : " << argv[0] << " shape halfsize se-name\n";
		exit(EXIT_FAILURE);
	}

	const enum ShapeType shapeType = (enum ShapeType) atoi(argv[1]);
	const int halfsize = atoi(argv[2]);
	const char * const seName = argv[3];

	process(shapeType, halfsize, seName);

	return 0;
}
