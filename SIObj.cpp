#include "SIObj.h"

#include <stdio.h>
#include <stdlib.h>

namespace SIMPLE_OBJECT_LINCOLN{

	SI_Triangle::SI_Triangle()
	{
		a = 0;
		b = 0;
		c = 0;
	}
	SI_Triangle::SI_Triangle(int ina, int inb, int inc)
	{
		setAs(ina, inb, inc);
	}
	SI_Triangle::SI_Triangle(const SI_Triangle& inp)
	{
		setAs(inp);
	}
	SI_Triangle SI_Triangle::getCopy()
	{
		SI_Triangle rv = (*this);
		return rv;
	}
	SI_Triangle& SI_Triangle::operator = (const SI_Triangle& inp)
	{
		a = inp.a;
		b = inp.b;
		c = inp.c;
		return (*this);
	}
	void SI_Triangle::setAs(const SI_Triangle& inp)
	{
		a = inp.a;
		b = inp.b;
		c = inp.c;
	}
	void SI_Triangle::setAs(int ina, int inb, int inc)
	{
		a = ina;
		b = inb;
		c = inc;
	}

	int SI_Triangle::operator == (SI_Triangle inp)
	{
		SI_Triangle tc = *this;
		tc.order();
		inp.order();
		if(tc.a == inp.a && tc.b == inp.b && tc.c == inp.c)
		{
			return 1;
		}
		return 0;
	}
	int SI_Triangle::operator > (SI_Triangle inp)
	{
		if((*this) == inp || (*this) < inp)
		{
			return 0;
		}
		return 1;
	}
	int SI_Triangle::operator < (SI_Triangle inp)
	{
		if(*this == inp)
		{
			return 0;
		}else
		{
			SI_Triangle cp = *this;
			cp.order();
			inp.order();
			if(cp.a < inp.a)
			{
				return 1;
			}else if(cp.a == inp.a)
			{
				if(cp.b < inp.b)
				{
					return 1;
				}else if(cp.b == inp.b)
				{
					if(cp.c < inp.c)
					{
						return 1;
					}
				}
			}
			return 0;
		}
	}

	void SI_Triangle::order()
	{
		int lis[3] = {a,b,c};
		int smi = 0;
		smi = (lis[smi] < lis[1]) ? smi : 1;
		smi = (lis[smi] < lis[2]) ? smi : 2;
		int tmp = lis[0];
		lis[0] = lis[smi];
		lis[smi] = tmp;
		smi = 1;
		smi = (lis[smi] < lis[2]) ? smi : 2;
		tmp = lis[1];
		lis[1] = lis[smi];
		lis[smi] = tmp;
		a = lis[0];
		b = lis[1];
		c = lis[2];
	}

	SI_FullTriangle::SI_FullTriangle()
	{
		setAs(R3(), R3(), R3());
	}
	SI_FullTriangle::SI_FullTriangle(R3 ina, R3 inb, R3 inc)
	{
		setAs(ina, inb, inc);
	}
	SI_FullTriangle::SI_FullTriangle(const SI_FullTriangle& inp)
	{
		setAs(inp.a, inp.b, inp.c);
	}
	SI_FullTriangle& SI_FullTriangle::operator=(const SI_FullTriangle& inp)
	{
		setAs(inp.a, inp.b, inp.c);
		return (*this);
	}
	SI_FullTriangle SI_FullTriangle::getCopy()
	{
		SI_FullTriangle rv = *this;
		return rv;
	}
	void SI_FullTriangle::setAs(SI_FullTriangle inp)
	{
		setAs(inp.a, inp.b, inp.c);
	}
	void SI_FullTriangle::setAs(R3 ina, R3 inb, R3 inc)
	{
		a = ina;
		b = inb;
		c = inc;
	}
	int SI_FullTriangle::pointInsideTriangle(R3 point, R3 normal){

		R3 Ns[3];
		float chq[3];
		normal = normal.getNormalizedCopy();
		Ns[0] = ((b - a) ^ normal);
		chq[0] = (point * Ns[0]) - (Ns[0] * a);

		Ns[1] = ((c - b) ^ normal);
		chq[1] = (point * Ns[1]) - (Ns[1] * b);


		Ns[2] = ((a - c) ^ normal);
		chq[2] = (point * Ns[2]) - (Ns[2] * c);

		if ((chq[0] >= 0.0f && chq[1] >= 0.0f && chq[2] >= 0.0f) ||
			(chq[0] <= 0.0f && chq[1] <= 0.0f && chq[2] <= 0.0f))
		{
			return 1;
		}

		return 0;

		/*R3 Ns[3];
		float chq[3];
		Ns[0] = ((b - a) ^ ((a + normal) - a));
		chq[0] = (point * Ns[0]) - (Ns[0] * a);

		Ns[1] = ((c - b) ^ ((b + normal) - b));
		chq[1] = (point * Ns[1]) - (Ns[1] * b);


		Ns[2] = ((a - c) ^ ((c + normal) - c));
		chq[2] = (point * Ns[2]) - (Ns[2] * c);

		if ((chq[0] >= 0.0f && chq[1] >= 0.0f && chq[2] >= 0.0f) ||
			(chq[0] <= 0.0f && chq[1] <= 0.0f && chq[2] <= 0.0f)){
			return 1;
		}

		return 0;
		*/
	}

	float SI_FullTriangle::RayIntersectsTriangle(R3 S, R3 V, R3 N, R3& intersectionPoint, int& hit){

		float Bop = N * V;
		if (Bop == 0.0f)
		{
			hit = 0;
			intersectionPoint = R3();
			return 0.0f;
		}
		float D = -(N * a);
		float t = -(((N * S) + D) / Bop);
		intersectionPoint = (S + ((V.getNormalizedCopy()) * t));
		if (this->pointInsideTriangle(intersectionPoint, N) == 1)
		{
			hit = 1;
			return t;
		}
		else
		{
			hit = 0;
			intersectionPoint = R3();
			return 0.0f;

		}

	}


	SI_Quad::SI_Quad()
	{
		normal = R3();
		point = R3();
		distance = 0.0f;
	}
	SI_Quad::SI_Quad(SI_Quad& inp)
	{
		setAs(inp);
	}
	SI_Quad::SI_Quad(R3 nin, R3 pin, float din)
	{
		setAs(nin, pin, din);
	}
	SI_Quad& SI_Quad::operator=(const SI_Quad& inp)
	{
		this->setAs(inp.normal, inp.point, inp.distance);
		return (*this);
	}
	void SI_Quad::setAs(SI_Quad inp)
	{
		normal = inp.normal;
		distance = inp.distance;
		point = inp.point;
	}
	void SI_Quad::setAs(R3 nin, R3 pin, float din)
	{
		normal = nin;
		point = pin;
		distance = din;
	}
	int SI_Quad::RayIntersection(R3 from_point, R3 direction, R3& position_hit, float& time_hit)
	{
		float b = normal * direction;
		if (b == 0.0f)
		{
			return 0;
		}
		float ab = -((normal * from_point) + distance);
		time_hit = ab / b;
		position_hit = (from_point + ((direction.getNormalizedCopy()) * time_hit));
		return 1;
	}

	SI_Cube::SI_Cube()
	{
		pos = R3();
		size = 0.0f;
	}
	SI_Cube::SI_Cube(R3 ina, float sizein)
	{
		this->pos = ina;
		this->size = sizein;
	}
	SI_Cube::SI_Cube(const SI_Cube& inp)
	{
		setAs(inp.pos,inp.size);
	}
	SI_Cube SI_Cube::getCopy()
	{
		SI_Cube rv(*this);
		return rv;
	}
	SI_Cube& SI_Cube::operator=(const SI_Cube& inp)
	{
		setAs(inp.pos, inp.size);
		return (*this);
	}
	void SI_Cube::setAs(const SI_Cube& inp)
	{
		setAs(inp.pos, inp.size);
	}
	void SI_Cube::setAs(R3 ina, float sizein)
	{
		pos = ina;
		size = sizein;
	}
	int SI_Cube::RayHitsSquare(int index, R3 P)
	{
		if (index == 0 || index == 2){
			if (P.x >= pos.x && P.x <= pos.x + size && P.y >= pos.y && P.y <= pos.y + size){
				return 1;
			}
			else{
				return 0;
			}
		}

		if (index == 1 || index == 3){
			if (P.z >= pos.z && P.z <= pos.z + size && P.y >= pos.y && P.y <= pos.y + size){
				return 1;
			}
			else{
				return 0;
			}
		}

		if (index == 4 || index == 5){
			if (P.x >= pos.x && P.x <= pos.x + size && P.z >= pos.z && P.z <= pos.z + size){
				return 1;
			}
			else{
				return 0;
			}
		}
	}
	SI_Quad SI_Cube::getQuad(int index)
	{
		R3 N = R3(0.0f, 0.0f, 0.0f);
		R3 P = pos;
		float D = 0.0f;
		if (index == 0){
			N = R3(0.0f, 0.0f, -1.0f);
		}
		else if (index == 1){
			P.x += size;
			N = R3(1.0f, 0.0f, 0.0f);
		}
		else if (index == 2){
			N = R3(0.0f, 0.0f, 1.0f);
			P.z += size;
		}
		else if (index == 3){
			N = R3(-1.0f, 0.0f, 0.0f);
		}
		else if (index == 4){
			P.y += size;
			N = R3(0.0f, 1.0f, 0.0f);
		}
		else{
			N = R3(0.0f, -1.0f, 0.0f);
		}

		D = -(N * P);
		SI_Quad rq = SI_Quad(P, N, D);
		return rq;
	}
	float SI_Cube::RayIntersection(R3 from_point, R3 direction, int& wasHit)
	{
		float hitTs[2];

		int hitCount = 0;

		if (containsPoint(from_point) == 0)
		{
			wasHit = 0;
			return 0.0f;
		}

		for (int i = 0; i < 6; i++)
		{
			SI_Quad aqu = this->getQuad(i);
			R3 outPos = R3(-1000.0f, 0.0f, 0.0f);
			float time_hit;
			int wh = aqu.RayIntersection(from_point, direction, outPos, time_hit);

			if (!wh)
			{
				continue;
			}

			int hit = this->RayHitsSquare(i, outPos);
			if (hit == 1){
				hitTs[hitCount] = time_hit;
				hitCount++;
			}
			if (hitCount == 2){
				break;
			}

		}



		if (hitCount == 2){


			//printf("Correct Face TypeB\n");
			if (hitTs[1] > 0.0f){
				wasHit = 1;
				return hitTs[1];
			}
			else if (hitTs[0] > 0.0f){
				wasHit = 1;
				return hitTs[0];
			}



		}/*else{
		 printf("Inside Cube, but something went wrong\n");
		 }*/

		wasHit = 0;
		return 0.0f;
	}
	R3 SI_Cube::RayIntersection2(R3 from_point, R3 direction, int& wasHit)
	{
		float hitTs[2];
		R3 hitVs[2];

		int hitCount = 0;

		if (containsPoint(from_point) == 0){
			wasHit = 0;
			return R3(0.0f, 0.0f, 0.0f);
		}

		for (int i = 0; i < 6; i++){
			SI_Quad aqu = this->getQuad(i);
			R3 outPos = R3(-1000.0f, 0.0f, 0.0f);
			float t = 0.0f;
			int wh = aqu.RayIntersection(from_point, direction, outPos, t);

			if (!wh){
				continue;
			}

			int hit = this->RayHitsSquare(i, outPos);
			if (hit == 1){
				hitTs[hitCount] = t;
				hitVs[hitCount] = outPos;
				hitCount++;
			}
			if (hitCount == 2){
				break;
			}

		}



		if (hitCount == 2){


			//printf("Correct Face TypeB\n");
			if (hitTs[1] > 0.0f){
				wasHit = 1;
				return hitVs[1];
			}
			else if (hitTs[0] > 0.0f){
				wasHit = 1;
				return hitVs[0];
			}



		}/*else{
		 printf("Inside Cube, but something went wrong\n");
		 }*/

		wasHit = 0;
		return R3(0.0f, 0.0f, 0.0f);

	}
	int SI_Cube::containsPoint(R3 point)
	{
		if (point.x >= pos.x && point.x <= pos.x + size &&
			point.y >= pos.y && point.y <= pos.y + size &&
			point.z >= pos.z && point.z <= pos.z + size){
			return 1;
		}

		return 0;
	}
	R3 SI_Cube::get_center()
	{
		float hs = size * 0.5f;
		return pos + R3(hs, hs, hs);
	}

	SIObj::SIObj()
	{
		numVerts = 1;
		numTriangles = 1;
		this->vertList = new R3[numVerts];
		this->triangleList = new SI_Triangle[numTriangles];
		this->normalList = new R3[numTriangles];

	}

	SIObj::SIObj(int numV, int numT)
	{
		numVerts = numV;
		numTriangles = numT;
		if(numVerts > 0)	this->vertList = new R3[numVerts];
		if(numTriangles > 0)
		{
			this->triangleList = new SI_Triangle[numTriangles];
			this->normalList = new R3[numTriangles];
		}
	}

	SIObj::SIObj(std::vector<R3> pointList)
	{
		numVerts = pointList.size();
		numTriangles = 0;
		if(numVerts > 0)	this->vertList = new R3[numVerts];
		for(int i = 0; i < pointList.size(); i++)
		{
			vertList[i] = pointList[i];
		}
		
	}

	SIObj::SIObj(SIObj & obj_in)
	{
		numVerts = obj_in.numVerts;
		numTriangles = obj_in.numTriangles;
		this->vertList = new R3[numVerts];
		this->triangleList = new SI_Triangle[numTriangles];
		this->normalList = new R3[numTriangles];
		for(int i = 0; i < numVerts; i++)
		{
			vertList[i] = obj_in.vertList[i];
		}

		for(int i = 0; i < numTriangles; i++)
		{
			triangleList[i] = obj_in.triangleList[i];
			this->normalList[i] = obj_in.normalList[i];
		}
	}

	SIObj::SIObj(SIObj * obj_in)
	{
		numVerts = obj_in->numVerts;
		numTriangles = obj_in->numTriangles;
		this->vertList = new R3[numVerts];
		this->triangleList = new SI_Triangle[numTriangles];
		this->normalList = new R3[numTriangles];
		for(int i = 0; i < numVerts; i++)
		{
			vertList[i] = obj_in->vertList[i];
		}

		for(int i = 0; i < numTriangles; i++)
		{
			triangleList[i] = obj_in->triangleList[i];
			this->normalList[i] = obj_in->normalList[i];
		}
	}

	SIObj & SIObj::operator = (SIObj & obj_in)
	{
		delete [] vertList;
		delete [] triangleList;
		delete [] normalList;
		numVerts = obj_in.numVerts;
		numTriangles = obj_in.numTriangles;
		this->vertList = new R3[numVerts];
		this->triangleList = new SI_Triangle[numTriangles];
		this->normalList = new R3[numTriangles];
		for(int i = 0; i < numVerts; i++)
		{
			vertList[i] = obj_in.vertList[i];
		}

		for(int i = 0; i < numTriangles; i++)
		{
			triangleList[i] = obj_in.triangleList[i];
			this->normalList[i] = obj_in.normalList[i];
		}
		return *this;
	}

	SIObj::SIObj(Object_File_Reader& object_reader)
	{
		//read in the number of vertices and triangles
		numVerts = object_reader.vert_List->size();
		numTriangles = object_reader.tri_List->size();
		

		//print these quantities out
		//printf("No. Vertices: %i\n", this->numVerts);
		//printf("No. Triangles: %i\n", this->numTriangles);

		this->vertList = new R3[numVerts];
		this->triangleList = new SI_Triangle[numTriangles];
		this->normalList = new R3[numTriangles];

		//read the vertices
		for (int i = 0; i < this->numVerts; i++)
		{
			this->vertList[i] = R3(object_reader.vert_List->at(i));
		}

		//read the triangles and calculate their normals
		for (int i = 0; i < this->numTriangles; i++)
		{
			this->triangleList[i] = SI_Triangle(object_reader.tri_List->at(i));

			SI_FullTriangle ft = this->getTriangle(i);
			R3 atmp = ft.b - ft.a;
			R3 btmp = ft.c - ft.b;
			//this->normalList[i] = !((atmp ^ btmp));//inversion for bunny
			this->normalList[i] = (atmp ^ btmp).getNormalizedCopy();

		}
	}

	SIObj::SIObj(const char* fname)
	{
		FILE* fi = fopen(fname, "r");

		this->numVerts = this->numTriangles = 0; //setup initial numbers as 0

		if (fi == NULL) //if could not open file, let the programmer know!
		{
			printf("BAD FILE NAME\n");
			system("PAUSE");
			return;
		}




		//read in the number of vertices and triangles
		fscanf(fi, "%i\n", &this->numVerts);
		fscanf(fi, "%i\n", &this->numTriangles);


		//print these quantities out
		//printf("No. Vertices: %i\n", this->numVerts);
		//printf("No. Triangles: %i\n", this->numTriangles);

		this->vertList = new R3[numVerts];
		this->triangleList = new SI_Triangle[numTriangles];
		this->normalList = new R3[numTriangles];

		int tmpa = 0, tmpb = 0, tmpc = 0;
		float tmpafloat = 0.0f, tmpbfloat = 0.0f, tmpcfloat = 0.0f;
		//read the vertices
		for (int i = 0; i < this->numVerts; i++)
		{

			fscanf(fi, "%f %f %f\n", &tmpafloat, &tmpbfloat, &tmpcfloat);

			this->vertList[i] = R3(tmpafloat, tmpbfloat, tmpcfloat);

		}

		//read the triangles and calculate their normals
		for (int i = 0; i < this->numTriangles; i++)
		{
			fscanf(fi, "%i %i %i\n", &tmpa, &tmpb, &tmpc);
			this->triangleList[i] = SI_Triangle(tmpa, tmpb, tmpc);

			SI_FullTriangle ft = this->getTriangle(i);
			R3 atmp = ft.b - ft.a;
			R3 btmp = ft.c - ft.b;
			this->normalList[i] = (!((atmp ^ btmp))).getNormalizedCopy();//inversion for bunny
			//this->normalList[i] = (atmp ^ btmp);

		}



		fclose(fi);
	}


	SIObj::~SIObj()
	{
		if (this->numVerts > 0)
		{
			delete [] this->vertList;
		}
		if (this->numTriangles > 0)
		{
			delete [] this->triangleList;
			delete [] this->normalList;
		}
	}

	void SIObj::save(std::string fname)
	{
		FILE* fi = fopen(fname.c_str(), "w");

		fprintf(fi, "%i\n%i\n", numVerts, numTriangles);
		for(int i = 0; i < numVerts; i++)
		{
			fprintf(fi, "%f %f %f\n", vertList[i].x, vertList[i].y, vertList[i].z);
		}
		for(int i = 0; i < numTriangles; i++)
		{
			fprintf(fi, "%i %i %i\n", triangleList[i].a, triangleList[i].b, triangleList[i].c);
		}



		fclose(fi);
	}

	void SIObj::mergeCloseVertices(float distance_threshold)
	{
		int* done = new int[numVerts];
		memset(done, 0, sizeof(int) * numVerts);
		float recent = 0.0f;
		for(int j = 0; j < numVerts; j++)
		{
			float nr = j/(float)numVerts;
			if(nr - recent > 0.01f)
			{
				std::cout << (nr) << std::endl;
				recent = nr;
			}
			if(done[j])
			{
				continue;
			}
			for(int i = j+1; i < numVerts; i++)
			{
				//if(j == i)
				//{
					//continue;
				//}
				if(done[i])
				{
					continue;
				}
				R3 a2b = vertList[i] - vertList[j];
				float dist = a2b.Magnitude();
				if(dist < distance_threshold)
				{
					//a2b *= 0.5f;
					//vertList[j] = vertList[i];
					vertList[i] = vertList[j];
					done[i] = 1;
					done[j] = 1;
				}
			}
		}
	}

	

	SIObj SIObj::getMinimalVJS()
	{
		LArrList<R3SetElement> mvl;
		for(int i = 0; i < this->numVerts; i++)
		{
			mvl += R3SetElement(vertList[i]);
		}
		
		mvl.reduceToOrderedSet();

		
		SI_Triangle* ntl = new SI_Triangle[this->numTriangles];
		int tcount = 0;
		for(int t = 0; t < this->numTriangles; t++)
		{
			SI_Triangle tri = this->triangleList[t];
			int counter = 0;
			int outa, outb, outc;
			outa = mvl.binarySearch(R3SetElement(vertList[tri.a]));
			outb = mvl.binarySearch(R3SetElement(vertList[tri.b]));
			outc = mvl.binarySearch(R3SetElement(vertList[tri.c]));
			if(outa == -1 || outb == -1 || outc == -1 || outa == outb || outa == outc || outb == outc)
			{
				//std::cout << "Got a bad triangle" << std::endl;
				continue;
			}
			ntl[tcount] = SI_Triangle(outa, outb, outc);
			tcount++;
		}
		//std::cout << "copied " << tcount << " triangles \n"; 
	//	std::cout << "copied triangle indices\n";

		SIObj rv(mvl.len, tcount);

		for(int i = 0; i < mvl.len; i++)
		{
			rv.vertList[i] = mvl[i].point;
		}

		//std::cout << "copy to vertlist\n";

		for(int i = 0; i < tcount; i++)
		{
			rv.triangleList[i] = ntl[i];
			SI_FullTriangle ft = rv.getTriangle(i);
			R3 atmp = ft.b - ft.a;
			R3 btmp = ft.c - ft.b;
			rv.normalList[i] = (atmp ^ btmp);
		}

		//std::cout << "copy to trilist\n";

		delete [] ntl;
		return rv;
	}



	SI_FullTriangle SIObj::getTriangle(int index){
		SI_FullTriangle rv = SI_FullTriangle(this->vertList[this->triangleList[index].a], this->vertList[this->triangleList[index].b], this->vertList[this->triangleList[index].c]);
		return rv;
	}

	R3 SIObj::getCenterOFTriangle(int index){
		SI_FullTriangle ft = this->getTriangle(index);
		R3 tmpA = ft.a;
		ft.b = (ft.b - ft.a);
		ft.c = (ft.c - ft.a);
		ft.a = R3(0.0f, 0.0f, 0.0f);
		float mag = (ft.b.Magnitude()) / 2.0f;
		ft.b = ((ft.b.getNormalizedCopy()) * mag);
		R3 vec2 = (ft.c - ft.b);
		float mag2 = (vec2.Magnitude()) / 2.0f;
		vec2 = ((vec2.getNormalizedCopy()) * mag2);
		return ((vec2 + ft.b) + tmpA);

	}
	
	float SIObj::getAvgNormal(int index, SI_Cube cu, int& numHits){
		SI_FullTriangle ft = this->getTriangle(index);
		float q1, q2;
		R3 incV = ft.b - ft.a, incU;
		q1 = incV.Magnitude() / 2.0f;
		int toq1 = (int)(q1 + 0.5f), toq2;
		incV *= (1.0f / q1);
		R3 iter1 = ft.a, iter2;
		numHits = 0;
		float rval = 0.0f;
		float actualAngle = (float)getClosestMatchingAxis(normalList[index]);
		for(int j = 0; j < toq1; j++)
		{
			iter2 = iter1;
			incU = ft.c - iter2;
			q2 = incU.Magnitude() / 2.0f;
			toq2 = (int)(q2 + 0.5f);
			incU *= (1.0f / q2);
			for(int i = 0; i < q2; i++)
			{
				if(cu.containsPoint(iter2))
				{
					numHits += 1;
					
				
				}

				iter2 += incU;
			}

		
			iter1 += incV;
		}


		return actualAngle * (float)numHits;

	}
	

	R3 SIObj::getAvgNormalR3(int index, SI_Cube cu, int& numHits){
		SI_FullTriangle ft = this->getTriangle(index);
		float q1, q2;
		R3 incV = ft.b - ft.a, incU;
		q1 = incV.Magnitude() / 2.0f;
		int toq1 = (int)(q1 + 0.5f), toq2;
		incV *= (1.0f / q1);
		R3 iter1 = ft.a, iter2;
		numHits = 0;
		float rval = 0.0f;
		R3 actualAngle = normalList[index];
		for(int j = 0; j < toq1; j++)
		{
			iter2 = iter1;
			incU = ft.c - iter2;
			q2 = incU.Magnitude() / 2.0f;
			toq2 = (int)(q2 + 0.5f);
			incU *= (1.0f / q2);
			for(int i = 0; i < q2; i++)
			{
				if(cu.containsPoint(iter2))
				{
					numHits += 1;
					
				
				}

				iter2 += incU;
			}

		
			iter1 += incV;
		}


		return actualAngle * (float)numHits;

	}
	
	R3 SIObj::getCornerPoint(SI_Cube cu, int direction, int corner){
		R3 posFrom = R3(0.0f, 0.0f, 0.0f);
		if (direction == 0){
			posFrom = cu.pos;
			if (corner == 1){
				posFrom = (posFrom + R3(0.0f, 0.0f, cu.size));
			}
			else if (corner == 2){
				posFrom = (posFrom + R3(0.0f, cu.size, cu.size));
			}
			else if (corner == 3){
				posFrom = (posFrom + R3(0.0f, cu.size, 0.0f));
			}
		}
		else if (direction == 1){
			posFrom = cu.pos;
			if (corner == 1){
				posFrom = (posFrom + R3(cu.size, 0.0f, 0.0f));
			}
			else if (corner == 2){
				posFrom = (posFrom + R3(cu.size, 0.0f, cu.size));
			}
			else if (corner == 3){
				posFrom = (posFrom + R3(0.0f, 0.0f, cu.size));
			}
		}
		else{
			posFrom = cu.pos;
			if (corner == 1){
				posFrom = (posFrom + R3(cu.size, 0.0f, 0.0f));
			}
			else if (corner == 2){
				posFrom = (posFrom + R3(cu.size, cu.size, 0.0f));
			}
			else if (corner == 3){
				posFrom = (posFrom + R3(0.0f, cu.size, 0.0f));
			}
		}
		return posFrom;
	}

	float SIObj::getCornerValue(int index, SI_Cube cu, int direction, int corner, int* wasHit){

		SI_FullTriangle ft = this->getTriangle(index);
		int hit = 0;
		float t = 0.0f;
		*wasHit = 0;
		R3 interP = R3(0.0f, 0.0f, 0.0f);
		R3 dirVec = R3(0.0f, 0.0f, 0.0f);
		R3 posFrom = R3(0.0f, 0.0f, 0.0f);
		if (direction == 0){
			dirVec = R3(1.0f, 0.0f, 0.0f);
			posFrom = cu.pos;
			if (corner == 1){
				posFrom = (posFrom + R3(0.0f, 0.0f, cu.size));
			}
			else if (corner == 2){
				posFrom = (posFrom + R3(0.0f, cu.size, cu.size));
			}
			else if (corner == 3){
				posFrom = (posFrom + R3(0.0f, cu.size, 0.0f));
			}
		}
		else if (direction == 1){
			dirVec = R3(0.0f, 1.0f, 0.0f);
			posFrom = cu.pos;
			if (corner == 1){
				posFrom = (posFrom + R3(cu.size, 0.0f, 0.0f));
			}
			else if (corner == 2){
				posFrom = (posFrom + R3(cu.size, 0.0f, cu.size));
			}
			else if (corner == 3){
				posFrom = (posFrom + R3(0.0f, 0.0f, cu.size));
			}
		}
		else{
			dirVec = R3(0.0f, 0.0f, 1.0f);
			posFrom = cu.pos;
			if (corner == 1){
				posFrom = (posFrom + R3(cu.size, 0.0f, 0.0f));
			}
			else if (corner == 2){
				posFrom = (posFrom + R3(cu.size, cu.size, 0.0f));
			}
			else if (corner == 3){
				posFrom = (posFrom + R3(0.0f, cu.size, 0.0f));
			}
		}

		t = ft.RayIntersectsTriangle(posFrom, dirVec, this->normalList[index], interP, hit);
		if (hit == 1){
			*wasHit = 1;
			return t;
		}
		return 0.0f;
	}



	float SIObj::calculate_MSE_from_SOTPlane_to_Mesh(int index, SI_Cube cu, int direction, float da, float db, float dc, float dd, int& numHits)
	{
		//calculate the MSE between an SOT plane and the mesh
		SI_FullTriangle SOT_plane_TriA, SOT_plane_TriB;
		R3 Na = R3();
		R3 Nb = R3();
		R3 hp = R3();
		R3 N = this->normalList[index];
		if (direction == 0)
		{
			R3 ta = R3(cu.pos.x + da, cu.pos.y, cu.pos.z);
			R3 tb = R3(cu.pos.x + db, cu.pos.y, cu.pos.z + cu.size);
			R3 tc = R3(cu.pos.x + dc, cu.pos.y + cu.size, cu.pos.z + cu.size);
			R3 td = R3(cu.pos.x + dd, cu.pos.y + cu.size, cu.pos.z);
			SOT_plane_TriA = SI_FullTriangle(ta, tc, td);
			SOT_plane_TriB = SI_FullTriangle(ta, tb, tc);
			N = R3(1.0f, 0.0f, 0.0f);

		}
		else if (direction == 1)
		{
			R3 ta = R3(cu.pos.x, cu.pos.y + da, cu.pos.z);
			R3 tb = R3(cu.pos.x + cu.size, cu.pos.y + db, cu.pos.z);
			R3 tc = R3(cu.pos.x + cu.size, cu.pos.y + dc, cu.pos.z + cu.size);
			R3 td = R3(cu.pos.x, cu.pos.y + dd, cu.pos.z + cu.size);
			SOT_plane_TriA = SI_FullTriangle(ta, tc, td);
			SOT_plane_TriB = SI_FullTriangle(ta, tb, tc);
			N = R3(0.0f, 1.0f, 0.0f);
		}
		else
		{
			R3 ta = R3(cu.pos.x, cu.pos.y, cu.pos.z + da);
			R3 tb = R3(cu.pos.x + cu.size, cu.pos.y, cu.pos.z + db);
			R3 tc = R3(cu.pos.x + cu.size, cu.pos.y + cu.size, cu.pos.z + dc);
			R3 td = R3(cu.pos.x, cu.pos.y + cu.size, cu.pos.z + dd);
			SOT_plane_TriA = SI_FullTriangle(ta, tc, td);
			SOT_plane_TriB = SI_FullTriangle(ta, tb, tc);
			N = R3(0.0f, 0.0f, 1.0f);

		}
		Na = ( ( (SOT_plane_TriA.b -  SOT_plane_TriA.a ).getNormalizedCopy() ) ^ ( ( SOT_plane_TriA.c - SOT_plane_TriA.a ).getNormalizedCopy() ) );
		Nb = Na;
		SI_FullTriangle test_triangle = this->getTriangle(index);


		float iterf1, iterf2;
		R3 incrementor_1 = test_triangle.b - test_triangle.c, incrementor_2;
		iterf1 = incrementor_1.Magnitude() / 2.0f;
		int num_iters1 = (int)(iterf1+0.5f), num_iters2;
		R3 iterator_1 = test_triangle.a;
		incrementor_1.Normalize();
		incrementor_1 *= (1.0f /  iterf1);
		
		double count = 0.0;
		double mean = 0.0;
		//q, a to b
		for (int q = 0; q < num_iters1; q++)
		{
			//a to c
			R3 iterator_2 = iterator_1;
			incrementor_2 = (test_triangle.c - iterator_2);
			iterf2 = incrementor_2.Magnitude() / 2.0f;
			num_iters2 = (int) (iterf2+0.5f);
			incrementor_2.Normalize();
			incrementor_2 *= (1.0f / iterf2);
			for (int p = 0; p < num_iters2; p++)
			{

				int tmpWH1 = 0, tmpWH2 = 0;
				float Tv1 = SOT_plane_TriA.RayIntersectsTriangle(iterator_2, N, Na, hp, tmpWH1);
				if (tmpWH1 == 1)
				{
					mean += pow((double)(Tv1), 2.0);
					count += 1.0;
				}
				else
				{
					float Tv2 = SOT_plane_TriB.RayIntersectsTriangle(iterator_2, N, Nb, hp, tmpWH2);
					if (tmpWH2 == 1)
					{
						mean += pow((double)(Tv2), 2.0);
						count += 1.0;
					}
				}


				iterator_2 += incrementor_2;
			}
			iterator_1 += incrementor_1;
		}

		if (count == 0.0)
		{
			numHits = 0;
			return 0.0f;
		}
		else
		{
			numHits = (int)count;
			return (float)(mean);
		}

	}
	
	int SIObj::getClosestMatchingAxis(R3 v)
	{
		int tlo[6] = 
		{
			0,
			0,
			1,
			1,
			2,
			2
		};

		R3 tl[6] = 
		{
			R3(1.0f, 0.0f, 0.0f),
			R3(-1.0f, 0.0f, 0.0f),
			R3(0.0f, 1.0f, 0.0f),
			R3(0.0f, -1.0f, 0.0f),
			R3(0.0f, 0.0f, 1.0f),
			R3(0.0f, 0.0f, -1.0f)
		}
		;

		v.Normalize();
		int min_index = 0;
		float min = abs(acos(v * tl[0]));
		for(int i = 1; i < 6; i++)
		{
			float tmp = abs(acos(v * tl[i]));
			if(tmp < min)
			{
				min = tmp;
				min_index = i;
			}
		}
		return tlo[min_index];
	}

	void SIObj::addTriangles(std::vector<SI_Triangle> & triList)
	{
		int ntll = numTriangles + triList.size();
		SI_Triangle* ntl = new SI_Triangle[ntll];
		R3 * nnl = new R3[ntll];
		for(int i = 0; i < numTriangles; i++)
		{
			ntl[i] = triangleList[i];
			nnl[i] = normalList[i];
		}
		int tls = triList.size();
		for(int i = 0; i < tls; i++)
		{
			int tindex = i+numTriangles;
			ntl[tindex] = triList.at(i);
			SI_FullTriangle ft(vertList[ntl[tindex].a], vertList[ntl[tindex].b], vertList[ntl[tindex].c]);
			R3 atmp = ft.b - ft.a;
			R3 btmp = ft.c - ft.b;
			nnl[tindex] = (atmp ^ btmp);
		}

		numTriangles = ntll;
		delete [] triangleList;
		delete [] normalList;
		triangleList = ntl;
		normalList = nnl;

	
	}

	void SIObj::savePLY(std::string fname)
	{
		FILE* fi = fopen(fname.c_str(), "wb");

		int triFiller = 0;
		float vertFiller = 0.0f;

		char headBuffer[1000];
		sprintf(headBuffer, "ply\nformat binary_little_endian 1.0\ncomment Luke Generaged\nelement vertex %i\nproperty float x\nproperty float y\nproperty float z\nproperty int flags\nelement face %i\nproperty list uchar int vertex_indices\nproperty int flags\nend_header\n", numVerts, numTriangles);

		fwrite(headBuffer, 1, strlen(headBuffer), fi);

		for(int i = 0; i < numVerts; i++)
		{
			fwrite((void*)&vertList[i].x, sizeof(float), 1, fi);
			fwrite((void*)&vertList[i].y, sizeof(float), 1, fi);
			fwrite((void*)&vertList[i].z, sizeof(float), 1, fi);
			fwrite((void*)&vertFiller, sizeof(float), 1, fi);
		}
		unsigned char numPointsInTri = 0x03;
		for(int i = 0; i < numTriangles; i++)
		{
			fwrite((void*)&numPointsInTri, 1, 1, fi);
			fwrite((void*)&triangleList[i].a, sizeof(int), 1, fi);
			fwrite((void*)&triangleList[i].b, sizeof(int), 1, fi);
			fwrite((void*)&triangleList[i].c, sizeof(int), 1, fi);
			fwrite((void*)&triFiller, sizeof(int), 1, fi);
		}

		fclose(fi);
	}

	void SIObj::saveOBJ(std::string fname)
	{
		FILE* fi = fopen(fname.c_str(), "w");

		fprintf(fi, "# OBJ FILE : GENERATED BY A PROGRAM WRITTEN BY LUKE LINCOLN\n");
		for(int i = 0; i < numVerts; i++)
		{
			R3 v = vertList[i];
			fprintf(fi, "v %f %f %f\n", v.x, v.y, v.z);
		}

		for(int i = 0; i < numTriangles; i++)
		{
			SI_Triangle t = triangleList[i];
			fprintf(fi, "f %i %i %i\n", t.a+1, t.b+1, t.c+1);
		}


		fclose(fi);
	}
	

	R3 SIObj::minimal_vertex_dimensions()
	{
		R3 rv = this->vertList[0];
		for(int i = 1; i < this->numVerts; i++)
		{
			rv.x = (rv.x < vertList[i].x) ? rv.x : vertList[i].x;
			rv.y = (rv.y < vertList[i].y) ? rv.y : vertList[i].y;
			rv.z = (rv.z < vertList[i].z) ? rv.z : vertList[i].z;
		}
		return rv;
	}
	float SIObj::minDist()
	{
		R3 rve = this->minimal_vertex_dimensions();
		float rv = (rve.x < rve.y) ? rve.x : rve.y;
		rv = (rv < rve.z) ? rv : rve.z;
		return rv;
	}
	R3 SIObj::maximal_vertex_dimensions()
	{
		R3 rv = this->vertList[0];
		for(int i = 1; i < this->numVerts; i++)
		{
			rv.x = (rv.x > vertList[i].x) ? rv.x : vertList[i].x;
			rv.y = (rv.y > vertList[i].y) ? rv.y : vertList[i].y;
			rv.z = (rv.z > vertList[i].z) ? rv.z : vertList[i].z;
		}
		return rv;
	}
	float SIObj::maxDist()
	{
		R3 rve = this->maximal_vertex_dimensions();
		float rv = (rve.x > rve.y) ? rve.x : rve.y;
		rv = (rv > rve.z) ? rv : rve.z;
		return rv;
	}

	void SIObj::addToEachPoint(float a)
	{
		addToEachPointR3(R3(a, a, a));
	}
	void SIObj::addToEachPointR3(R3 p)
	{
		for(int i = 0; i < numVerts; i++)
		{
			vertList[i] += p;
		}
	}
	void SIObj::subToEachPoint(float a)
	{
		subToEachPointR3(R3(a, a, a));
	}
	void SIObj::subToEachPointR3(R3 p)
	{
		for(int i = 0; i < numVerts; i++)
		{
			vertList[i] -= p;
		}
	}
	void SIObj::scaleEachPoint(float a)
	{
		scaleEachPointR3(R3(a, a, a));
	}
	void SIObj::scaleEachPointR3(R3 p)
	{
		for(int i = 0; i < numVerts; i++)
		{
			vertList[i].x *= p.x;
			vertList[i].y *= p.z;
			vertList[i].z *= p.y;
		}
	}
	void SIObj::divEachPoint(float a)
	{
		divEachPointR3(R3(a, a, a));
	}
	void SIObj::divEachPointR3(R3 p)
	{
		for(int i = 0; i < numVerts; i++)
		{
			vertList[i].x /= p.x;
			vertList[i].y /= p.z;
			vertList[i].z /= p.y;
		}
	}

	float SIObj::normalize(float largestDist)
	{
		R3 mini;
		float maxD;
		mini = this->minimal_vertex_dimensions();
		this->subToEachPointR3(mini);
		maxD = maxDist();
		divEachPoint(maxD);
		scaleEachPoint(largestDist);
		return maxD;
	}

	void SIObj::unnormalize(float undo)
	{
		normalize(1.0f);
		scaleEachPoint(undo);
	}

	R3 SIObj::toOrigin()
	{
		R3 mini = minimal_vertex_dimensions();
		subToEachPointR3(mini);
		return mini;
	}
	R3 SIObj::ew_normalize(float largestDist)
	{
		toOrigin();
		R3 maxi = maximal_vertex_dimensions();
		divEachPointR3(maxi);
		scaleEachPoint(largestDist);
		return maxi;
	}

	void SIObj::ew_unnormalize(R3 undo)
	{
		float ld = this->maxDist();
		divEachPoint(ld);
		scaleEachPointR3(undo);
	}

	

	//simple point cloud object

	SimplePointCloudObject::SimplePointCloudObject()
	{
		this->blen = 2000;
		this->len = 0;
		this->list = (R3*)malloc(sizeof(R3) * blen);
	}

	SimplePointCloudObject::SimplePointCloudObject(char* fname)
	{
		this->blen = 2000;
		this->len = 0;
		this->list = (R3*)malloc(sizeof(R3) * blen);
		SIObj si(fname);
		for (int i = 0; i < si.numTriangles; i++)
		{
			add(si.getTriangle(i));
		}
	}

	SimplePointCloudObject::SimplePointCloudObject(char* fname, int a)
	{
		this->blen = 2000;
		this->len = 0;
		this->list = (R3*)malloc(sizeof(R3) * blen);
		SIObj si(fname);
		for (int i = 0; i < si.numTriangles; i++)
		{
			add_set(si.getTriangle(i));
		}
	}

	SimplePointCloudObject::SimplePointCloudObject(SIObj& obj)
	{
		this->blen = 2000;
		this->len = 0;
		this->list = (R3*)malloc(sizeof(R3) * blen);
		for (int i = 0; i < obj.numTriangles; i++)
		{
			add(obj.getTriangle(i));
		}
	}
	SimplePointCloudObject::SimplePointCloudObject(SIObj* obj)
	{
		this->blen = 2000;
		this->len = 0;
		this->list = (R3*)malloc(sizeof(R3) * blen);
			for (int i = 0; i < obj->numTriangles; i++)
		{
			add(obj->getTriangle(i));
		}
	}

	SimplePointCloudObject::~SimplePointCloudObject()
	{
		free(list);
	}
	void SimplePointCloudObject::inc_buffer()
	{
		int nblen = blen * 2;
		list = (R3*)realloc(list, sizeof(R3)* nblen);
		blen = nblen;
	}
	void SimplePointCloudObject::save_obj_file(char* name)
	{
		FILE* fi = fopen(name, "w");

		for (int i = 0; i < len; i++)
		{
			R3 v = this->list[i];
			fprintf(fi, "v %f %f %f\n", v.x, v.y, v.z);
		}

		fclose(fi);
	}

	void SimplePointCloudObject::add(R3 point)
	{
		if (len >= blen - 10)
		{
			inc_buffer();
		}

		this->list[len] = point;
		len++;

	}

	void SimplePointCloudObject::add_set(R3 point)
	{
		if (len >= blen - 10)
		{
			inc_buffer();
		}

		if(containsPoint(point))
		{
			return;
		}

		this->list[len] = point;
		len++;

	}

	int SimplePointCloudObject::roundF(float fin)
	{
		return ((int)(fin+0.5f));
	}

	void SimplePointCloudObject::add(SI_FullTriangle tri)
	{

		/*SimplePointCloud* spc1 = this;
		(*spc1) += tri.a;
	
		(*spc1) += tri.b;
		(*spc1) += tri.c;

		Vec3 s1 = subtract(tri.b, tri.a);
		float m1 = Magnitude(s1) * 0.5f;
		Vec3 p1 = scale(Normalize(s1), m1);
		p1 = add(p1, tri.a);

		Vec3 s2 = subtract(tri.c, p1);
		float m2 = Magnitude(s2) * 0.5f;
		Vec3 p2 = scale(Normalize(s2), m2);
		p2 = add(p2, p1);

		(*this)+= p2;


		return;*/

		float diff1 = ((tri.b - tri.a).Magnitude());
	

	


		int quantize_sample_size1 = 30;//(int)roundF((diff1 / 3.0f));
	

		if (quantize_sample_size1 == 0)
		{
			return;
		}

		R3 a = tri.a;
		R3 dir1 = (tri.b - a);
		float inc1 = (dir1.Magnitude()) / (float)quantize_sample_size1;
		R3 incVector1 = ((dir1.getNormalizedCopy()) * inc1);
	
	

		for (int r = 0; r < quantize_sample_size1; r++)
		{
			R3 b = a;
			R3 dir2 = (tri.c - b);

			float diff2 = ((tri.c - b)).Magnitude();


			int quantize_sample_size2 = 30;//(int)roundF((diff2 / 3.0f));

			float inc2 = (dir2.Magnitude()) / (float)quantize_sample_size2;
			R3 incVector2 = ((dir2.getNormalizedCopy()) * inc2);

		

			if (quantize_sample_size2 == 0)
			{
				break;
			}

			for (int i = 0; i < quantize_sample_size2; i++)
			{
				add(b);
			

				b += incVector2;
			}


			a += incVector1;
		}

	}

	void SimplePointCloudObject::add_set(SI_FullTriangle tri)
	{
		float dec_am = 4.0f;

		float diff1 = ((tri.b - tri.a).Magnitude());
		float quantize_sample_size1_dist = roundF((tri.b - tri.a).Magnitude());
		int quantize_sample_size1 = roundF(quantize_sample_size1_dist / dec_am);//(int)roundF((diff1 / 3.0f));
		
		if(quantize_sample_size1 <= 0)
		{
			return;
		}

		R3 a = tri.a;
		R3 dir1 = (tri.b - a);
		float inc1 = (dir1.Magnitude()) / (float)quantize_sample_size1;
		R3 incVector1 = ((dir1.getNormalizedCopy()) * inc1);
		for (int r = 0; r < quantize_sample_size1; r++)
		{
			R3 b = a;
			R3 dir2 = (tri.c - b);

			float diff2 = ((tri.c - b)).Magnitude();
			int quantize_sample_size2 = roundF(diff2/ dec_am);

			if(quantize_sample_size2 <= 0)
			{
				return;
			}

			float inc2 = (dir2.Magnitude()) / (float)quantize_sample_size2;
			R3 incVector2 = ((dir2.getNormalizedCopy()) * inc2);
		

			for (int i = 0; i < quantize_sample_size2; i++)
			{
				add_set(b);
				b += incVector2;
			}
			a += incVector1;
		}

	}

	void SimplePointCloudObject::operator+=(R3 point)
	{
		add(point);
	}

	void SimplePointCloudObject::operator+=(SI_FullTriangle tri)
	{
		add(tri);
	}

	R3 SimplePointCloudObject::operator[](int index)
	{
		return get(index);
	}

	R3 SimplePointCloudObject::get(int index)
	{
		return (index >= 0  && index < len)? list[index] : R3();
	}

	void SimplePointCloudObject::savePCD(char* name)
	{
		FILE* fi = fopen(name, "w");
		
		fprintf(fi, "#%s\nVERSION .7\nFIELDS x y z\nSIZE 4 4 4\nTYPE F F F\nCOUNT 1 1 1\nWIDTH %i\nHEIGHT 1\nVIEWPOINT 0 0 0 1 0 0 0\nPOINTS %i\nDATA ascii\n", name, len, len);
		
		for(int i = 0; i < len; i++)
		{
			fprintf(fi, "%f %f %f\n", list[i].x, list[i].y, list[i].z);
		}

		fclose(fi);
	}


	int SimplePointCloudObject::containsPoint(R3 p)
	{
		int rv = 0;
		
		return rv;
	}



}