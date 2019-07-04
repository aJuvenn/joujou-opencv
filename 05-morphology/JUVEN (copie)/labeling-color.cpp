#include <iostream>
#include <cstdlib>

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int
_find(int p, int* roots)
{
	while(roots[p] != p)
		p = roots[p];
	return p;
}

int
_union(int r0, int r1, int* roots)
{
	if(r0 == r1) return r0;
	if(r0 == -1) return r1;
	if(r1 == -1) return r0;
	if(r0 <  r1){
		roots[r1] = r0;
		return r0;
	}else{
		roots[r0] = r1;
		return r1;
	}
}

int
_add(int p, int r, int* roots)
{
	if(r==-1)
		roots[p] = p;
	else
		roots[p] = r;
	return roots[p];
}

void 
process(const char* imsname, const char * regname, const char * colorName)
{
	Mat ims = imread(imsname);

	if(!ims.data){
		cerr<<"Image not found, exit"<<endl;
		exit(EXIT_FAILURE);
	}

	cvtColor(ims, ims, CV_BGR2GRAY);

	int* roots = new int[ims.total()];
	int  rows  = ims.rows;
	int  cols  = ims.cols;
	int p      = 0;
	int r      = -1;
	uchar* ps  = ims.data;

	for(int i=0; i<rows; i++){
		for(int j=0; j<cols; j++){
			r = -1;

			if( j>0 && (*(ps-1) == (*ps)) )
				r = _union( _find(p-1, roots), r, roots);

			if( (i>0 && j>0) && (*(ps-1-cols) == (*ps)) )
				r = _union( _find(p-1-cols, roots), r, roots);

			if( i>0 && (*(ps-cols) == (*ps)) )
				r = _union(_find(p-cols, roots), r, roots);

			if( (j<(cols-1) && i>0) && (*(ps+1-cols) == (*ps)) )
				r = _union(_find(p+1-cols, roots), r, roots);

			r = _add(p, r, roots);

			p++;
			ps++;
		}
	}

	for(p=0; p<rows*cols; p++){
		roots[p] = _find(p, roots);
	}

	int l=0;
	for(int i=0; i<rows; i++){
		for(int j=0; j<cols; j++){
			int p = i*cols+j;
			if(roots[p] == p)
				roots[p] = l++;
			else
				roots[p] = roots[roots[p]];
		}
	}

	cout<<"labeling: "<< l << " components detected"<<endl;




	Mat reg(rows, cols, CV_8UC1);

	for(int i=0; i<rows; i++){
		for(int j=0; j<cols; j++){
			reg.at<uchar>(i,j) = (uchar) roots[i*cols + j];
		}
	}

	imwrite(regname, reg);


	Mat colorMap(rows, cols, CV_8UC3);
	Vec3b randColors[l];

	randColors[0] = Vec3b(0,0,0);

	for (int i = 1 ; i < l ; i++){

		uchar randR = (uchar) (rand() % 256);
		uchar randG = (uchar) (rand() % 256);
		uchar randB = (uchar) (rand() % 256);

		randColors[i] = Vec3b(randR,randG,randB);
	}

	for(int i=0; i<rows; i++){
		for(int j=0; j<cols; j++){
			colorMap.at<Vec3b>(i,j) =  randColors[roots[i*cols + j]];
		}
	}

	imwrite(colorName, colorMap);


	delete [] roots;
}

void 
usage (const char *s)
{
	std::cerr<<"Usage: "<<s<<" ims reg-name color-name"<<std::endl;
	exit(EXIT_FAILURE);
}

#define param 3
int 
main( int argc, char* argv[] )
{
	if(argc != (param+1))
		usage(argv[0]);
	process(argv[1], argv[2], argv[3]);
	return EXIT_SUCCESS;
}

