/*
 * morphology.cpp
 *
 *  Created on: 19 nov. 2018
 *      Author: ajuven
 */


#include "morphology.hpp"





uchar applyComparison(int x, int y, Mat & se, Mat & ims, void (*pf)(uchar, uchar*))
{
	uchar output = 0;
	int halfSize = se.cols / 2;
	int x_offset, y_offset;
	int firstValueIsEncountered = 0;

	for (y_offset = -halfSize ; y_offset <= halfSize ; ++y_offset){

		int y_ims = y + y_offset;

		if (y_ims < 0 || y_ims >= ims.rows){
			continue;
		}

		for (x_offset = -halfSize ; x_offset <= halfSize ; ++x_offset){

			int x_ims = x + x_offset;

			if (x_ims < 0 || x_ims >= ims.cols){
				continue;
			}

			if (!se.at<uchar>(y_offset + halfSize, x_offset + halfSize)){
				continue;
			}

			uchar pixelValue = ims.at<uchar>(y_ims, x_ims);

			if (firstValueIsEncountered){
				pf(pixelValue, &output);
			} else {
				output = pixelValue;
				firstValueIsEncountered = 1;
			}
		}

	}

	return output;
}





/**
 * @brief  compute a morphological dilation or erosion on a grayscale image
 *         with a given structuring element. Dilation or erosion
 *         processing depends on an order function  defined by the pointer pf
 * @param  se: the structing element shape umber
 * @param  ims: the input image source to process
 * @param  imd: the destination image
 * @param  pf: a pointer on a ordering function
 */
void mm(Mat se, Mat ims, Mat imd, void (*pf)(uchar, uchar*))
{
	int x, y;


	for (y = 0 ; y < imd.rows ; ++y){
		for (x = 0 ; x < imd.cols ; ++x){
			imd.at<uchar>(y,x) = applyComparison(x, y, se, ims, pf);
		}
	}

}

/**
 * @brief  ordering function, if val is geater than max then update max
 * @param  val: the input value
 * @param  max: the value to update
 */
void maximum(uchar val, uchar* max)
{
	if (val > *max){
		*max = val;
	}
}

/**
 * @brief  ordering function, if val is lower than min then update min
 * @param  val: the input value
 * @param  min: the value to update
 */
void minimum(uchar val, uchar* min)
{
	if (val < *min){
		*min = val;
	}
}

