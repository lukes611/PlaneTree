#include "LImPro.h"

#include "LBMC.h"

namespace LImPro_NS
{

LImPro::LImPro()
{
	width = 1;
	height = 1;
	data = new float[width * height];
	set(0.0f);
}

LImPro::LImPro(char* fname)
{
	LBMC_Lincoln::LBMC<float> im(fname);
	
	width = im.width;
	height = im.height;
	data = new float[width * height];
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			LBMC_Lincoln::LBMC_Pixel<float> pixel = im.getPixel(LBMC_Lincoln::LBMC_Point(x,y));
			float f = ((float)pixel.r) + ((float)pixel.g) + ((float)pixel.b);
			f /= 3.0f;
			set(x,y, f);
		}
	}
}
LImPro::LImPro(int wid_in, int hei_in)
{
	width = wid_in;
	height = hei_in;
	data = new float[width * height];
	set(0.0f);
}
LImPro::LImPro(LImPro* im_in)
{
	width = im_in->width;
	height = im_in->height;
	data = new float[width * height];
	direct_copy(*im_in);
}
LImPro::LImPro(LImPro& im_in)
{
	width = im_in.width;
	height = im_in.height;
	data = new float[width * height];
	direct_copy(im_in);
}
LImPro& LImPro::operator=(LImPro& im_in)
{
	delete [] data;
	width = im_in.width;
	height = im_in.height;
	data = new float[width * height];
	direct_copy(im_in);
	return *this;
}
LImPro::~LImPro()
{
	width = height = 0;
	delete [] data;
}
void LImPro::direct_copy(LImPro& im_in)
{
	if(width == im_in.width && height == im_in.height)
	{
		for(int y = 0; y < height; y++)
		{
			for(int x = 0; x < width; x++)
			{
				set( x, y, im_in.get(x, y) );
			}
		}
	}
}
float LImPro::get(int x, int y)
{
	if ( pixel_in_bounds(x, y)  == 1)
	{
		return data[y*width + x];
	}
	return 0.0f;
}
void LImPro::set(int x, int y, float v)
{
	if ( pixel_in_bounds(x, y)  == 1)
	{
		data[y*width + x] = v;
	}
}
float LImPro::get_f(int x, int y)
{
	return data[y*width + x];
}
void LImPro::set_f(int x, int y, float v)
{
	data[y*width + x] = v;
}
int LImPro::pixel_in_bounds(int x, int y)
{
	if(x < width && y < height && x >= 0 && y >= 0)
	{
		return 1;
	}
	return 0;
}
void LImPro::save(char* fname)
{
	LImPro im2(this);
	im2.map(0.0f, 255.0f);
	LBMC_Lincoln::LBMC<float> im(width, height);
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			float greyscale_pixel = im2.get(x,y);
			LBMC_Lincoln::LBMC_Pixel<float> pixel(greyscale_pixel, greyscale_pixel, greyscale_pixel);
			im.setPixel_safe(LBMC_Lincoln::LBMC_Point(x,y), pixel);

		}
	}
	im.Flip_Red_Green_Channels();
	
	im.save(fname);
}
void LImPro::set_size(int new_width, int new_height)
{
	delete [] data;
	width = new_width;
	height = new_height;
	data = new float[width*height];
	set(0.0f);
}
void LImPro::fix_odd_size()
{
	if(width % 2 == 1)
	{
		LImPro cp(this);
		set_size(width+1, height);
		for(int y = 0; y < height; y++)
		{
			for(int x = 0; x < width; x++)
			{
				set(x, y, cp.get(x,y));
			}
		}
	}
}

void LImPro::set(float v)
{
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			set(x , y, v);
		}
	}
}
void LImPro::scale(float sc)
{
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			set(x , y, get(x, y) * sc);
		}
	}
}
void LImPro::add(float v)
{
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			set(x , y, get(x, y) + v);
		}
	}
}
void LImPro::div(float v)
{
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			set(x , y, get(x, y) / v);
		}
	}
}
void LImPro::threshold(float v)
{
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			set(x , y, (get(x, y) >= v) ? get(x,y) : 0.0f);
		}
	}
}
void LImPro::binaryThreshold(float v)
{
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			set(x , y, get(x, y) >= v ? 1.0f : 0.0f);
		}
	}
}
float LImPro::get_global_variance()
{
	double var = 0.0f;
	double mean = get_global_mean();
	double count = 0.0;
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			double pix = (double) get(x, y);
			double tmp = pix - mean;
			tmp *= tmp;
			var += tmp;
			count += 1.0;
		}
	}
	return (float) (var / count);
}
float LImPro::get_global_mean()
{
	double mean = get_global_mean();
	double count = 0.0;
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			mean += (double) get(x, y);
			count += 1.0;
		}
	}
	return (float) (mean / count);
}
float LImPro::get_global_standard_deviation()
{
	return sqrt(get_global_variance());
}
float LImPro::get_global_max()
{
	float max_val = get(0, 0);
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			float pix = get(x, y);
			max_val = max_val < pix ? pix : max_val;
		}
	}
	return max_val;
}
float LImPro::get_global_min()
{
	float min_val = get(0, 0);
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			float pix = get(x, y);
			min_val = min_val > pix ? pix : min_val;
		}
	}
	return min_val;
}
void LImPro::normalize()
{
	float max_v = get_global_max();
	float min_v = get_global_min();
	float diff = max_v - min_v;
	add(-min_v);
	div(diff);
}
void LImPro::map(float min_v_inp, float max_v_inp)
{
	float max_v = get_global_max();
	float min_v = get_global_min();
	float diff = max_v - min_v;
	add(-min_v);
	div(diff);

	float diff_in = max_v_inp - min_v_inp;
	scale(diff_in);
	add(min_v_inp);

}



void LImPro::bilinearly_interpolate(int new_width, int new_height)
{
	LImPro cp(this);
	
	delete [] data;
	this->width = new_width;
	this->height = new_height;
	this->data = new float[width * height];

	float xat = 0.0f;
	float yat = 0.0f;
	float xinc = cp.width / (float)width;
	float yinc = cp.height / (float)height;
	
	for (int y = 0; y < height; y++)
	{
		xat = 0.0f;
		for (int x = 0; x < width; x++)
		{
			
			float yc = yat;
			float ya = floor(yc);
			float yb = ya + 1.0f;
			


			float xa1 = floor(xat);
			float xb1 = xa1 + 1.0f;
			float xc1 = xat;
			float xt1 = (xc1 - xa1) / (xb1 - xa1);
			float xdiff1 = cp.get(xb1, ya) - cp.get(xa1, ya);
			float xdiff2 = cp.get(xb1, yb) - cp.get(xa1, yb);
			float xpix1 = cp.get(xa1, ya) + xt1 * xdiff1;
			float xpix2 = cp.get(xa1, yb) + xt1 * xdiff2;

			float ydiff = xpix2 - xpix1;
			float yt = (yc - ya) / (yb - ya);
			float pix_out = xpix1 + yt * ydiff;

			set(x, y, pix_out);
			xat += xinc;
		}
		yat += yinc;
	}

}

void LImPro::basic_resample(int new_width, int new_height)
{
	float x_scale = width / (float)new_width;
	float y_scale = height / (float)new_height;
	LImPro cp(this);
	delete [] data;
	width = new_width;
	height = new_height;
	data = new float[width * height];
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			float xf = x_scale * (float)x;
			float yf = y_scale * (float)y;
			int xi = (int)(xf+0.5f);
			int yi = (int)(yf+0.5f);
			set(x,y, cp.get(xi, yi));
		}
	}
}


void LImPro::box_filter(int width_half, int height_half)
{
	LImPro cp(this);
	float div = (float)( ((width_half*2)+1) * ((height_half*2)+1) );
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			float sum = 0.0f;
			for(int j = -height_half; j < height_half; j++)
			{
				for(int i = -width_half; i < width_half; i++)
				{
					sum += cp.get(x + i, y + j);
				}
			}
			sum /= div;
			set(x,y, sum);
			

		}
	}
}

void LImPro::median_filter(int width_half, int height_half)
{
	LImPro cp(this);

	float maxv = cp.get_global_max();
	float minv = cp.get_global_min();
	cp.map(0.0f, 255.0f);

	int index = ( ((width_half*2)+1) * ((height_half*2)+1) );
	index /= 2;
	int histogram[256];
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			memset(histogram, 0, sizeof(int) * 256);
			for(int j = -height_half; j < height_half; j++)
			{
				for(int i = -width_half; i < width_half; i++)
				{
					int pixel = (int)cp.get(x + i, y + j);
					histogram[pixel]++;
				}
			}
			int median_counter = 0;
			int out_col = 0;
			while(median_counter < index)
			{
				median_counter += histogram[out_col];
				out_col++;
			}
			float oc = (float) out_col;
			set(x,y, oc);
			

		}
	}
	map(minv, maxv);
}


void LImPro::max_filter(int width_half, int height_half)
{
	LImPro cp(this);

	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			float p = cp.get_local_max(x,y, width_half, height_half);
			set(x,y, p);
		}
	}
}


void LImPro::set_if_max_filter(int width_half, int height_half)
{
	LImPro cp(this);

	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			float p = cp.get_local_max(x,y, width_half, height_half);
			if(cp.get(x,y) == p)
				set(x,y, p);
			else
			{
				set(x,y, 0.0f);
			}
		}
	}
}

float LImPro::get_local_mean(int x, int y, int width_half, int height_half)
{
	float div = (float)( ((width_half*2)+1) * ((height_half*2)+1) );
	float sum = 0.0f;
	for(int j = -height_half; j < height_half; j++)
	{
		for(int i = -width_half; i < width_half; i++)
		{
			sum += get(x + i, y + j);
		}
	}
	return sum / div;
}

float LImPro::get_local_max(int x, int y, int width_half, int height_half)
{
	float div = (float)( ((width_half*2)+1) * ((height_half*2)+1) );
	float ma = 0.0f;
	bool set = false;
	for(int j = -height_half; j < height_half; j++)
	{
		for(int i = -width_half; i < width_half; i++)
		{
			if(!set){
				ma = get(x + i, y + j);
				set = true;
			}
			else if(get(x + i, y + j) > ma)
				ma = get(x + i, y + j);
		}
	}
	return ma;
}



float LImPro::get_moment(int x, int y, int width_half, int height_half, float num_moments)
{
	
	float div = (float)( ((width_half*2)+1) * ((height_half*2)+1) );
	float sum = 0.0f;
	float mean = get_local_mean(x, y, width_half, height_half);
	for(int j = -height_half; j < height_half; j++)
	{
		for(int i = -width_half; i < width_half; i++)
		{
			sum += pow(get(x + i, y + j) - mean, num_moments);
		}
	}
	return sum / div;
}

void LImPro::moment_filter(int width_half, int height_half, float moment)
{
	LImPro cp(this);
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			float pixel = cp.get_moment(x,y, width_half, height_half, moment);
			set(x,y, pixel);
		}
	}
}


float LImPro::get_roberts_edge(int x, int y)
{
	float pix = get(x, y);
	float difY = get(x+1, y) - pix;
	float difX = get(x, y+1) - pix;
	difX *= difX;
	difY *= difY;
	return sqrt(difX + difY);
}

void LImPro::roberts_filter()
{
	LImPro cp(this);
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			set(x,y, cp.get_roberts_edge(x,y));
		}
	}
}

void LImPro::correlate(LImPro& filter)
{
	int xs = filter.width / 2;
	int ys = filter.height / 2;
	float sum = 0.0f;
	LImPro cp(this);
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			sum = 0.0f;
			for(int j = 0; j < filter.height; j++)
			{
				for(int i = 0; i < filter.width; i++)
				{
					sum += filter.get(i, j) * cp.get(x + (i - xs), y + (j - ys));
				}
			}
			set(x, y, sum);
		}
	}
}

void LImPro::correlate_N(LImPro& filter)
{
	int xs = filter.width / 2;
	int ys = filter.height / 2;
	float sum = 0.0f;
	float count = (float)(filter.width * filter.height);
	LImPro cp(this);
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			sum = 0.0f;
			for(int j = 0; j < filter.height; j++)
			{
				for(int i = 0; i < filter.width; i++)
				{
					sum += filter.get(i, j) * cp.get(x + (i - xs), y + (j - ys));
				}
			}
			set(x, y, sum/count);
		}
	}
}

void LImPro::convolute(LImPro& filter2)
{
	LImPro filter = filter2;
	filter.flip_180();
	int xs = filter.width / 2;
	int ys = filter.height / 2;
	float sum = 0.0f;
	LImPro cp(this);
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			sum = 0.0f;
			for(int j = 0; j < filter.height; j++)
			{
				for(int i = 0; i < filter.width; i++)
				{
					sum += filter.get(i, j) * cp.get(x + (i - xs), y + (j - ys));
				}
			}
			set(x, y, sum);
		}
	}
}

void LImPro::convolute_N(LImPro& filter2)
{
	LImPro filter = filter2;
	filter.flip_180();
	int xs = filter.width / 2;
	int ys = filter.height / 2;
	float sum = 0.0f;
	float count = (float)(filter.width * filter.height);
	LImPro cp(this);
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			sum = 0.0f;
			for(int j = 0; j < filter.height; j++)
			{
				for(int i = 0; i < filter.width; i++)
				{
					sum += filter.get(i, j) * cp.get(x + (i - xs), y + (j - ys));
				}
			}
			set(x, y, sum/count);
		}
	}
}

void LImPro::bilateral_filter(float sd, float sr)
{
	float sum, wp, w, p;
	LImPro cp = this;
	LImPro g;
	g.generate_gausian_filter(sd);
	int box_half = g.width /2;
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			sum = 0.0f;
			wp = 0.0f;
			p = cp.get(x,y);
			for(int y2 = y-box_half; y2 < y+box_half; y2++)
			{
				for(int x2 = x-box_half; x2 < x+box_half; x2++)
				{
					w = g.get((x2-x)+box_half, (y2-y)+box_half) * this->Gausian1D(sr, abs(cp.get(x2, y2) - p));
					wp += w;
					sum += w * cp.get(x2, y2);
				}
			}
			sum /= wp;
			set(x,y, sum);
		}
	}
}

void LImPro::generate_box_filter(int radius)
{
	set_size(radius*2 - 1, radius*2 - 1);	
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			set(x, y, 1.0f);
		}
	}
	
}

void LImPro::generate_pillbox_filter(int radius)
{
	set_size(radius*2 - 1, radius*2 - 1);	
	float centerX = (float)((width) / 2);
	float centerY = (float)(height / 2);
	
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			float xf = (float)x;
			float yf = (float)y;
			xf -= centerX;
			yf -= centerY;
			xf*=xf;
			yf*=yf;
			xf += yf;
			xf = sqrt(xf);
			if(xf < (float) radius)
			{
				set(x, y, 1.0f);
			}else
			{
				set(x, y, 0.0f);
			}
		}
	}
	
}

float LImPro::Gausian2D(float std_dev, int x, int y)
{
	float e = 2.71828f;
	float pi = 3.141592;
	std_dev = pow(std_dev, 2.0f);
	float p1 = 1.0f / (2.0f * pi * std_dev);
	float p3 = -((pow(x,2.0f) + pow(y,2.0f)) / (2.0f * std_dev));
	return p1 * pow(e, p3);
}

float LImPro::Gausian1D(float std_dev, float dist)
{
	float e = 2.71828f;
	float a = 1.0f / (sqrt(2.0f * 3.141592) * std_dev);
	float c = (dist*dist) / (2.0f*std_dev*std_dev);
	float b = exp(-c);
	return a * b;
}


void LImPro::generate_gausian_filter(float std_dev)
{
	int kernel_size = (int)(std_dev * 6.0f + 0.5f);
	if(kernel_size % 2 == 0)
	{
		kernel_size++;
	}
	set_size(kernel_size, kernel_size);

	int x_shift = width/2;
	int y_shift = height/2;
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			float pixel = Gausian2D(std_dev, x-x_shift, y-y_shift);
			set(x, y, pixel);
		}
	}
}

void LImPro::generate_gausian_filter(int radius)
{

	float std_dev = ((float)radius) / 6.0f;
	set_size(radius*2 - 1, radius*2 - 1);

	int x_shift = width/2;
	int y_shift = height/2;
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			float pixel = Gausian2D(std_dev, x-x_shift, y-y_shift);
			set(x, y, pixel);
		}
	}
}

void LImPro::flip_180()
{
	LImPro cp(this);
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			set(x,y, cp.get(width - 1 - x, height - 1 - y));
		}
	}
}


void LImPro::draw_box(int xA, int yA, int w, int h, float g, bool fill)
{
	if(fill)
	{
		for(int y = yA; y < yA + h; y++)
		{
			for(int x = xA; x < xA + w; x++)
			{
				set(x, y, g);
			}
		}
	}else
	{
		int m = (w > h) ? w : h;
		for(int i = 0; i < m; i++)
		{
			if(i <= w)
			{
				set(xA + i, yA, g);
				set(xA + i, yA + h, g);
			}
			if(i < h)
			{
				set(xA, yA + i, g);
				set(xA + w, yA + i, g);
			}
		}
	}
}





}

