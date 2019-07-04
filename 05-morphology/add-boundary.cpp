/*
 * add-boundary.cpp
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




Mat loadPicture(const char * const path)
{
	Mat output = imread(path, CV_LOAD_IMAGE_COLOR);

	if (output.data == NULL){
		std::cerr << "Invalid picture path : " << path << '\n';
		exit(EXIT_FAILURE);
	}

	return output;
}



Mat loadGrayPicture(const char * const path)
{
	Mat output = imread(path, CV_LOAD_IMAGE_GRAYSCALE);

	if (output.data == NULL){
		std::cerr << "Invalid picture path : " << path << '\n';
		exit(EXIT_FAILURE);
	}

	return output;
}



void process(const char * const ims, const char * const boundPath, const char * const imd)
{
	Mat m = loadPicture(ims);
	Mat bounds = loadGrayPicture(boundPath);

	for (int i = 0 ; i < m.rows ; ++i){
		for (int j = 0 ; j < m.cols ; ++j){

			if (bounds.at<uchar>(i,j)){
				m.at<Vec3b>(i,j) = Vec3b(0,0,255);
			}
		}
	}

	imwrite(imd, m);
}



int main(int argc, char * argv[])
{

	if (argc != 4){
		cerr << "Usage : " << argv[0] << " ims bounds imd\n";
		exit(EXIT_FAILURE);
	}

	process(argv[1], argv[2], argv[3]);

	return 0;
}
