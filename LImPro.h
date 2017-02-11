#ifndef LImPro_H
#define LImPro_H

namespace LImPro_NS
{

	class LImPro
	{
	public:
		int width, height;
		float* data;
		LImPro();
		LImPro(char* fname);
		LImPro(int wid_in, int hei_in);
		LImPro(LImPro* im_in);
		LImPro(LImPro& im_in);
		LImPro& operator=(LImPro& im_in);
		~LImPro();
		float get(int x, int y);
		void set(int x, int y, float v);
		float get_f(int x, int y);
		void set_f(int x, int y, float v);
		int pixel_in_bounds(int x, int y);
		void save(char* fname);
		void direct_copy(LImPro& im_in);
		void set_size(int new_width, int new_height);
		void fix_odd_size();

		//basic operators
		void set(float v); //set all pixels to v
		void scale(float sc); //scale all pixels by sc
		void add(float v); //add all pixels by v
		void div(float v); //divide all pixels by div
		void threshold(float v); //all pixels OVER v = their original value, all pixels < v = 0
		void binaryThreshold(float v); //all pixels OVER v = 1, all under = 0
		float get_global_variance();
		float get_global_mean();
		float get_global_standard_deviation();
		float get_global_max();
		float get_global_min();
		void normalize();
		void map(float min_v, float max_v);


		void bilinearly_interpolate(int new_width, int new_height);
		void basic_resample(int new_width, int new_height);

		//filters
		void box_filter(int width_half, int height_half);

		void median_filter(int width_half, int height_half);
		void max_filter(int width_half, int height_half);
		void set_if_max_filter(int width_half, int height_half);
		void bilateral_filter(float sd, float sr);
		void moment_filter(int width_half, int height_half, float moment);
		void roberts_filter();
		void correlate(LImPro& filter);
		void correlate_N(LImPro& filter);
		void flip_180();
		void convolute(LImPro& filter);
		void convolute_N(LImPro& filter);


		float get_local_mean(int x, int y, int width_half, int height_half);
		float get_local_max(int x, int y, int width_half, int height_half);
		float get_moment(int x, int y, int width_half, int height_half, float num_moments);
		float get_roberts_edge(int x, int y);


		void generate_box_filter(int radius);
		void generate_pillbox_filter(int radius);
		void generate_gausian_filter(float std_dev);
		void generate_gausian_filter(int radius);


		float Gausian2D(float std_dev, int x, int y);
		float Gausian1D(float std_dev, float dist);
		void draw_box(int xA, int yA, int w, int h, float g, bool fill = false);

	
	};









}


#endif
