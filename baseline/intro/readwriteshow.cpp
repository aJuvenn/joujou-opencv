#include <iostream>
#include <cstdlib>

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;


#define OUTPUT_WIDTH 200
#define OUTPUT_HEIGHT 100


static const char * name_from_path(const char * const path)
{
	const char * output = path;
	const char * cursor;

	for (cursor = path ; *cursor != 0 ; cursor++){

		if (*cursor == '/' && *(cursor + 1) != 0){
			output = cursor + 1;
		}
	}

	return output;
}


void process(const char* imsname, const char* imdname)
{

	Mat imsmat = imread(imsname, CV_LOAD_IMAGE_COLOR);

	if (imsmat.data == NULL){
		std::cerr << "Invalid file path : " << imsname << '\n';
		exit(EXIT_FAILURE);
	}

	imshow(name_from_path(imsname), imsmat);


	cout << "Width : " << imsmat.cols << '\n';
	cout << "Height : " << imsmat.rows << '\n';

	static const Scalar colorImage = Scalar(255,0,255);
	Mat imdmat = Mat(OUTPUT_HEIGHT, OUTPUT_WIDTH, CV_8UC3, colorImage);
	imshow(name_from_path(imdname), imdmat);
	imwrite(imdname, imdmat);

	waitKey(0);
}


void usage(const char *s)
{
	std::cerr<<"Usage: "<<s<<" imsname imdname\n"<<std::endl;
	exit(EXIT_FAILURE);
}


int main(int argc, char* argv[])
{
	if (argc != 3){
		usage(argv[0]);
	}

	const char * const imsname = argv[1];
	const char * const imdname = argv[2];

	process(imsname, imdname);

	return EXIT_SUCCESS;
}


