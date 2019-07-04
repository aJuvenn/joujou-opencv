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


void process(const char * const ims, float h, float s, float v, const char * output_path)
{
	Mat m = loadPicture(ims);

	Mat hsv_m;
	cvtColor(m, hsv_m, COLOR_BGR2HSV);

	Mat hsv[3];
	split(hsv_m, hsv);

	imshow("h", hsv[0]);
	imshow("s", hsv[1]);
	imshow("v", hsv[2]);


	hsv[0] += h;
	hsv[1] += s;
	hsv[2] += v;

	Mat hsv_output;
	merge(hsv, 3, hsv_output);

	Mat output;
	cvtColor(hsv_output, output, COLOR_HSV2BGR);

	imwrite(output_path, output);
	waitKey(0);
}


int main(int argc, char * argv[])
{

	if (argc != 6){
		cerr << "Usage : " << argv[0] << " h s v ims imd\n";
		exit(EXIT_FAILURE);
	}


	float h = atof(argv[1]);
	float s = atof(argv[2]);
	float v = atof(argv[3]);
	const char * const ims = argv[4];
	const char * const imd = argv[5];


	process(ims, h, s, v, imd);

	return 0;
}



