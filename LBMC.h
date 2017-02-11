#ifndef LBMC_H	//Luke's Bitmap colour data structure. It can also read BMP files
#define LBMC_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


namespace LBMC_Lincoln
{

	class LBMC_Point
	{
	public:
		int x, y;
		LBMC_Point()
		{
			x = y = 0;
		}
		LBMC_Point(int xin, int yin)
		{
			setAs(xin, yin);
		}
		LBMC_Point(const LBMC_Point& pin)
		{
			setAs(pin.x, pin.y);
		}
		LBMC_Point(LBMC_Point* pin)
		{
			setAs(pin->x, pin->y);
		}
		void operator=(const LBMC_Point& inp)
		{
			setAs(inp.x, inp.y);
		}
		void operator=(LBMC_Point* inp)
		{
			setAs(inp->x, inp->y);
		}
		void setAs(int xin, int yin)
		{
			x = xin;
			y = yin;
		}
		void add(LBMC_Point other)
		{
			x += other.x;
			y += other.y;
		}
		void subtract(LBMC_Point other)
		{
			x -= other.x;
			y -= other.y;
		}
		LBMC_Point operator+(LBMC_Point& p2)
		{
			LBMC_Point rv(this);
			rv.add(p2);
			return rv;
		}
		LBMC_Point operator-(LBMC_Point& p2)
		{
			LBMC_Point rv(this);
			rv.subtract(p2);
			return rv;
		}

		float distance(LBMC_Point& p2)
		{
			LBMC_Point rv(this);
			rv.subtract(p2);
			return sqrt(	(	(float)(rv.x*rv.x)	) + (	(float)(rv.y*rv.y)	)	);
		}

		void operator+=(LBMC_Point p2)
		{
			add(p2);
		}
		void operator-=(LBMC_Point p2)
		{
			subtract(p2);
		}
	};

	template <class Type>
	class LBMC_Pixel
	{
	public:
		Type r, g, b;
		LBMC_Pixel()
		{
			r = (Type)0;
			g = (Type)0;
			b = (Type)0;
		}
		LBMC_Pixel(LBMC_Pixel<Type>& pix_in)
		{
			setAs(pix_in);
		}

		LBMC_Pixel(LBMC_Pixel<Type>* pix_in)
		{
			setAs(*pix_in);
		}

		LBMC_Pixel(Type rin, Type gin, Type bin)
		{
			setAs(rin, gin, bin);
		}

		void operator=(LBMC_Pixel<Type>& pix_in)
		{
			setAs(pix_in);
		}

		void operator=(LBMC_Pixel<Type>* pix_in)
		{
			setAs(*pix_in);
		}

		void setAs(Type rin, Type gin, Type bin)
		{
			r = rin;
			g = gin;
			b = bin;
		}

		void setAs(LBMC_Pixel<Type>& pix_in)
		{
			r = pix_in.r;
			g = pix_in.g;
			b = pix_in.b;
		}

		void flipRG()
		{
			Type t = r;
			r = b;
			b = t;
		}

	};

	template <class Type>
	class LBMC
	{
	public:
		int width, height;
		LBMC_Pixel<Type>* data;
		LBMC(char* fn)
		{
			FILE* fi = fopen(fn, "rb");
			if(fi==0)
			{
				error("CANNOT FIND FILE");
			}
			unsigned char buff[1200];
			int fileSize,offset,numBytesInDBH;
			int rn = fread(buff, 1, 2, fi);
			rn = fread(&fileSize, 1, 4, fi);
			unsigned char* file = (unsigned char*)malloc(fileSize);
			fseek(fi, 0, SEEK_SET);
			unsigned char* p = file;
			int num_read = 0;
			while(num_read<fileSize)
			{
				rn = 1;
				unsigned char c = fgetc(fi);
				*p=c;
				p+=rn;
				num_read+=rn;
			}
			fclose(fi);
			if(num_read!=fileSize)
			{
				error("FILE SIZE INCORRECT");
			}

			memcpy(&offset, &file[10], 4);
		
			int tmpN;
			memcpy(&tmpN, &file[30], 4);
			short numBitsPerPixel;
			memcpy(&numBitsPerPixel, &file[28], 2);
			
			int compression;
			memcpy(&compression, &file[30], 4);
			
			memcpy(&width, &file[18], 4);
			memcpy(&height, &file[22], 4);
			data = new LBMC_Pixel<Type>[width * height];
	
	
			int rowpad = (width * 3) % 4;
	
			int rs = width * 3 + rowpad;
			int s = rs * height;
	
			unsigned char* imd = &file[offset + ((s)-(rs))];
			for(int y = 0; y < height; y++)
			{
				for(int x = 0; x < width; x++)
				{
					unsigned char r = imd[0],g=imd[1],b=imd[2];
					
					setPixel(LBMC_Point(x,y), LBMC_Pixel<Type>((Type)r, (Type)g, (Type)b));
					imd+=3;
			
				}
		
				imd-=width*3;
				imd-=rs;
			}
			
		}
		LBMC(int width_in, int height_in)
		{
			width = width_in;
			height = height_in;
			data = new LBMC_Pixel<Type>[width * height];
			clear();
		}

		~LBMC()
		{
			freeMemory();
		}

		void freeMemory()
		{
			width = 0, height = 0;
			delete [] data;
		}

		LBMC(LBMC<Type>& im)
		{
			width = im.width;
			height = im.height;
			data = new LBMC_Pixel<Type>[width * height];
			copy_Pixels_into(im);	
		}

		LBMC(LBMC<Type>* im)
		{
			width = im->width;
			height = im->height;
			data = new LBMC_Pixel<Type>[width * height];
			for(int y = 0; y < im->height && y < height; y++)
			{
				for(int x = 0; x < width && x < im->width; x++)
				{
					LBMC_Point point(x,y);
					setPixel( point, im->getPixel(point));
				}
			}
		}

		void operator=(LBMC<Type>& im2)
		{
			freeMemory();
			width = im2.width;
			height = im2.height;
			data = new LBMC_Pixel<Type>[width * height];
			copy_Pixels_into(im2);
		}

		void copy_Pixels_into(LBMC<Type>& im)
		{
			for(int y = 0; y < im.height && y < height; y++)
			{
				for(int x = 0; x < width && x < im.width; x++)
				{
					LBMC_Point point(x,y);
					setPixel( point, im.getPixel(point));
				}
			}
		}
		

		void save(char* fn)
		{
			flipData();
			FILE* fi = fopen(fn, "wb");
			int rs = width;
			int s = rs * height;
			int rowpad = (width * 3) % 4;
			//manual header save
			fputc('B', fi);fputc('M', fi);
			unsigned int fileSize = width*height*3 + 54, Zero = 0, tnum;
	
			fwrite(&fileSize, 4, 1, fi);
			fwrite(&Zero, 2, 1, fi);
			fwrite(&Zero, 2, 1, fi);
			tnum = 54;//offset
			fwrite(&tnum, 4, 1, fi);

	
			tnum = 40;
			fwrite(&tnum, 4, 1, fi);
			fwrite(&width, 4, 1, fi);
			fwrite(&height, 4, 1, fi);
			unsigned char tchar;unsigned short tshort;
			tshort = 1;//num planes
			fwrite(&tshort, 2, 1, fi);
			tshort = 24;
			fwrite(&tshort, 2, 1, fi);
	
	

			tnum = 0;//compression
			fwrite(&tnum, 4, 1, fi);
	
	
	
	
	
			tnum = width*height*3+rowpad;//imsize
			fwrite(&tnum, 4, 1, fi);

	


			tnum = 3780;//resolution e (hori,vert)
			fwrite(&tnum, 4, 1, fi);
			fwrite(&tnum, 4, 1, fi);
	
			fwrite(&Zero, 4, 1, fi);
			fwrite(&Zero, 4, 1, fi);

	
			//end manual header save


			for(int y = 0; y < height; y++){
				for(int x = 0; x < width; x++){
					unsigned char r,g,b;
					LBMC_Pixel<Type> pixel = getPixel(LBMC_Point(x,y));
					r = (unsigned char) pixel.r;
					g = (unsigned char) pixel.g;
					b = (unsigned char) pixel.b;
					fputc(r, fi);
					fputc(g, fi);
					fputc(b, fi);
					
				}
				for(int i = 0; i < rowpad; i++){
					putc(0, fi);
				}
	
			}
	
	


			fclose(fi);
			flipData();
		}

		void flipData()
		{
			LBMC<Type> copy = this;
			for(int y = 0, y2 = height-1; y < height; y++, y2--)
			{
				for(int x = 0; x < width; x++)
				{
					LBMC_Point p(x,y);
					LBMC_Point p2(x,y2);
					setPixel(p, copy.getPixel(p2));
				}
			}
		}

		void error(char* err)
		{
			std::cout << "ERROR IN LBMC CLASS\n";
			std::cout << err << std::endl;
			system("PAUSE");
			exit(2);
		}

		void clear()
		{
			for(int y = 0; y < height; y++)
			{
				for(int x = 0; x < width; x++)
				{
					setPixel(LBMC_Point(x,y), LBMC_Pixel<Type>());
				}
			}
		}

		void setPixel(LBMC_Point point, LBMC_Pixel<Type> pixel)
		{
			this->data[point.y*width + point.x] = pixel;
		}

		void setPixel_safe(LBMC_Point point, LBMC_Pixel<Type> pixel)
		{
			if(safe_range(point))
			{
				setPixel(point, pixel);
			}
		}

		int safe_range(LBMC_Point p1)
		{
			return (p1.x >= 0 && p1.y >= 0 && p1.x < width && p1.y < height)? 1 : 0;
		}

		LBMC_Pixel<Type> getPixel(LBMC_Point point)
		{
			return this->data[point.y*width + point.x];
		}

		LBMC_Pixel<Type> getPixel_safe(LBMC_Point point)
		{
			if(safe_range(point))
			{
				return getPixel(point);
			}
			return LBMC_Pixel<Type>();
		}

		void Flip_Red_Green_Channels()
		{
			for(int i = 0; i < width * height; i++)
			{
				data[i].flipRG();
			}
		}

		

	};
}


#endif