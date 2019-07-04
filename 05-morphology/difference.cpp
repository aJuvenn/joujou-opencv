/*
 * difference.cpp
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



Mat loadGrayPicture(const char * const path)
{
	Mat output = imread(path, CV_LOAD_IMAGE_GRAYSCALE);

	if (output.data == NULL){
		std::cerr << "Invalid picture path : " << path << '\n';
		exit(EXIT_FAILURE);
	}

	return output;
}



void process(const char * const ims1, const char * const ims2, const char * const imd)
{
	Mat m1 = loadGrayPicture(ims1);
	Mat m2 = loadGrayPicture(ims2);
	Mat output =  m2 - m1;

	imwrite(imd, output);
}

int main(int argc, char * argv[])
{

	if (argc != 4){
		cerr << "Usage : " << argv[0] << "ims1 ims2 imd\n";
		exit(EXIT_FAILURE);
	}

	process(argv[1], argv[2], argv[3]);

	return 0;
}


