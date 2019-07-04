#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <ctime>

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;




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



void print_video_details(VideoCapture & cap)
{
	double fps =  cap.get(CV_CAP_PROP_FPS);
	double width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	double height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
	double nframes = cap.get(CV_CAP_PROP_FRAME_COUNT);

	cout << "fps: " << fps << '\n';
	cout << "width: " << width << '\n';
	cout << "height: " << height << '\n';
	cout << "number of frames: " << nframes << '\n';

}

void process(const char* vidname)
{
	static const char * track_name = "Frame";
	VideoCapture cap(vidname);
	const char * vidname_nopath = name_from_path(vidname);

	if(!cap.isOpened()){
		cout << "Error opening video stream or file" << endl;
		exit(EXIT_FAILURE);
	}

	float fps =  cap.get(CV_CAP_PROP_FPS);
	int sleeping_time = (int) 1000./fps;

	print_video_details(cap);

	int current_frame = 0;
	int nframes = (int) cap.get(CV_CAP_PROP_FRAME_COUNT);

	namedWindow(vidname_nopath, WINDOW_AUTOSIZE);
	createTrackbar(track_name, vidname_nopath, &current_frame, nframes, NULL);

	while(1){

		Mat frame;
		cap >> frame;


		if (frame.empty()){
			break;
		}

		imshow(vidname_nopath, frame);
		char c = (char) waitKey(sleeping_time); // to be accurate we could take computing time into account

		if(c == 27){
			break;
		}

		int frame_number = getTrackbarPos(track_name, vidname_nopath);
		setTrackbarPos(track_name, vidname_nopath, frame_number+1);
		cap.set(CV_CAP_PROP_POS_FRAMES, frame_number);
	}

	cap.release();
	destroyAllWindows();

}



void usage (const char *s)
{
	fprintf(stderr, "Usage: %s imsname\n", s);
	exit(EXIT_FAILURE);
}


int main(int argc, char* argv[])
{
	if (argc != 2){
		usage(argv[0]);
	}

	process(argv[1]);

	return EXIT_SUCCESS;
}
