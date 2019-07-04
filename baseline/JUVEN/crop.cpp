#include <iostream>
#include <cstdlib>
#include <math.h>

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;









void check_bounds(int im_width, int im_height, int max_width, int max_height)
{
	if (im_width >= max_width && im_height >= max_height){
		return;
	}

	std::cerr << "Invalid bounds" << '\n';
	exit(EXIT_FAILURE);
}


void process(const char* imsname,
		int i0,
		int j0,
		int w,
		int h)
{
	Mat im = imread(imsname, CV_LOAD_IMAGE_COLOR);

	if (im.data == NULL){
		std::cerr << "Invalid file path" << '\n';
		exit(EXIT_FAILURE);
	}

	check_bounds(im.cols, im.rows, j0 + w, i0 + h);


	Mat manual_output(h, w, CV_8UC3);
	int i, j;

	for (i = 0 ; i < h ; ++i){
		for (j = 0 ; j < w ; ++j){
			manual_output.at<Vec3b>(i, j) = im.at<Vec3b>(i0 + i, j0 + j);
		}
	}


	imwrite("crop.png", manual_output);
	Mat rectangle_output(im, Rect(j0, i0, w, h));
	imwrite("crop-ocv.png", rectangle_output);


	imshow("image difference", rectangle_output - manual_output);

	for (i = 0 ; i < h ; ++i){
		for (j = 0 ; j < w ; ++j){

			if (rectangle_output.at<Vec3b>(i, j) != manual_output.at<Vec3b>(i0 + i, j0 + j)){
				std::cerr << "Pixel and rectangle crop images are not equal" << '\n';
				exit(EXIT_FAILURE);
			}
		}
	}

	waitKey(0);
}


void usage(const char *s)
{
	std::cerr<<"Usage: "<<s<<" imsname i0 j0 w h\n"<<std::endl;
	exit(EXIT_FAILURE);
}


#define param 5

int main(int argc, char* argv[])
{
	if (argc != (param+1)){
		usage(argv[0]);
	}

	const char * imsname = argv[1];
	int i0 = atoi(argv[2]);
	int j0 = atoi(argv[3]);
	int w = atoi(argv[4]);
	int h = atoi(argv[5]);

	process(imsname, i0, j0, w, h);

	return EXIT_SUCCESS;
}
