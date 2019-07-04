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





void fillHistogram(unsigned histogram[256], Mat & m)
{
	memset(histogram, 0, 256 * sizeof(unsigned));

	for (int i = 0 ; i < m.rows ; ++i){
		for (int j = 0 ; j < m.cols ; ++j){

			uchar I = m.at<uchar>(i,j);
			histogram[I]++;
		}
	}
}


void fillCumulatedHistogram(unsigned histogram[256], unsigned cumulated_histogram[256])
{
	for (unsigned k = 0 ; k < 256 ; ++k){
		cumulated_histogram[k] = histogram[k] + ((k > 0) ? cumulated_histogram[k-1] : 0);
	}
}



double computeAvg1(unsigned histogram[256], int t)
{
	unsigned x = 0, y = 0;

	for (int i = 0 ; i <= t ; ++i){
		x += i * (int) histogram[i];
		y += histogram[i];
	}

	if (y == 0){
		return 0.;
	}

	return (double) x / (double) y;
}


double computeAvg2(unsigned histogram[256], int t)
{
	unsigned x = 0, y = 0;

	for (int i = t+1 ; i < 256 ; ++i){
		x += i * (int) histogram[i];
		y += histogram[i];
	}

	if (y == 0){
		return 0.;
	}

	return (double) x / (double) y;
}


int computeCeil(Mat & m)
{
	unsigned histogram[256];
	unsigned cumulatedHistogram[256];
	fillHistogram(histogram, m);
	fillCumulatedHistogram(histogram, cumulatedHistogram);


	double n_element = (double) (m.cols * m.rows);

	double v_max;
	int arg_max;

	for (int t = 0 ; t < 256 ; ++t){

		double avg1 = computeAvg1(histogram, t);
		double avg2 = computeAvg2(histogram, t);
		double p1 = ((double) cumulatedHistogram[t]) / n_element;
		double v = p1 * (1. - p1) * (avg1 - avg2)*(avg1 - avg2);

		if (t == 0 || v > v_max){
			v_max = v;
			arg_max = t;
		}
	}

	return arg_max;
}



void process(const char * const ims)
{
	Mat m = loadGrayPicture(ims);

	Mat thres_m;
	double found_thred = threshold(m, thres_m, 0, 255, THRESH_BINARY + THRESH_OTSU);
	imshow("otsu-th-ocv.png", thres_m);
	imwrite("otsu-th-ocv.png", thres_m);



	double ceil = computeCeil(m);

	for (int i = 0 ; i < m.rows ; ++i){
		for (int j = 0 ; j < m.cols ; ++j){

			uchar & value = m.at<uchar>(i,j);

			if (value > ceil){
				value = 255;
			} else {
				value = 0;
			}
		}
	}

	imshow("otsu-th.png", m);;
	imwrite("otsu-th.png", m);


	Mat diff = thres_m - m;
	imshow("diff", diff);

	cout << "Manual found threshold: " << ceil << '\n';
	cout << "OpenCV found threshold: " << found_thred << '\n';

	waitKey(0);
}


int main(int argc, char * argv[])
{

	if (argc != 2){
		cerr << "Usage : " << argv[0] << "ims\n";
		exit(EXIT_FAILURE);
	}

	const char * const ims = argv[1];

	process(ims);

	return 0;
}



