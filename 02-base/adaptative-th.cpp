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



double average(const Mat & m)
{
	double sum = 0.;
	double nPixels = m.cols * m.rows;

	if (nPixels == 0){
		return 0.;
	}

	for (int i = 0 ; i < m.rows ; ++i){
		for (int j = 0 ; j < m.cols ; ++j){
			sum += (double) m.at<uchar>(i,j);
		}
	}

	return sum / nPixels;
}



void process(const char * const ims, int radius, double constant)
{
	Mat m = loadGrayPicture(ims);

	Mat mean_adapt;
	adaptiveThreshold(m, mean_adapt, 255., ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 2*radius+1, constant);
	imwrite("th-ocv-mean.png", mean_adapt);

	Mat gauss_adapt;
	adaptiveThreshold(m, gauss_adapt, 255., ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 2*radius+1, constant);
	imwrite("th-ocv-gauss.png", gauss_adapt);



	Mat output = loadGrayPicture(ims);

	for (int i = 0 ; i < m.rows ; ++i){
		for (int j = 0 ; j < m.cols ; ++j){

			double value = (double) m.at<uchar>(i,j);
			double avg = average(neighborhood(m, j, i, radius));

			if (value >= avg - constant){
				output.at<uchar>(i,j) = 255;
			} else {
				output.at<uchar>(i,j) = 0;
			}
		}
	}

	imwrite("th.png", output);
	imwrite("diff.png", output - mean_adapt);


}


int main(int argc, char * argv[])
{

	if (argc != 4){
		cerr << "Usage : " << argv[0] << "ims radius constant\n";
		exit(EXIT_FAILURE);
	}

	const char * const ims = argv[1];
	int radius = atoi(argv[2]);
	double constant = atof(argv[3]);

	process(ims, radius, constant);

	return 0;
}



