/*
 * median-filter.cpp
 *
 *  Created on: 24 oct. 2018
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


Mat neighborhood(Mat &m, int j, int i, int radius)
{
	int j_left = max(0, j - radius);
	int i_left = max(0, i - radius);

	int rectWidth = min(2 * radius + 1, m.cols - j_left);
	int rectHeight = min(2 * radius + 1, m.rows - i_left);

	return Mat(m, Rect(j_left, i_left, rectWidth, rectHeight));
}


uchar median(const Mat & m)
{
	int nPixels = m.cols * m.rows;

	if (nPixels == 0){
		return 0.;
	}

	uchar values[nPixels];

	for (int i = 0 ; i < m.rows ; ++i){
		for (int j = 0 ; j < m.cols ; ++j){
			values[j + m.cols * i] = m.at<uchar>(i,j);
		}
	}

	sort(values, values + nPixels);


	return values[nPixels/2];
}



void process(int r, const char * const ims, const char * const imd)
{
	Mat m = loadGrayPicture(ims);

	Mat output;
	m.copyTo(output);

	for (int i = 0 ; i < m.rows ; ++i){
		for (int j = 0 ; j < m.cols ; ++j){
			output.at<uchar>(i,j) = median(neighborhood(m, j, i, r));
		}
	}

	imshow("manual median", output);
	imwrite(imd, output);

	Mat ocvOutput;
	medianBlur(m, ocvOutput, 2*r+1);
	imshow("ocv medianBlur", ocvOutput);

	imshow("diff", output - ocvOutput);

	waitKey(0);
}


int main(int argc, char * argv[])
{

	if (argc != 4){
		cerr << "Usage : " << argv[0] << " r ims imd\n";
		exit(EXIT_FAILURE);
	}

	int r = atoi(argv[1]);
	const char * const ims = argv[2];
	const char * const imd = argv[3];

	process(r, ims, imd);

	return 0;
}



