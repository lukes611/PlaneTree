#include "opencv2\core\core.hpp"
#include "opencv2/flann/miniflann.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/photo/photo.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/ml/ml.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/core/core_c.h"
#include "opencv2/highgui/highgui_c.h"
#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2\features2d\features2d.hpp"
#include "opencv2\nonfree\features2d.hpp"
#include "opencv2\legacy\legacy.hpp"
#include <stdio.h>
#include <stdlib.h>
//#include <mrpt
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <stack>
#include <queue>
#include <deque>
#include <complex>
#include <list>

using namespace std;
using namespace cv;

namespace LDR
{

//pgm reader:
class PGM_reader
{
protected:
	int * im_data;
	bool heap_used;
	int width, height, max_pixel_val;
	string file_name;
	
	void basic_init();
	void basic_de_init();
	void error(string st);
	void make_sure(bool check, string st);
	void setup();
	void swap_short_byte(unsigned short & s);
	void setup_pixels_(FILE * fi);
	bool isNum(unsigned char c);
	int getInt(FILE * fi);

public:
	PGM_reader ();
	PGM_reader(string fname);
	~PGM_reader();
	void open(string fname);
	Mat getUCIm();
	Mat getFIm();
	Size size();
};


//ppm reader


class PPM_reader : public PGM_reader
{
	void setup();
	void setup_pixels_(FILE * fi);
	Vec3i bgr(Vec3i & inp);
public:
	PPM_reader ();
	PPM_reader(string fname);
	~PPM_reader();
	Vec3i pixel(int x, int y);
	Mat getUCIm();
	Mat getFIm();
};


//yuv reader:
class YUV_reader
{
	FILE * file_;
	int width, height;
	string file_name;
	Mat frame;
	void init();
	void open_file();
	void close_file();
	void clamp(double & b, double a, double c);
	Vec3b yuv2rgb(Vec3b inp);
	Mat read_frame(bool & canGet);

public:
	YUV_reader(string fname, int w, int h);
	YUV_reader & operator=(const YUV_reader & inp);
	~YUV_reader();
	bool get(Mat & m);
};


}//end of namespace