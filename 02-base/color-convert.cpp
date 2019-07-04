#include <iostream>
#include <cstdlib>
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





void process(const char * const path)
{
	Mat m = loadPicture(path);

	/*
	 * RGB decomposition
	 */

	Mat bgr[3];
	split(m, bgr);
	imshow("b", bgr[0]);
	imshow("g", bgr[1]);
	imshow("r", bgr[2]);

	/*
	 * RGB to Gray
	 */

	Mat gray_m;
	cvtColor(m, gray_m, COLOR_BGR2GRAY);
	imshow("RGBtoGray", gray_m);


	/*
	 * RGB to YCrCb
	 */

	Mat YCbCr_m;
	Mat YCrCb[3];
	cvtColor(m, YCbCr_m, COLOR_BGR2YCrCb);
	split(YCbCr_m, YCrCb);
	imshow("y", YCrCb[0]);
	imshow("cr", YCrCb[1]);
	imshow("cb", YCrCb[2]);

	Mat RGB_m;
	cvtColor(YCbCr_m, RGB_m, COLOR_YCrCb2BGR);
	imshow("RGB->YCbCr->RGB", RGB_m);





	waitKey(0);
}


int main(int argc, char * argv[])
{
	if (argc != 2){
		std::cerr << "Usage: " << argv[0] << " ims\n" << std::endl;
		exit(EXIT_FAILURE);
	}

	process(argv[1]);

	return EXIT_SUCCESS;
}


