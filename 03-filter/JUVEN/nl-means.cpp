

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




static inline double gaussian(double x, double sigma)
{
	return exp(-(x*x)/(2 * sigma * sigma));
}


Rect intersectionPatch(int centerX, int centerY, const Rect & centerNeighb, int qX, int qY, const Rect & qNeighb)
{

	int i_left = max(centerNeighb.y - centerY, qNeighb.y - qY);
	int j_left = max(centerNeighb.x - centerX, qNeighb.x - qX);

	int centerNeighRightCornerX = centerNeighb.x + centerNeighb.width - 1;
	int centerNeighRightCornerY = centerNeighb.y + centerNeighb.height - 1;

	int qNeighRightCornerX = qNeighb.x + qNeighb.width - 1;
	int qNeighRightCornerY = qNeighb.y + qNeighb.height - 1;

	int i_right = min(centerNeighRightCornerY - centerY, qNeighRightCornerY - qY);
	int j_right = min(centerNeighRightCornerX - centerX, qNeighRightCornerX - qX);

	int rectWidth = j_right - j_left + 1;
	int rectHeight = i_right - i_left + 1;

	return Rect(j_left, i_left, rectWidth, rectHeight);
}


double patchDistance(const Mat & m, int patchRadius, int centerX, int centerY, int qX, int qY)
{
	const Rect centerNeighb = neighborhoodBounds(m, centerX, centerY, patchRadius);
	const Rect qNeighb = neighborhoodBounds(m, qX, qY, patchRadius);
	const Rect patch = intersectionPatch(centerX, centerY, centerNeighb, qX, qY, qNeighb);
	int nElem = patch.height * patch.width;


	if (nElem == 0){
		return 0.;
	}

	const int centerYOffset = centerY + patch.y;
	const int centerXOffset = centerX + patch.x;
	const int qYOffset = qY + patch.y;
	const int qXOffset = qX + patch.x;

	double sum = 0.;

	for (int i = 0 ; i < patch.height ; ++i){
		for (int j = 0 ; j < patch.width ; ++j){

			const double centerPatchVal = (double) m.at<uchar>(centerYOffset + i, centerXOffset + j);
			const double qPatchVal = (double) m.at<uchar>(qYOffset + i, qXOffset + j);
			const double normalizedDiff = (centerPatchVal - qPatchVal) / 256.;

			sum += normalizedDiff * normalizedDiff;
		}
	}

	return sum / ((double) nElem);
}



uchar patchMeanValue(const Mat & m, int patchRadius, int windowRadius, double sigma, int centerX, int centerY)
{
	double coeffSum = 0.;
	double valueToNormalize = 0.;

	const Rect window = neighborhoodBounds(m, centerX, centerY, windowRadius);
	const int qYMax = window.y + window.height ;
	const int qXMax = window.x + window.width;

	for (int qY = window.y ; qY < qYMax ; ++qY){
		for (int qX = window.x ; qX < qXMax ; ++qX){

			double dist = patchDistance(m, patchRadius, centerX, centerY, qX, qY);
			double coeffToNormalize = gaussian(dist, sigma);

			valueToNormalize += coeffToNormalize * ((double) m.at<uchar>(qY,qX));// / 256.;
			coeffSum += coeffToNormalize;
		}
	}

	return doubleToUchar(valueToNormalize / coeffSum);
}





void process(double sigma, int patchRadius, int windowRadius, const char * const ims, const char * const imd)
{
	Mat m = loadGrayPicture(ims);

	Mat output;
	m.copyTo(output);


	for (int i = 0 ; i < m.rows ; ++i){
		for (int j = 0 ; j < m.cols ; ++j){
			output.at<uchar>(i,j) = patchMeanValue(m, patchRadius, windowRadius, sigma, j, i);
		}
	}

	imwrite(imd, output);

}


#define WINDOW_RADIUS 5
#define PATCH_RADIUS 3


int main(int argc, char * argv[])
{

	if (argc != 4){
		cerr << "Usage : " << argv[0] << " sigma ims imd\n";
		exit(EXIT_FAILURE);
	}

	double sigma = atof(argv[1]) / 1000.; // I cheat to have sigma in practical ranges
	const char * const ims = argv[2];
	const char * const imd = argv[3];

	process(sigma, PATCH_RADIUS, WINDOW_RADIUS, ims, imd);

	return 0;
}




