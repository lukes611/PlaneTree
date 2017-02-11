#include "LDatasetReader.h"


//pgm reader:

namespace LDR
{

void PGM_reader::basic_init()
{
	im_data = NULL;
	heap_used = false;
	width = height = max_pixel_val = 0;
	file_name = "";
}
void PGM_reader::basic_de_init()
{
	if(heap_used)
	{
		heap_used = false;
		delete [] im_data;
	}
}

void PGM_reader::error(string st)
{
	cout << "error in pgm_reader: " << st << endl;
	system("pause");
	exit(5);
}

void PGM_reader::make_sure(bool check, string st)
{
	if(!check)
	{
		error(st);
	}
}

void PGM_reader::setup()
{
	FILE * fi = fopen(file_name.c_str(), "rb");

	char ps_[4];
	make_sure(fread_s(ps_, 4, 1, 2, fi) == 2, "could not read");
	ps_[2] = NULL;
	make_sure(string(ps_) == "P5", "not pgm image");

	width = getInt(fi);
	height = getInt(fi);
	max_pixel_val = getInt(fi);
	setup_pixels_(fi);
	fclose(fi);
}

void PGM_reader::swap_short_byte(unsigned short & s)
{
	unsigned char * pt = (unsigned char * ) &s;
	unsigned char tmp = pt[1];
	pt[1] = pt[0];
	pt[0] = tmp;
}

void PGM_reader::setup_pixels_(FILE * fi)
{
	im_data = new int[width * height];
	heap_used = true;
	if(max_pixel_val > 255)
	{
		for(int s = 0; s < width * height; s++)
		{
			unsigned short tmp = 0;
			fread_s(&tmp, sizeof(short), sizeof(short), 1, fi);
			swap_short_byte(tmp);
			im_data[s] = (int) tmp;
		}
	}else
	{
		for(int s = 0; s < width * height; s++)
		{
			unsigned char tmp = 0;
			fread_s(&tmp, sizeof(char), sizeof(char), 1, fi);
			im_data[s] = (int) tmp;
		}
	}
}

bool PGM_reader::isNum(unsigned char c)
{
	string nums = "0123456789";
	for(int i = 0; i < nums.size(); i++)
	{
		if(c == nums[i]) return true;
	}
	return false;
}

int PGM_reader::getInt(FILE * fi)
{
	string op = "";
	unsigned char tmp;
	bool seen = false;
	do
	{
		make_sure(fread_s(&tmp, 1, 1, 1, fi) == 1, "could not read file");
		if(isNum(tmp))
		{
			op += tmp;
			seen = (!seen) ? true : seen;
		}
			
	}while(isNum(tmp) || !seen);
		
	int rv = 0;
	sscanf(op.c_str(), "%i", &rv);
	return rv;
		
}

Size PGM_reader::size()
{
	return Size(width, height);
}

PGM_reader::PGM_reader ()
{
	basic_init();
}
PGM_reader::PGM_reader(string fname)
{
	basic_init();
	file_name = fname;
	setup();
}

PGM_reader::~PGM_reader()
{
	basic_de_init();
}

void PGM_reader::open(string fname)
{
	basic_de_init();
	basic_init();
	file_name = fname;
	setup();
}

	
Mat PGM_reader::getUCIm()
{
	Mat rv = Mat::zeros(Size(width, height), CV_8UC1);
	for(int i = 0; i < width * height; i++)
	{
		rv.data[i] = (unsigned char)((im_data[i] / (double) max_pixel_val) * 255.0);
	}
	return rv;
}

Mat PGM_reader::getFIm()
{
	Mat rv = Mat::zeros(Size(width, height), CV_64FC1);
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			rv.at<double>(Point2i(x,y)) = im_data[y*width + x] / ( double ) max_pixel_val;
		}
	}
	return rv;
}


//ppm reader

void PPM_reader::setup()
{
	FILE * fi = fopen(file_name.c_str(), "rb");

	char ps_[4];
	make_sure(fread_s(ps_, 4, 1, 2, fi) == 2, "could not read");
	ps_[2] = NULL;
	make_sure(string(ps_) == "P6", "not ppm image");

	width = getInt(fi);
	height = getInt(fi);
	max_pixel_val = getInt(fi);
	setup_pixels_(fi);
	fclose(fi);
}

void PPM_reader::setup_pixels_(FILE * fi)
{
	im_data = new int[width * height * 3];
	heap_used = true;
	if(max_pixel_val > 255)
	{
		for(int s = 0; s < width * height * 3; s++)
		{
			unsigned short tmp = 0;
			fread_s(&tmp, sizeof(short), sizeof(short), 1, fi);
			swap_short_byte(tmp);
			im_data[s] = (int) tmp;
		}
	}else
	{
		for(int s = 0; s < width * height * 3; s++)
		{
			unsigned char tmp = 0;
			fread_s(&tmp, sizeof(char), sizeof(char), 1, fi);
			im_data[s] = (int) tmp;
		}
	}
}

	

	

Vec3i PPM_reader::bgr(Vec3i & inp)
{
	return Vec3i(inp[2], inp[1], inp[0]);
}


PPM_reader::PPM_reader () : PGM_reader()
{
}
PPM_reader::PPM_reader(string fname)
{
	basic_init();
	file_name = fname;
	setup();
}

PPM_reader::~PPM_reader()
{
		
}

	

Vec3i PPM_reader::pixel(int x, int y)
{
	int * ptr = im_data + (y*width*3 + x*3);
	return bgr(Vec3i(*ptr, *(ptr+1), *(ptr+2)));
}
	
Mat PPM_reader::getUCIm()
{
	Mat rv = Mat::zeros(Size(width, height), CV_8UC3);
	for(int y = 0; y < rv.size().height; y++)
	{
		for(int x = 0; x < rv.size().width; x++)
		{
			Vec3i pix = pixel(x,y);
			Vec3b pix_out;
			for(int i = 0; i < 3; i++)
			{
				pix_out[i] = (unsigned char)((pix[i] / (double) max_pixel_val) * 255.0);
			}
			rv.at<Vec3b>(Point2i(x,y)) = pix_out;
		}
	}
	return rv;
}

Mat PPM_reader::getFIm()
{
	Mat rv = Mat::zeros(Size(width, height), CV_64FC3);
	for(int y = 0; y < rv.size().height; y++)
	{
		for(int x = 0; x < rv.size().width; x++)
		{
			Vec3i pix = pixel(x,y);
			Vec3d pix_out;
			for(int i = 0; i < 3; i++)
			{
				pix_out[i] = ((pix[i] / (double) max_pixel_val) * 255.0);
			}
			rv.at<Vec3d>(Point2i(x,y)) = pix_out;
		}
	}
	return rv;
}

//yuv reader:

void YUV_reader::init()
{
	file_ = NULL;
	width = height = 0;
	file_name = "";
}

void YUV_reader::open_file()
{
	file_ = fopen(file_name.c_str(), "rb");
}

void YUV_reader::close_file()
{
	fclose(file_);
}
	
void YUV_reader::clamp(double & b, double a, double c)
{
	if(b < a) b = a;
	else if(b > c) b = c;
}


Vec3b YUV_reader::yuv2rgb(Vec3b inp)
{
	double y = (double)inp[0];
	double u = (double)inp[1];
	double v = (double)inp[2];
	double red = y + (1.370705 * (double)(v-128.0));
	double green = y - (0.698001 * (v-128.0)) - (0.337633 * (u-128.0));
	double blue = y + (1.732446 * (u-128.0));
	clamp(red, 0.0, 255.0);
	clamp(green, 0.0, 255.0);
	clamp(blue, 0.0, 255.0);
	Vec3b rv;
	rv[0] = (unsigned char) blue;
	rv[1] = (unsigned char) green;
	rv[2] = (unsigned char) red;
	return rv;
}

Mat YUV_reader::read_frame(bool & canGet)
{
		
	int size = width * height;
	int smallSize = size / 4;
	unsigned char * raw = new unsigned char[size + smallSize * 2];
	int ar = fread_s(raw, size + smallSize * 2, 1, size + smallSize * 2, file_);
	Mat rv = Mat::zeros(Size(width, height), CV_8UC3);
	if(ar == size + smallSize * 2)
	{
		canGet = true;
	}else
	{
		canGet = false;
		return rv;
	}
		
	//get yuvs
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			Vec3b yuv;
			yuv[0] = raw[y*width + x];
			yuv[1] = raw[(y/2) * (width/2) + (x/2) + size];
			yuv[2] = raw[(y/2) * (width/2) + (x/2) + size + smallSize];
			rv.at<Vec3b>(Point2i(x,y)) = yuv2rgb(yuv);
		}
	}

	delete [] raw;
	return rv;
}


YUV_reader::YUV_reader(string fname, int w, int h)
{
	init();
	file_name = fname;
	width = w;
	height = h;
	open_file();
}

YUV_reader::~YUV_reader()
{
	close_file();
}

bool YUV_reader::get(Mat & m)
{
	bool rv;
	m = read_frame(rv);
	return rv;
}

YUV_reader & YUV_reader::operator =(const YUV_reader & inp)
{
	init();
	file_name = inp.file_name;
	width = inp.width;
	height = inp.height;
	open_file(); return *this;
}

}//ending namespace ldr