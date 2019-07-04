/*
 * bilateral.cpp
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





Rect neighborhoodBounds(const Mat & m, int j, int i, int radius)
{
	int j_left = max(0, j - radius);
	int i_left = max(0, i - radius);

	int rectWidth = min(2 * radius + 1, m.cols - j_left);
	int rectHeight = min(2 * radius + 1, m.rows - i_left);

	return Rect(j_left, i_left, rectWidth, rectHeight);
}






int getNeighborhoodRadius(double sigma_s, double precision)
{
	return (int) (ceil(sqrt(-2. * sigma_s * sigma_s * log(precision)))/2.);
}





uchar doubleToUchar(double value)
{
	if (value <= 0.){
		return 0;
	} else if (value >= 255.){
		return 255;
	} else {
		return(uchar) value;
	}
}



const double * gaussianArray(int nValues, double sigma)
{
	double * output = new double [nValues];

	for (int k = 0 ; k < nValues ; ++k){
		output[k] = exp(-((double) (k*k))/(2 * sigma * sigma));
	}

	return output;
}






uchar bilateralValue(const Mat & m, int centerX, int centerY, int radius, const double gsComputedValues[], const double gcComputedValues[])
{
	double coeffSum = 0.;
	double valueToNormalize = 0.;
	Rect neighBounds = neighborhoodBounds(m, centerX, centerY, radius);

	for (int qY = neighBounds.y ; qY < neighBounds.y + neighBounds.height ; ++qY){
		for (int qX = neighBounds.x ; qX < neighBounds.x + neighBounds.width ; ++qX){

			int spatialDist = abs(qX - centerX) + abs(qY - centerY);
			int colorDist = abs(((int) m.at<uchar>(centerY, centerX)) - ((int) m.at<uchar>(qY, qX)));

			double coeffToNormalize = gsComputedValues[spatialDist] * gcComputedValues[colorDist];
			valueToNormalize +=  coeffToNormalize * ((double) m.at<uchar>(qY,qX));
			coeffSum += coeffToNormalize;
		}
	}

	return doubleToUchar(valueToNormalize / coeffSum);
}


#define PRECISION 0.1

void process(double sigma_s, double sigma_g, const char * const ims, const char * const imd)
{
	Mat m = loadGrayPicture(ims);

	Mat output;
	m.copyTo(output);

	int radius = getNeighborhoodRadius(sigma_s, PRECISION);

	const double * const gsComputedValues = gaussianArray(2 * radius + 1, sigma_s);
	const double * const gcComputedValues = gaussianArray(256, sigma_g);

	for (int i = 0 ; i < m.rows ; ++i){
		for (int j = 0 ; j < m.cols ; ++j){
			output.at<uchar>(i,j) = bilateralValue(m, j, i, radius, gsComputedValues, gcComputedValues);
		}
	}

	delete [] gsComputedValues;
	delete [] gcComputedValues;



	imshow("input", m);
	imshow("output", output);

	imwrite(imd, output);

	Mat ocvOutput;
	bilateralFilter(m, ocvOutput, 2 * radius, sigma_g, sigma_s);
	imshow("ocvOutput", ocvOutput);

	imshow("diff", ocvOutput - output);


	waitKey(0);
}



int main(int argc, char * argv[])
{

	if (argc != 5){
		cerr << "Usage : " << argv[0] << " sigma_s sigma_g ims imd\n";
		exit(EXIT_FAILURE);
	}

	double sigma_s = atof(argv[1]);
	double sigma_g = atof(argv[2]);
	const char * const ims = argv[3];
	const char * const imd = argv[4];

	process(sigma_s, sigma_g, ims, imd);

	return 0;
}




