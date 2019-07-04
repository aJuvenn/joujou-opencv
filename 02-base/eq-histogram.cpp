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





void rescalePixels(Mat & m, unsigned cumulated_histogram[256])
{
	double factor = 255. / ((double) m.rows * m.cols);

	for (int i = 0 ; i < m.rows ; ++i){
		for (int j = 0 ; j < m.cols ; ++j){
			uchar I = m.at<uchar>(i,j);
			uchar newI = (uchar) (factor * ((double) cumulated_histogram[I]));
			m.at<uchar>(i,j) = newI;
		}
	}
}


void process(const char * const ims)
{
	Mat m = loadGrayPicture(ims);
	Mat eq_m;

	equalizeHist(m, eq_m);
	imwrite("eq-ocv.png", eq_m);


	Mat m2 = loadGrayPicture(ims);

	unsigned histogram[256];
	unsigned cumulated_histogram[256];

	fillHistogram(histogram, m2);
	fillCumulatedHistogram(histogram, cumulated_histogram);
	rescalePixels(m2, cumulated_histogram);

	imwrite("eq.png", m2);

	Mat diff = m2 - eq_m;
	imwrite("diff.png", diff);

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



