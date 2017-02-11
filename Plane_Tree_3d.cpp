#include "Plane_Tree_3d.h"

//Quad_3d
Quad_3D::Quad_3D()
{}
Quad_3D::Quad_3D(R3 & aIn, R3 & bIn, R3 & cIn, R3 & dIn)
{
	copy_in(aIn, bIn, cIn, dIn);
}
Quad_3D::Quad_3D(Quad_3D & q)
{
	copy_in(q);
}
Quad_3D & Quad_3D::operator = (Quad_3D & q)
{
	copy_in(q);
	return *this;
}
void Quad_3D::copy_in(Quad_3D & q)
{
	copy_in(q.a, q.b, q.c, q.d);
}
void Quad_3D::copy_in(R3 & aIn, R3 & bIn, R3 & cIn, R3 & dIn)
{
	a = aIn;
	b = bIn;
	c = cIn;
	d = dIn;
}
R3 Quad_3D::Normal()
{
	R3 atob = (b-a).getNormalizedCopy();
	R3 atoc = (c-a).getNormalizedCopy();
	atob ^= atoc;
	atob.invert();
	return atob;
}
R3 Quad_3D::iNormal()
{
	R3 rv = Normal();
	rv.invert();
	return rv;
}
R3 Quad_3D::average()
{
	return (a+b+c+d) / 4.0f;
}

//SpecialPlane
SpecialPlane::SpecialPlane()
{
	edge_found = true;
	cube_size = 10.0f;
	Normal = R3(0.0f, 1.0f, 0.0f);
	initA();
}
//Constructor A below
SpecialPlane::SpecialPlane(R3 & cube_position, float & cubeSize, R3 & pnt_on_plane, R3 & NormalIn)
{
	edge_found = true;
	cube_pos = cube_position;
	cube_size = cubeSize;
	point_on_plane = pnt_on_plane;
	Normal = NormalIn.getNormalizedCopy();
	initA();
}
//Constructor B below
SpecialPlane::SpecialPlane(R3 & cube_position, float & cubeSize, int & edge_num, float & distanceCode, R3 & NormalIn)
{
	edge_found = true;
	cube_pos = cube_position;
	cube_size = cubeSize;
	edge_number = edge_num;
	distance_code = distanceCode;
	Normal = NormalIn.getNormalizedCopy();
	initB();
}
SpecialPlane::SpecialPlane(const SpecialPlane & s)
{
	copy_in(s);
}
SpecialPlane & SpecialPlane::operator = (const SpecialPlane & s)
{
	copy_in(s);
	return *this;
}
void SpecialPlane::copy_in(const SpecialPlane & s)
{
	cube_pos = s.cube_pos;
	cube_size = s.cube_size;
	edge_number = s.edge_number;
	distance_code = s.distance_code;
	Normal = s.Normal;
	point_on_plane = s.point_on_plane;
	D = s.D;
	edge_found = s.edge_found;
}
void SpecialPlane::initA() //must initialize edge number and distance code and D
{
	initD();
	bool found_edge = false;
	for(int i = 0; i < 12; i++)
	{
		R3 p = get_edge_point_A(i);
		R3 ray = get_edge_dir(i);
		bool intersection = false;
		float t = ray_intersect_plane(p, ray, intersection);
		if(intersection)
		{
			if(t >= 0.0f && t <= cube_size)
			{
				found_edge = true;
				edge_number = i;
				distance_code = t / cube_size;
				break;
			}
		}
	}
	if(!found_edge)
	{
		edge_found = false;
		//cout << "could not find edge!!! error " << endl;
	}
}
void SpecialPlane::initB() //must initialize point on plane and D
{
	point_on_plane = get_edge_point_A(edge_number) + (get_edge_dir(edge_number) * distance_code);
	initD();
}
void SpecialPlane::initD()
{
	//R3 iNormal = Normal;
	//iNormal.invert();
	//D = iNormal.DotProduct(point_on_plane);
	D = -(Normal.DotProduct(point_on_plane));
}
//edges go from 0 to 11
R3 SpecialPlane::get_edge_point_A(int edge_number_input)
{
	R3 rv;
	/*
		____6___
		|       |
		|7      |5
		|___4___|
			
		11   8  10
		/   /   /
		/11 /   /
		/   /   /10
	/___/___/   /
	|  /    |  /9
	| /8    | / 
	|/______|/
		   

		____2___
	|       |
	|3      |1
	|___0___|

	*/

	switch(edge_number_input)
	{
	case 0:
	case 3:
	case 8:
	break;
	case 1:
	case 9:
		rv.x = cube_size;
		break;
	case 2:
	case 11:
		rv.y = cube_size;
		break;
	case 4:
	case 7:
		rv.z = cube_size;
		break;
	case 5:
		rv.z = cube_size;
		rv.x = cube_size;
		break;
	case 6:
		rv.z = cube_size;
		rv.y = cube_size;
		break;
	case 10:
		rv.x = cube_size;
		rv.y = cube_size;
		break;
	}
	return rv + cube_pos;
}
//edges go from 0 to 11
R3 SpecialPlane::get_edge_dir(int edge_number_input)
{
	R3 rv;
	/*
		____>___
		|       |
		^       ^
		|___>___|
			
		        
		/   /   /
		/   /   /
		^   ^   ^
	/___/___/   /
	|  /    |  ^
	| /     | / 
	|/______|/
		   

		____>___
	|       |
	^       ^
	|___>___|

	*/

	switch(edge_number_input)
	{
	case 0:
	case 2:
	case 4:
	case 6:
		rv.x = 1.0f;
	break;
	case 1:
	case 3:
	case 5:
	case 7:
		rv.y = 1.0f;
		break;
	case 8:
	case 9:
	case 10:
	case 11:
		rv.z = 1.0f;
		break;
	}
	return rv;
}
R3 SpecialPlane::get_edge_point_B(int edge_number_input)
{
	return get_edge_point_A(edge_number_input) + (get_edge_dir(edge_number_input) * cube_size);
}
//returns the scalar for ray_vector in which the ray intersects the plane
float SpecialPlane::ray_intersect_plane(R3 & point, R3 ray_vector, bool & hit)
{
	hit = false;
	ray_vector.Normalize();
	float b = Normal.DotProduct(ray_vector);
	if(b == 0.0f) return 0.0f;
	hit = true;
	float a = Normal.DotProduct(point) + D;
	return (-a) / b;
}
//returns the distance from a point to the plane
float SpecialPlane::distance(R3 & point)
{
	return abs(Normal.DotProduct(point) + D);
}
R3 SpecialPlane::get_edge_point()
{
	return get_edge_point_A(edge_number) + (get_edge_dir(edge_number) * distance_code);
}
vector<R3> SpecialPlane::get_points()
{
	vector<R3> rv;
	for(int i = 0; i < 12; i++)
	{
		R3 p = get_edge_point_A(i);
		R3 ray = get_edge_dir(i);
		ray.Normalize();
		bool intersection = false;
		float t = ray_intersect_plane(p, ray, intersection);
		if(intersection)
		{
			if(t >= 0.0f && t < cube_size)
			{
				R3 output = p + (ray*t); 
				rv.push_back(output);
			}
		}
	}
	order_points(rv);
	return rv;
}
void SpecialPlane::order_points(vector<R3> & pts)
{
	if(pts.size() <= 0) return;
	R3 avg;
	for(int i = 0; i < pts.size(); i++)
	{
		avg += pts[i];
	}
	avg /= (float)pts.size();
		
	vector<float> angles;
	R3 yvec = Normal;
	R3 xvec = (yvec.x == 0.0f && yvec.y == 1.0f && yvec.z == 0.0f)? R3(0.5f, 0.5f, 0.0f).getNormalizedCopy(): R3(0.0f, 1.0f, 0.0f);
	xvec = yvec ^ xvec;
	R3 zvec = yvec ^ xvec;
	for(int i = 0; i < pts.size(); i++)
	{
		R3 pt = pts[i];
		pt -= avg;
		pt = R3(xvec.x*pt.x + xvec.y*pt.y + xvec.z*pt.z,
			yvec.x*pt.x + yvec.y*pt.y + yvec.z*pt.z,
			zvec.x*pt.x + zvec.y*pt.y + zvec.z*pt.z);
			
		float ang = (pt.x == 0.0f)? 0.0f : getAngle(pt.x, pt.z);
		angles.push_back(ang);
	}

	for(int i = 0; i < pts.size(); i++)
	{
		for(int j = 0; j < pts.size()-1; j++)
		{
			if(angles[j+1] < angles[j])
			{
				swap(angles[j+1], angles[j]);
				swap(pts[j+1], pts[j]);
			}
		}
	}
}
vector<SI_FullTriangle> SpecialPlane::get_triangles()
{
	vector<SI_FullTriangle> rv;
	vector<R3> pts = get_points();
	if(pts.size() < 3) return rv;
	for(int i = 1; i < pts.size()-1; i++)
	{
		rv.push_back(SI_FullTriangle(pts[0], pts[i], pts[i+1]));
	}
	return rv;
}


//functions:
Mat luke_ls(Mat & X, Mat & y)
{
	Mat Xt = X.clone();
	Xt = Xt.t();
	Mat p1 = Xt * X;
	p1 = p1.inv();
	p1 *= Xt;
	p1 *= y;
	return p1.clone();
}
Quad_3D retrieve_planeY(SI_Cube & cube, vector<R3> & pnts)
{
	Quad_3D rv;
	
	int N = pnts.size();
	Mat X(N, 3, CV_32FC1);
	Mat y(N, 1, CV_32FC1); 
	for(int i = 0; i < N; i++)
	{
		R3 p = pnts[i] - cube.pos;
		X.at<float>(i, 0) = p.x;
		X.at<float>(i, 1) = p.z;
		X.at<float>(i, 2) = 1.0f;
		y.at<float>(i,0) = p.y;
	}
	Mat B = luke_ls(X, y);
	//cout << "sizes: " << endl;
	//cout << B.size() << endl;
	//cout << B_2.size() << endl;

	float a1 = B.at<float>(0,0);
	float a2 = B.at<float>(1,0);
	float b = B.at<float>(2,0);

	R3 p1(0.0f, a1*0.0f + a2*0.0f + b, 0.0f);
	R3 p2(cube.size, a1*cube.size + a2*0.0f + b, 0.0f);
	R3 p3(0.0f, a1*0.0f + a2*cube.size + b, cube.size);
	R3 p4(cube.size, a1*cube.size + a2*cube.size + b, cube.size);
	rv.a = p1 + cube.pos;
	rv.b = p2 + cube.pos;
	rv.c = p4 + cube.pos;
	rv.d = p3 + cube.pos;
	return rv;
}
Quad_3D retrieve_planeX(SI_Cube & cube, vector<R3> & pnts)
{
	Quad_3D rv;
	
	int N = pnts.size();
	Mat X(N, 3, CV_32FC1);
	Mat y(N, 1, CV_32FC1); 
	for(int i = 0; i < N; i++)
	{
		R3 p = pnts[i] - cube.pos;
		X.at<float>(i, 0) = p.y;
		X.at<float>(i, 1) = p.z;
		X.at<float>(i, 2) = 1.0f;
		y.at<float>(i,0) = p.x;
	}
	Mat B = luke_ls(X, y);
	//cout << "sizes: " << endl;
	//cout << B.size() << endl;
	//cout << B_2.size() << endl;

	float a1 = B.at<float>(0,0);
	float a2 = B.at<float>(1,0);
	float b = B.at<float>(2,0);

	R3 p1(a1*0.0f + a2*0.0f + b, 0.0f, 0.0f);
	R3 p2(a1*cube.size + a2*0.0f + b, cube.size, 0.0f);
	R3 p3(a1*0.0f + a2*cube.size + b, 0.0f, cube.size);
	R3 p4(a1*cube.size + a2*cube.size + b, cube.size, cube.size);
	rv.a = p1 + cube.pos;
	rv.b = p2 + cube.pos;
	rv.c = p4 + cube.pos;
	rv.d = p3 + cube.pos;
	return rv;
}
Quad_3D retrieve_planeZ(SI_Cube & cube, vector<R3> & pnts)
{
	Quad_3D rv;
	
	int N = pnts.size();
	Mat X(N, 3, CV_32FC1);
	Mat y(N, 1, CV_32FC1); 
	for(int i = 0; i < N; i++)
	{
		R3 p = pnts[i] - cube.pos;
		X.at<float>(i, 0) = p.x;
		X.at<float>(i, 1) = p.y;
		X.at<float>(i, 2) = 1.0f;
		y.at<float>(i,0) = p.z;
	}
	Mat B = luke_ls(X, y);
	//cout << "sizes: " << endl;
	//cout << B.size() << endl;
	//cout << B_2.size() << endl;

	float a1 = B.at<float>(0,0);
	float a2 = B.at<float>(1,0);
	float b = B.at<float>(2,0);

	R3 p1(0.0f, 0.0f, a1*0.0f + a2*0.0f + b);
	R3 p2(cube.size, 0.0f, a1*cube.size + a2*0.0f + b);
	R3 p3(0.0f, cube.size, a1*0.0f + a2*cube.size + b);
	R3 p4(cube.size, cube.size, a1*cube.size + a2*cube.size + b);
	rv.a = p1 + cube.pos;
	rv.b = p2 + cube.pos;
	rv.c = p4 + cube.pos;
	rv.d = p3 + cube.pos;
	return rv;
}
Quad_3D retrieve_plane_q(SI_Cube & cube, vector<R3> & pnts)
{
	Quad_3D rv;
	if(pnts.size() == 0) return rv;
	R3 avg;
	for(int i = 0; i < pnts.size(); i++)
	{
		avg += pnts[i];
	}
	avg /= (float)(pnts.size());
	R3 vars;
	float tmp;
	for(int i = 0; i < pnts.size(); i++)
	{
		tmp = pnts[i].x - avg.x;
		vars.x += tmp*tmp;
		tmp = pnts[i].y - avg.y;
		vars.y += tmp*tmp;
		tmp = pnts[i].z - avg.z;
		vars.z += tmp*tmp;
	}
	if(vars.x <= vars.y && vars.x <= vars.z)
	{
		return retrieve_planeX(cube, pnts);
	}else if(vars.y <= vars.z)
	{
		return retrieve_planeY(cube, pnts);
	}else
	{
		return retrieve_planeZ(cube, pnts);
	}

}
SpecialPlane retrieve_plane(SI_Cube & cube, vector<R3> & pnts)
{
	Quad_3D q = retrieve_plane_q(cube, pnts);
	SpecialPlane rv(cube.pos, cube.size, q.average(), q.Normal());
	return rv;
}
vector<R3> grab_points_basic(SIObj & ob, SI_Cube & cube)
{
	vector<R3> rv;
	for(int i = 0; i < ob.numVerts; i++)
	{
		if(cube.containsPoint(ob.vertList[i]))//ob.vertList[i].x >= cube.pos.x)
			rv.push_back(ob.vertList[i]);
	}
	cout << rv.size() << endl;
	return rv;
}
vector<R3> grab_points_interpolate(vector<SI_FullTriangle> & triangles, float sample_frequency)
{
	vector<R3> rv;
	for(int t = 0; t < triangles.size(); t++)
	{
		SI_FullTriangle tri = triangles[t];
		R3 a_to_c = tri.c - tri.a;
		float mag = a_to_c.Magnitude();
		mag /= 4.0f;
		a_to_c.Normalize();
		a_to_c *= mag;
		for(int j = 0; j < (int) sample_frequency; j++, tri.a += a_to_c)
		{
			R3 a = tri.a;
			R3 a_to_b = tri.b - a;
			mag = a_to_b.Magnitude();
			mag /= 4.0f;
			a_to_b.Normalize();
			a_to_b *= mag;
			for(int i = 0; i < (int) sample_frequency; i++, a += a_to_b)
			{
				rv.push_back(a);
			}
		}
	}
	return rv;
}
vector<R3> grab_points_interpolate(SIObj & ob, SI_Cube & cube)
{
	vector<R3> rv;
	float sample_frequency = 4.0f;
	for(int t = 0; t < ob.numTriangles; t++)
	{
		SI_FullTriangle tri = ob.getTriangle(t);
		R3 a_to_c = tri.c - tri.a;
		float mag = a_to_c.Magnitude();
		mag /= 4.0f;
		a_to_c.Normalize();
		a_to_c *= mag;
		for(int j = 0; j < (int) sample_frequency; j++, tri.a += a_to_c)
		{
			R3 a = tri.a;
			R3 a_to_b = tri.b - a;
			mag = a_to_b.Magnitude();
			mag /= 4.0f;
			a_to_b.Normalize();
			a_to_b *= mag;
			for(int i = 0; i < (int) sample_frequency; i++, a += a_to_b)
			{
				if(cube.containsPoint(a))
				{
					rv.push_back(a);
				}
			}
		}
	}
	return rv;
}

vector<R3> grab_points_interpolate(SIObj & ob, SI_Cube & cube, vector<int> & triangle_list, float sample_frequency)
{
	vector<R3> rv;
	for(int ii = 0; ii < triangle_list.size(); ii++)
	{
		int t = triangle_list[ii];
		SI_FullTriangle tri = ob.getTriangle(t);
		R3 a_to_c = tri.c - tri.a;
		float mag = a_to_c.Magnitude();
		mag /= 4.0f;
		a_to_c.Normalize();
		a_to_c *= mag;
		for(int j = 0; j < (int) sample_frequency; j++, tri.a += a_to_c)
		{
			R3 a = tri.a;
			R3 a_to_b = tri.b - a;
			mag = a_to_b.Magnitude();
			mag /= 4.0f;
			a_to_b.Normalize();
			a_to_b *= mag;
			for(int i = 0; i < (int) sample_frequency; i++, a += a_to_b)
			{
				if(cube.containsPoint(a))
				{
					rv.push_back(a);
				}
			}
		}
	}
	return rv;
}
