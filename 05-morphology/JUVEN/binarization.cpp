/*
 * binarization.cpp
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



void process(uchar thmin, uchar thmax, const char * const ims, const char * const imd)
{
	Mat m = loadGrayPicture(ims);
	Mat output(m.rows, m.cols, CV_8UC1);


	for (int i = 0 ; i < m.rows ; ++i){
		for (int j = 0 ; j < m.cols ; ++j){

			uchar pixelVal = m.at<uchar>(i,j);

			if (pixelVal >= thmin && pixelVal <= thmax){
				output.at<uchar>(i,j) = 255;
			} else {
				output.at<uchar>(i,j) = 0;
			}
		}
	}

	imwrite(imd, output);
}

int main(int argc, char * argv[])
{

	if (argc != 5){
		cerr << "Usage : " << argv[0] << " thmin thmax ims imd\n";
		exit(EXIT_FAILURE);
	}

	const uchar thmin = (uchar) atoi(argv[1]);
	const uchar thmax = (uchar) atoi(argv[2]);

	process(thmin, thmax, argv[3], argv[4]);

	return 0;
}
