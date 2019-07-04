/*
 * dilatation.cpp
 *
 *  Created on: 19 nov. 2018
 *      Author: ajuven
 */



#include "morphology.hpp"


Mat loadGrayPicture(const char * const path)
{
	Mat output = imread(path, CV_LOAD_IMAGE_GRAYSCALE);

	if (output.data == NULL){
		std::cerr << "Invalid picture path : " << path << '\n';
		exit(EXIT_FAILURE);
	}

	return output;
}


void process(const char * const seName, const char * const imsName, const char * const imdName)
{
	Mat shape = loadGrayPicture(seName);
	Mat ims = loadGrayPicture(imsName);
	Mat tmp(ims.rows, ims.cols, CV_8UC1);
	Mat output(ims.rows, ims.cols, CV_8UC1);

	mm(shape, ims, tmp, maximum);
	mm(shape, tmp, output, minimum);

	imwrite(imdName, output);
}



int main(int argc, char * argv[])
{

	if (argc != 4){
		cerr << "Usage : " << argv[0] << " se-name ims-name imd-name\n";
		exit(EXIT_FAILURE);
	}

	process(argv[1], argv[2], argv[3]);

	return 0;
}
