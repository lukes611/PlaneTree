#ifndef SIOBJ_H
#define SIOBJ_H
#include "R3.h"
#include <math.h>
#include "BitReaderWriter.h"
#include <string>
#include <vector>

using namespace LINCOLN;

namespace SIMPLE_OBJECT_LINCOLN
{


	class SI_Triangle
	{
	public:
		int a, b, c;
		SI_Triangle();
		SI_Triangle(int ina, int inb, int inc);
		SI_Triangle(const SI_Triangle& inp);
		SI_Triangle& operator=(const SI_Triangle& inp);
		SI_Triangle getCopy();
		void setAs(const SI_Triangle& inp);
		void setAs(int ina, int inb, int inc);
		int operator == (SI_Triangle inp);
		int operator > (SI_Triangle inp);
		int operator < (SI_Triangle inp);
		void order();
	};

	class SI_FullTriangle
	{
	public:
		R3 a, b, c;
		SI_FullTriangle();
		SI_FullTriangle(R3 ina, R3 inb, R3 inc);
		SI_FullTriangle(const SI_FullTriangle& inp);
		SI_FullTriangle& operator=(const SI_FullTriangle& inp);
		SI_FullTriangle getCopy();
		void setAs(SI_FullTriangle inp);
		void setAs(R3 ina, R3 inb, R3 inc);
		int pointInsideTriangle(R3 point, R3 normal);
		float RayIntersectsTriangle(R3 S, R3 V, R3 N, R3& intersectionPoint, int& hit);
	};

	class SI_Quad{
	public:
		R3 normal, point;
		float distance;
		SI_Quad();
		SI_Quad(SI_Quad& inp);
		SI_Quad(R3 nin, R3 pin, float din);
		SI_Quad& operator=(const SI_Quad& inp);
		void setAs(SI_Quad inp);
		void setAs(R3 nin, R3 pin, float din);
		int RayIntersection(R3 from_point, R3 direction, R3& position_hit, float& time_hit); //formerly RayHitAQuad

	};

	class SI_Cube
	{
	public:
		R3 pos;
		float size;
		SI_Cube();
		SI_Cube(R3 ina, float sizein);
		SI_Cube(const SI_Cube & inp);
		SI_Cube getCopy();
		SI_Cube& operator=(const SI_Cube& inp);
		void setAs(const SI_Cube & inp);
		void setAs(R3 ina, float sizein);
		float RayIntersection(R3 from_point, R3 direction, int& wasHit); //formerly rayhitcube, returns time value where hit
		R3 RayIntersection2(R3 from_point, R3 direction, int& wasHit); //formerly rayhitcube, returns time value where hit
		int containsPoint(R3 point); //formerly pointInCube
		int RayHitsSquare(int index, R3 P);
		SI_Quad getQuad(int index);
		R3 get_center();
	};

	
	class Object_File_Reader
	{
	public:
		std::vector<LINCOLN::R3>* vert_List;
		std::vector<SI_Triangle>* tri_List;
		Object_File_Reader(char* fname)
		{
			vert_List = new std::vector<LINCOLN::R3>;
			tri_List = new std::vector<SI_Triangle>;
			generate_object(fname);

		}
		~Object_File_Reader()
		{
			delete vert_List, tri_List;
		}

		void error(char* error_name)
		{
			std::cout << error_name << std::endl;
			system("PAUSE");
			exit(5);
		}

		void readFile(char* fname, std::string & file_out)
		{
			FILE* fi = fopen(fname, "r");
			if(fi == NULL)
			{
				error("FILE COULD NOT BE LOCATED\n");
			}
			char buf[10000];
			int max_len = 9999;
			while(1)
			{
				int bytes_read = fread(buf, 1, max_len, fi);

				if(bytes_read > 0)
				{
					buf[bytes_read] = NULL;
					file_out += buf;
				}
				if(feof(fi))
				{
					break;
				}
				//std::cout << file_out.len << std::endl;
			}

			fclose(fi);

			int ind = 0;

		}

		void get_line(std::string file, int& index, std::string & st_out)
		{
			st_out = "";
			char bu[1000];
			int c = 0;
			for(; index < file.size(); index++)
			{
				if(file[index] == '\n')
				{
					break;
				}
				//st_out += file[index];
				bu[c] = file[index];
				c++;
			}
			bu[c] = NULL;
			st_out += bu;
			index++;
		}

		void split_(std::string & st_in, char split_char, std::vector<std::string>& vec)
		{
			std::string accu;
			int last_was = 0;
			for(int i = 0; i < st_in.size(); i++)
			{
				if(st_in[i] == split_char && last_was == 0)
				{
					vec.push_back(accu);
					accu = "";
					last_was = 1;
					continue;
				}
				accu += st_in[i];
				last_was = 0;
			}

			vec.push_back(accu);
			
		
		}

		void generate_object(char* f_name)
		{
			//std::cout << "going to read the file\n";
			std::string fi;
			int index = 0;
			readFile(f_name, fi);
			std::cout << "read file\n";
			//std::cout << fi << std::endl;

			while(1)
			{
				std::string line;
				get_line(fi, index, line);
				//continue;
				if(line.size() > 3)
				{
				
					if(line[0] == 'v' && line[1] == ' ')
					{
						std::vector<std::string> strings;
						std::string line_in = line.substr(2);
						this->split_(line_in, ' ', strings);
						if(strings.size() >= 3)
						{
							float x = 0.0f,y = 0.0f,z = 0.0f, u;
							sscanf(strings.at(0).c_str(), "%f", &x);
							sscanf(strings.at(1).c_str(), "%f", &y);
							sscanf(strings.at(2).c_str(), "%f", &z);
							vert_List->push_back(LINCOLN::R3(x,y,z));
							//std::cout << "adding: " << vert_List->at(vert_List->size()-1) << std::endl;
						}
						else
						{
							error("ERROR IN Object_File_Reader.generate_object() - VERTEX COULD NOT BE READ PROPERLY");
						}
					}
					else if(line[0] == 'f' && line[1] == ' ')
					{

						std::vector<std::string> strings;
						std::string line_in = line.substr(2);
						//std::cout << line_in << std::endl;
						//system("pause");
						this->split_(line_in, ' ', strings);
						if(strings.size() == 3)
						{
							//std::cout << strings[0] << std::endl;
							//std::cout << strings[1] << std::endl;
							//std::cout << strings[2] << std::endl;
							//system("pause");
							int x = 0,y = 0,z = 0, u;
							/*sscanf(strings.at(0).c_st, "%i", &x);
							sscanf(strings.at(1).c_st, "%i", &y);
							sscanf(strings.at(2).c_st, "%i", &z);*/

							sscanf(strings.at(0).c_str(), "%i//%i", &x, &u);
							sscanf(strings.at(1).c_str(), "%i//%i", &y, &u);
							sscanf(strings.at(2).c_str(), "%i//%i", &z, &u);

							SI_Triangle newTri(x-1,y-1,z-1);
							tri_List->push_back(newTri);
							//std::cout << "adding T: (" << newTri.a << ", " << newTri.b << ", " << newTri.c << ")" << std::endl;
						}
						else
						{
							error("ERROR IN Object_File_Reader.generate_object() - TRIANGLE INFORMATION COULD NOT BE READ PROPERLY");
						}
					}



				}

			
				//std::cout << "line: " << line << std::endl;

				if(index >= fi.size())
				{
					break;
				}

			
			}

		
		}
	};

	template <class T>
	class LStackNode
	{
	public:
		T data;
		LStackNode * next;
		LStackNode()
		{
			next = NULL;
		}
		LStackNode(T ndata)
		{
			data = ndata;
			next = NULL;
		}
		LStackNode(T ndata, LStackNode * nextIn)
		{
			data = ndata;
			next = nextIn;
		}
		LStackNode(LStackNode & input)
		{
			next = input.next;
			data = input.data;
		}
	};

template <class T>
	class LStack
	{
	public:
		LStackNode<T> * head;
		int len;
		LStack<T>()
		{
			len = 0;
			head = NULL;
		}
		~LStack<T>()
		{
			while( len != 0 )
			{
				pop();
			}
		}
		void error(char * err)
		{
			std::cout << err << std::endl;
			system("PAUSE");
			exit(-1);
		}
		void add(T data)
		{
			if(len == 0)
			{
				head = new LStackNode<T>(data);
			}else
			{
				LStackNode<T> * nn = new LStackNode<T>(data, head);
				head = nn;
			}
			len++;
		}
		T pop()
		{
			if(len == 0)
			{
				error("ERROR IN LSTACK - cannot pop from an empty stack");
				return 0;
			}
			T rv = head->data;
			LStackNode<T> * tmp = head;
			head = head->next;
			delete tmp;
			len--;
			return rv;
		}

		void operator += (T data)
		{
			add(data);
		}

		T & get_reference(int index)
		{
			if(index < 0 || index >= len)
			{
				error("index out of range");
			}
			LStackNode<T> * p = head;
			for(int i = 0; i < index; i++)
			{
				p = p->next;
			}
			return p->data;
		}

		T & operator [] (int index)
		{
			return get_reference(index);
		}
	};


	class R3SetElement
	{
	public:
		R3 point;
		R3SetElement()
		{
			point = 0.0f;
		}
		R3SetElement(R3 input)
		{
			point = input;
		}
		R3SetElement(R3SetElement & input)
		{
			point = input.point;
		}
		R3SetElement & operator = (R3SetElement & input)
		{
			point = input.point;
			return *this;
		}
		R3SetElement & operator = (R3 & input)
		{
			point = input;
			return *this;
		}
		void setAs(R3 input)
		{
			point = input;
		}
		int operator<(R3SetElement & input)
		{
			if(point == input.point)
			{
				return 0;
			}
			if(point.x < input.point.x)
			{
				return 1;
			}else if(point.x == input.point.x)
			{
				if(point.y < input.point.y)
				{
					return 1;
				}else if(point.y == input.point.y)
				{
					if(point.z < input.point.z)
					{
						return 1;
					}
				}
			}
			return 0;
		}
		int operator==(R3SetElement & input)
		{
			return (input.point == point);
		}
		int operator>(R3SetElement & input)
		{
			return (!((*this) == input)) && (!((*this) < input));
		}
	};

	

	class R3DistElement
	{
	public:
		R3 point;
		float distance;
		int PointIndex;
		R3DistElement(R3 p1, int ind, R3 classifier)
		{
			point = p1;
			distance = (p1-classifier).Magnitude();
			PointIndex = ind;
		}
		R3DistElement()
		{
			point = 0.0f;
			distance = 0.0f;
			PointIndex = 0;
		}
		R3DistElement(R3DistElement & inp)
		{
			point = inp.point;
			distance = inp.distance;
			PointIndex = inp.PointIndex;
		}
		R3DistElement & operator = (R3DistElement & inp)
		{
			point = inp.point;
			distance = inp.distance;
			PointIndex = inp.PointIndex;
			return *this;
		}
		int operator == (R3DistElement & inp)
		{
			return (inp.point == point);
		}
		int operator < (R3DistElement & inp)
		{
			return (distance < inp.distance) || ((distance == inp.distance) && !(point == inp.point));
		}
		int operator > (R3DistElement & inp)
		{
			return (!((*this) == inp)) && (!((*this) < inp));
		}
		
	};
	
	template <class T>
	class LTopList
	{
	public:
		T * list;
		int len;
		int NE;
		LTopList(int NE_in = 10)
		{
			NE = NE_in;
			list = new T[NE];
			len = 0;
		}

		LTopList(LTopList & inp)
		{
			NE = inp.NE;
			list = new T[NE];
			len = inp.len;
			for(int i = 0; i < len; i++)
			{
				list[i] = inp.list[i];
			}
		}

		LTopList & operator = (LTopList & inp)
		{
			delete [] list;
			NE = inp.NE;
			list = inp.list;
			len = inp.len;
			return * this;
		}

		~LTopList()
		{
			delete [] list;
		}

		void insert(int index, T data)
		{
			int from = (len == NE)? len - 1 : len;
			for( ; from > index; from--)
			{
				list[from] = list[from-1];
			}
			list[index] = data;
			if(len < NE)
			{
				len++;
			}
		}
		int findLocation(T data)
		{
			if(len == 0)
			{
				return 0;
			}
			if(data > list[len-1])
			{
				if(len == NE || data == list[len-1])
				{
					return -1;
				}else
				{
					return len;
				}
			}
			for(int i = 0; i < len; i++)
			{
				if(data < list[i])
				{
					return i;
				}else if(data == list[i])
				{
					return -1;
				}
			}
			return -1;
		}
		void add(T data)
		{
			int i = findLocation(data);
			if(i == len)
			{
				list[i] = data;
				len++;
			}else if(i != -1)
			{
				insert(i, data);
				
			}
		}
		void operator += (T d)
		{
			add(d);
		}
		T & get_ref(int index)
		{
			return list[index];
		}
		T & operator [] (int index)
		{
			return list[index];
		}
	};

	template <class T>
	class LArrList
	{
	public:
		T * data;
		int len, blen;
		LArrList<T>()
		{
			blen = 100;
			len = 0;
			data = (T*) malloc(sizeof(T) * blen);
		}
		LArrList<T>(LArrList<T> & listIn)
		{
			len = listIn.len;
			blen = listIn.blen;
			data = (T *) malloc(sizeof(T) * blen);
			for(int i = 0; i < len; i++)
			{
				data[i] = listIn.data[i];
			}
		}
		LArrList<T> & operator = (LArrList<T> & listIn)
		{
			free(data);
			len = listIn.len;
			blen = listIn.blen;
			data = (T *) malloc(sizeof(T) * blen);
			for(int i = 0; i < len; i++)
			{
				data[i] = listIn.data[i];
			}
			return *this;
		}
		~LArrList()
		{
			free(data);
		}
		void add(T newItem)
		{
			if(len == blen-1)
			{
				inc_buffer();
			}
			data[len] = newItem;
			len++;
		}
		void inc_buffer()
		{
			blen *= 2;
			data = (T *)realloc(data, sizeof(T) * blen);
		}
		T & get_ref(int index)
		{
			return data[index];
		}
		void operator += (T newItem)
		{
			add(newItem);
		}
		T & operator [] (int index)
		{
			return get_ref(index);
		}

		void swap(int ind1, int ind2)
		{
			T v = data[ind1];
			data[ind1] = data[ind2];
			data[ind2] = v;
		}

		int partition(int first, int last)
		{
			T pivot;
			int index, smallIndex;
			swap(first, (first+last)/2);
			pivot = data[first];
			smallIndex = first;
			for(index = first + 1; index <= last; index++)
			{
				if(data[index] < pivot)
				{
					smallIndex++;
					swap(smallIndex, index);
				}
			}
			swap(first, smallIndex);

			return smallIndex;
	
		}

		void qsort()
		{
			LStack<int> fs;
			LStack<int> ls;
			fs += 0;
			ls += (len-1);
			while(fs.len != 0)
			{
				int f = fs.pop();
				int l = ls.pop();
				if(f < l)
				{
					int pivL = partition(f, l);
					fs += pivL + 1;
					ls += l;
					fs += f;
					ls += pivL - 1;
				}
			}
		}

		void reduceToOrderedSet()
		{
			if(len == 0)
			{
				return;
			}
			LArrList<T> cpl;
			qsort();
			cpl += data[0];
			T lastV = data[0];
			for(int i = 1; i < len; i++)
			{
				if(!(lastV == data[i]))
				{
					lastV = data[i];
					cpl += data[i];
				}
			}
			(*this) = cpl;
		}

		int binarySearch(T item)
		{
			int first = 0;
			int last = len - 1;
			int mid;
			while(first <= last)
			{
				mid = (first + last) / 2;
				if(data[mid] == item)
				{
					return mid;
				}else
				{
					if(item < data[mid])
					{
						last = mid - 1;
					}else
					{
						first = mid + 1;
					}
				}
			}
			return -1;
		}

	};

	class SIObj
	{
	public:
		int numVerts;
		int numTriangles;
		R3* vertList;
		SI_Triangle* triangleList;
		R3* normalList;
		SIObj();
		SIObj(int numV, int numT);
		SIObj(std::vector<R3> pointList);
		SIObj(SIObj & obj_in);
		SIObj(SIObj * obj_in);
		SIObj & operator = (SIObj & obj_in);
		SIObj(const char* fname);

		SIObj(Object_File_Reader& object_reader);
		~SIObj();

		SI_FullTriangle getTriangle(int index);
		SI_FullTriangle operator[](int index);
		R3 getCenterOFTriangle(int index);
		
		float getAvgNormal(int index, SI_Cube cu, int& numHits);

		LINCOLN::R3 getAvgNormalR3(int index, SI_Cube cu, int& numHits);
		
		float getCornerValue(int index, SI_Cube cu, int direction, int corner, int* wasHit);
		
		float calculate_MSE_from_SOTPlane_to_Mesh(int index, SI_Cube cu, int direction, float da, float db, float dc, float dd, int& numHits);
		
		R3 getCornerPoint(SI_Cube cu, int direction, int corner);
		void savePLY(std::string fname);
		void saveOBJ(std::string fname);
		void save(std::string fname);
		int getClosestMatchingAxis(R3 v);

		void addTriangles(std::vector<SI_Triangle> & triList);

		SIObj getMinimalVJS();
		void mergeCloseVertices(float distance_threshold);


		R3 minimal_vertex_dimensions();
		float minDist();
		R3 maximal_vertex_dimensions();
		float maxDist();

		void addToEachPoint(float a);
		void addToEachPointR3(R3 p);
		void subToEachPoint(float a);
		void subToEachPointR3(R3 p);
		void scaleEachPoint(float a);
		void scaleEachPointR3(R3 p);
		void divEachPoint(float a);
		void divEachPointR3(R3 p);


		float normalize(float largestDist);
		void unnormalize(float undo);

		R3 toOrigin();

		

		R3 ew_normalize(float largestDist);
		void ew_unnormalize(R3 undo);

	};


	


	class SimplePointCloudObject
	{
	public:
		R3* list;
		int len, blen;
		SimplePointCloudObject();
		SimplePointCloudObject(char* fname);
		SimplePointCloudObject(char* fname, int a);
		SimplePointCloudObject(SIObj& obj);
		SimplePointCloudObject(SIObj* obj);
		~SimplePointCloudObject();
		void inc_buffer();
		void save_obj_file(char* name);
		void add(R3 point);
		void add_set(R3 point);
		void add(SI_FullTriangle tri);
		void add_set(SI_FullTriangle tri);
		void operator+=(R3 point);
		void operator+=(SI_FullTriangle tri);
		R3 operator[](int index);
		R3 get(int index);
		void savePCD(char* name);
		int roundF(float fin);
		int containsPoint(R3 p);
		
		
	};

}

#endif