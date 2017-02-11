#ifndef LBIM_H
#define LBIM_H

#include "LImPro.h"
#include <vector>
#include "LBMC.h"

using namespace LImPro_NS;
#define FG 0xFF
#define BG 0x00
#define DC 0x01

typedef unsigned char bpixel;

namespace LBIM_NS
{

	class LBIM
	{
	public:
		int width, height;
		bpixel * data;
		
		LBIM();
		~LBIM();
		LBIM(int w, int h);
		LBIM(char * fname, float th);
		LBIM(LImPro im, float th);
		LBIM(LImPro & im);
		LBIM(const LBIM & im);
		LBIM & operator = (const LBIM & inp);
		LBIM copy();


		LImPro getlimpro();
		void setlimpro(LImPro & imin);
		void save(char * fname);

		void set(int x, int y, bpixel p);
		bpixel get(int x, int y) const;

		bool inrange(int x, int y) const;

		void set(bpixel p);
		void set_size(int nw, int nh);
		void setTo(const LBIM & imin);

		void setSquareSE(int half_size);
		void set4CSE();
		void setCircular(int rad);

		LBIM erode(LBIM & SE);
		LBIM dilate(LBIM & SE);


		void reflect();

		LBIM open(LBIM & SE);
		LBIM close(LBIM & SE);
		bool operator == (LBIM & imin);

		
		LImPro getDistanceMap();
		float distanceToClosestBG(std::vector<LBMC_Lincoln::LBMC_Point> & border, int x, int y);

		LImPro getDistanceMapSquareDistance();
		float distanceToClosestBGSquareDistance(std::vector<LBMC_Lincoln::LBMC_Point> & border, int x, int y);

		LBIM subtract(LBIM & in);

		bool nextTo(int x, int y, bpixel p);
		void invert();

		std::vector<LBMC_Lincoln::LBMC_Point> get_border();

		bool can_delete_skeleton_algorithm(int x, int y, bool step = false);

		LBIM skeleton_step();

		LBIM get_skeleton();

		bool match(LBIM & se, int x, int y);
		bool isCorner(int x, int y);

		std::vector<LBMC_Lincoln::LBMC_Point> getCorners();
	};








}



#endif