#include "Lbim.h"
#include "LBMC.h"
#include <queue>
#include <iostream>
using namespace std;
using namespace LBMC_Lincoln;


namespace LBIM_NS
{

LBIM::LBIM()
{
	width = height = 1;
	data = new bpixel[width * height];
	set(BG);
}
LBIM::~LBIM()
{
	delete [] data;
	width = height = 0;
}
LBIM::LBIM(int w, int h)
{
	width = w;
	height = h;
	data = new bpixel[width * height];
	set(BG);
}
LBIM::LBIM(char * fname, float th)
{
	LImPro im = fname;
	im.normalize();
	im.binaryThreshold(th);
	width = im.width;
	height = im.height;
	data = new bpixel[width * height];
	setlimpro(im);
}
LBIM::LBIM(LImPro im, float th)
{
	im.normalize();
	im.binaryThreshold(th);
	width = im.width;
	height = im.height;
	data = new bpixel[width * height];
	setlimpro(im);
}
LBIM::LBIM(LImPro & im)
{
	set_size(im.width, im.height);
	setlimpro(im);
}
LBIM::LBIM(const LBIM & im)
{
	width = height = 1;
	data = new bpixel[width * height];
	setTo(im);
	
}
LBIM & LBIM::operator = (const LBIM & inp)
{
	setTo(inp);
	return *this;
}

LBIM LBIM::copy()
{
	LBIM rv = *this;
	return rv;
}

void LBIM::set_size(int nw, int nh)
{
	if(width != nw || height != nh)
	{
		delete [] data;
		width = nw;
		height = nh;
		data = new bpixel[width * height];
	}
}


LImPro LBIM::getlimpro()
{
	LImPro im(width, height);
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			if(get(x,y) == FG)
			{
				im.set(x,y, 0.0f);
			}else
			{
				im.set(x,y, 1.0f);
			}
		}
	}
	return im;
}
void LBIM::save(char * fname)
{
	LImPro im = getlimpro();
	im.save(fname);
}

void LBIM::set(int x, int y, bpixel p)
{
	if(inrange(x,y))
	{
		data[y*width + x] = p;
	}
}
bpixel LBIM::get(int x, int y) const
{
	if(inrange(x,y))
	{
		return data[y*width + x];
	}
	return BG;
}

bool LBIM::inrange(int x, int y) const
{
	if(x >= 0 && y >= 0 && x < width && y < height)
	{
		return true;
	}
	return false;
}

void LBIM::set(bpixel p)
{
	int size = width * height;
	for(int i = 0; i < size; i++)
	{
		data[i] = p;
	}
}

void LBIM::setlimpro(LImPro & imin)
{
	int mw = (width > imin.width)? imin.width : width;
	int mh = (height > imin.height)? imin.height : height;
	for(int y = 0; y < mh; y++)
	{
		for(int x = 0; x < mw; x++)
		{
			if(imin.get(x,y) == 1.0f)
			{
				set(x,y, BG);
			}else
			{
				set(x,y, FG);
			}
		}
	}
}

void LBIM::setTo(const LBIM & imin)
{
	set_size(imin.width, imin.height);
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			set(x,y, imin.get(x,y));
		}
	}
}


void LBIM::setSquareSE(int half_size)
{
	int size = (half_size * 2) + 1;
	set_size(size, size);
	set(FG);
}
void LBIM::set4CSE()
{
	set_size(3,3);
	set(0, 0, BG);set(1, 0, FG);set(2, 0, BG);
	set(0, 1, FG);set(1, 1, FG);set(2, 1, FG);
	set(0, 2, BG);set(1, 2, FG);set(2, 2, BG);
}

LBIM LBIM::erode(LBIM & SE)
{
	LBIM rv(width, height);
	int hw = SE.width / 2;
	int hh = SE.height / 2;
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			bool shade = true;
			for(int q = 0; q < SE.height; q++)
			{
				for(int p = 0; p < SE.width; p++)
				{

					if(SE.get(p, q) == FG && get((p-hw)+x, (q-hh)+y) == BG)
					{
						shade = false;
						break;
					}
				}
				if(!shade)
				{
					break;
				}
			}
			if(shade)
			{
				rv.set(x,y, FG);
			}else
			{
				rv.set(x,y, BG);
			}
		}
	}
	return rv;
}
LBIM LBIM::dilate(LBIM & SEin)
{
	LBIM SE = SEin;
	SE.reflect();
	LBIM rv(width, height);
	int hw = SE.width / 2;
	int hh = SE.height / 2;
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			bool shade = false;
			for(int q = 0; q < SE.height; q++)
			{
				for(int p = 0; p < SE.width; p++)
				{

					if(SE.get(p, q) == FG && get((p-hw)+x, (q-hh)+y) == FG)
					{
						shade = true;
						break;
					}
				}
				if(shade)
				{
					break;
				}
			}
			if(shade)
			{
				rv.set(x,y, FG);
			}else
			{
				rv.set(x,y, BG);
			}
		}
	}
	return rv;
}

void LBIM::reflect()
{
	LBIM cp = *this;
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			bpixel p = cp.get(x,y);
			int ax = width-x;
			int ay = height-y;
			set(ax, ay, p);
		}
	}
}


LBIM LBIM::open(LBIM & SE)
{
	LBIM rv = erode(SE);
	rv = rv.dilate(SE);
	return rv;
}

LBIM LBIM::close(LBIM & SE)
{
	LBIM rv = dilate(SE);
	rv = rv.erode(SE);
	return rv;
}
bool LBIM::operator == (LBIM & imin)
{
	if(width != imin.width || height != imin.height)
	{
		return false;
	}
	int size = width * height;
	for(int i = 0; i < size; i++)
	{
		if(data[i] != imin.data[i])
		{
			return false;
		}
	}
	return true;
}



vector<LBMC_Point> LBIM::get_border()
{
	vector<LBMC_Point> rv;
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			if(get(x,y) == FG && nextTo(x,y, BG))
			{
				rv.push_back(LBMC_Point(x,y));
			}
		}
	}
	return rv;
}

bool LBIM::can_delete_skeleton_algorithm(int x, int y, bool step2)
{
	int np = 0, tp = 0;
	bool p2 = false;
	np = (get(x, y-1) == FG)? np+1 : np;
	np = (get(x+1, y-1) == FG)? np+1 : np;
	np = (get(x+1, y) == FG)? np+1 : np;
	np = (get(x+1, y+1) == FG)? np+1 : np;
	np = (get(x, y+1) == FG)? np+1 : np;
	np = (get(x-1, y+1) == FG)? np+1 : np;
	np = (get(x-1, y) == FG)? np+1 : np;
	np = (get(x-1, y-1) == FG)? np+1 : np;

	tp = (get(x, y-1) == BG && get(x+1, y-1) == FG)? tp+1 : tp;
	tp = (get(x+1, y-1) == BG && get(x+1, y) == FG)? tp+1 : tp;
	tp = (get(x+1, y) == BG && get(x+1, y+1) == FG)? tp+1 : tp;
	tp = (get(x+1, y+1) == BG && get(x, y+1) == FG)? tp+1 : tp;
	tp = (get(x, y+1) == BG && get(x-1, y+1) == FG)? tp+1 : tp;
	tp = (get(x-1, y+1) == BG && get(x-1, y) == FG)? tp+1 : tp;
	tp = (get(x-1, y) == BG && get(x-1, y-1) == FG)? tp+1 : tp;
	tp = (get(x-1, y-1) == BG && get(x, y-1) == FG)? tp+1 : tp;


	if(!step2)
	{
		p2 = (get(x,y-1) == BG || get(x+1,y) == BG || get(x,y+1) == BG) && (get(x+1,y) == BG || get(x,y+1) == BG || get(x-1,y) == BG);
	}else
	{
		p2 = (get(x,y-1) == BG || get(x+1,y) == BG || get(x-1,y) == BG) && (get(x,y-1) == BG || get(x,y+1) == BG || get(x-1,y) == BG);
	}

	if(p2 && tp == 1 && np >= 2 && np <= 6)
	{
		return true;
	}
	return false;
}


LImPro LBIM::getDistanceMap()
{
	LImPro rv(width, height);
	vector<LBMC_Point> border;
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			if(get(x,y) == BG)
			{
				if(nextTo(x,y, FG))
				{
					border.push_back(LBMC_Point(x,y));
				}
			}
		}
	}

	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			if(get(x,y) == FG)
			{
				rv.set(x,y, distanceToClosestBG(border, x, y));
			}else
			{
				rv.set(x,y, 0.0f);
			}
		}
	}
	rv.normalize();
	return rv;

}
float LBIM::distanceToClosestBG(vector<LBMC_Point> & border, int x, int y)
{
	LBMC_Point p(x,y);
	float cd = p.distance(border[0]);
	int s = border.size();
	for(int i = 1; i < s; i++)
	{
		float tmp = p.distance(border[i]);
		if(tmp < cd)
		{
			cd = tmp;
		}
	}
	return cd;
}


LImPro LBIM::getDistanceMapSquareDistance()
{
	LImPro rv(width, height);
	vector<LBMC_Point> border;
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			if(get(x,y) == BG)
			{
				if(nextTo(x,y, FG))
				{
					border.push_back(LBMC_Point(x,y));
				}
			}
		}
	}

	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			if(get(x,y) == FG)
			{
				rv.set(x,y, distanceToClosestBGSquareDistance(border, x, y));
			}else
			{
				rv.set(x,y, 0.0f);
			}
		}
	}
	rv.normalize();
	return rv;

}
float LBIM::distanceToClosestBGSquareDistance(vector<LBMC_Point> & border, int x, int y)
{
	LBMC_Point p(x,y);
	float cd = max(abs(p.x-border[0].x) , abs(p.y-border[0].y));
	int s = border.size();
	for(int i = 1; i < s; i++)
	{
		float tmp = max(abs(border[i].x-p.x) , abs(border[i].y-p.y));
		if(tmp < cd)
		{
			cd = tmp;
		}
	}
	return cd;
}


LBIM LBIM::subtract(LBIM & in)
{
	LBIM rv = *this;
	for(int y = 0; y < in.height; y++)
	{
		for(int x = 0; x < in.width; x++)
		{
			if(in.get(x,y) == FG)
			{
				rv.set(x,y, BG);
			}
		}
	}
	return rv;
}

bool LBIM::nextTo(int x, int y, bpixel p)
{
	if(get(x+1, y) == p)
	{
		return true;
	}
	if(get(x+1, y-1) == p)
	{
		return true;
	}
	if(get(x, y-1) == p)
	{
		return true;
	}
	if(get(x-1, y-1) == p)
	{
		return true;
	}
	if(get(x-1, y) == p)
	{
		return true;
	}
	if(get(x-1, y+1) == p)
	{
		return true;
	}
	if(get(x, y+1) == p)
	{
		return true;
	}
	if(get(x+1, y+1) == p)
	{
		return true;
	}
	return false;
}

void LBIM::invert()
{
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			if(get(x,y) == BG)
			{
				set(x,y, FG);
			}
			else
			{
				set(x,y, BG);
			}
		}
	}
}

LBIM LBIM::skeleton_step()
{
	LBIM cp = *this;
	vector<LBMC_Point> border = cp.get_border();
	vector<LBMC_Point> del;
	int size = border.size();
	for(int i = 0; i < size; i++)
	{
		if(cp.can_delete_skeleton_algorithm(border[i].x, border[i].y, false))
		{
			del.push_back(border[i]);
		}
	}
	size = del.size();
	for(int i = 0; i < size; i++)
	{
		cp.set(del[i].x, del[i].y, BG);
	}

	//part 2

	border = cp.get_border();
	del.clear();
	size = border.size();
	for(int i = 0; i < size; i++)
	{
		if(cp.can_delete_skeleton_algorithm(border[i].x, border[i].y, true))
		{
			del.push_back(border[i]);
		}
	}
	size = del.size();
	for(int i = 0; i < size; i++)
	{
		cp.set(del[i].x, del[i].y, BG);
	}

	return cp;
}

LBIM LBIM::get_skeleton()
{
	LBIM rv = *this, tmp;
	while(true)
	{
		tmp = rv.skeleton_step();
		if(rv == tmp)
		{
			break;
		}else
		{
			rv = tmp;
		}
	}
	return rv;
}


void LBIM::setCircular(int rad)
{
	set_size(rad*2, rad*2);
	LBMC_Point p1, p2(rad, rad);
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			p1.x = x;
			p1.y = y;
			if(p1.distance(p2) < (float)rad)
			{
				set(x,y, FG);
			}else
			{
				set(x,y, BG);
			}

		}
	}
}

bool LBIM::match(LBIM & se, int xi, int yi)
{
	int hw = se.width / 2;
	int hh = se.height / 2;
	for(int y = yi-hh, y2 = 0; y2 < se.height; y++,y2++)
	{
		for(int x = xi-hw, x2 = 0; x2 < se.width; x++,x2++)
		{
			if(se.get(x2, y2) != get(x,y))
			{
				return false;
			}
		}
	}
	return true;
}

bool LBIM::isCorner(int x, int y)
{
	LBIM s(3,3);
	
	s.set(BG);
	s.set(1,1, FG);
	s.set(1, 0, FG);
	if(match(s, x,y))
	{
		return true;
	}

	s.set(BG);
	s.set(1,1, FG);
	s.set(2, 0, FG);
	if(match(s, x,y))
	{
		return true;
	}


	s.set(BG);
	s.set(1,1, FG);
	s.set(2, 1, FG);
	if(match(s, x,y))
	{
		return true;
	}

	s.set(BG);
	s.set(1,1, FG);
	s.set(2, 2, FG);
	if(match(s, x,y))
	{
		return true;
	}

	s.set(BG);
	s.set(1,1, FG);
	s.set(1, 2, FG);
	if(match(s, x,y))
	{
		return true;
	}

	s.set(BG);
	s.set(1,1, FG);
	s.set(0, 2, FG);
	if(match(s, x,y))
	{
		return true;
	}

	s.set(BG);
	s.set(1,1, FG);
	s.set(0, 1, FG);
	if(match(s, x,y))
	{
		return true;
	}

	s.set(BG);
	s.set(1,1, FG);
	s.set(0, 0, FG);
	if(match(s, x,y))
	{
		return true;
	}

	return false;
}


vector<LBMC_Point> LBIM::getCorners()
{
	vector<LBMC_Point> rv;

	for(int y = 0;y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			if(this->isCorner(x,y))
			{
				rv.push_back(LBMC_Point(x,y));
			}
		}
	}

	return rv;
}

}