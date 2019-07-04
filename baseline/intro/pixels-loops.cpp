#include <iostream>
#include <cstdlib>
#include <cstdio>

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;


void process(const char* imsname)
{
	Mat imsmat = imread(imsname, CV_LOAD_IMAGE_COLOR);

	if (imsmat.data == NULL){
		std::cerr << "Invalid file path : " << imsname << '\n';
		exit(EXIT_FAILURE);
	}

	(void) imsname;
}


void usage(const char *s)
{
	fprintf(stderr, "Usage: %s imsname\n", s);
	exit(EXIT_FAILURE);
}


int main(int argc, char* argv[])
{
	if(argc != 2){
		usage(argv[0]);
	}

	process(argv[1]);
	return EXIT_SUCCESS;
}
