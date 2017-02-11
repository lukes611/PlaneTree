#ifndef PLANE_TREE_3D_H
#define PLANE_TREE_3D_H

#include "LOcvHeaders.h"
#include "R3.h"
using namespace LINCOLN;
#include "SIObj.h"
using namespace SIMPLE_OBJECT_LINCOLN;
#include <iostream>
#include <vector>
using namespace std;

class Quad_3D
{
public:
	R3 a, b, c, d;
	Quad_3D();
	Quad_3D(R3 & aIn, R3 & bIn, R3 & cIn, R3 & dIn);
	Quad_3D(Quad_3D & q);
	Quad_3D & operator = (Quad_3D & q);
	void copy_in(Quad_3D & q);
	void copy_in(R3 & aIn, R3 & bIn, R3 & cIn, R3 & dIn);
	R3 Normal();
	R3 iNormal();
	R3 average();
};

class SpecialPlane
{
public:
	R3 cube_pos; //
	float cube_size; //
	int edge_number; // A
	float distance_code;//goes from 0 to 1 and represents the distance from the edge-point to an actual plane point // A
	R3 Normal; //
	R3 point_on_plane; //B
	float D; //A B
	bool edge_found;
	SpecialPlane();
	//Constructor A below
	SpecialPlane(R3 & cube_position, float & cubeSize, R3 & pnt_on_plane, R3 & NormalIn);
	//Constructor B below
	SpecialPlane(R3 & cube_position, float & cubeSize, int & edge_num, float & distanceCode, R3 & NormalIn);
	SpecialPlane(const SpecialPlane & s);
	SpecialPlane & operator = (const SpecialPlane & s);
	void copy_in(const SpecialPlane & s);
	void initA(); //must initialize edge number and distance code and D
	void initB(); //must initialize point on plane and D
	void initD();
	//edges go from 0 to 11
	R3 get_edge_point_A(int edge_number_input);
	//edges go from 0 to 11
	R3 get_edge_dir(int edge_number_input);
	R3 get_edge_point_B(int edge_number_input);
	//returns the scalar for ray_vector in which the ray intersects the plane
	float ray_intersect_plane(R3 & point, R3 ray_vector, bool & hit);
	//returns the distance from a point to the plane
	float distance(R3 & point);
	R3 get_edge_point();
	vector<R3> get_points();
	void order_points(vector<R3> & pts);
	vector<SI_FullTriangle> get_triangles();
};


Mat luke_ls(Mat & X, Mat & y);
Quad_3D retrieve_planeY(SI_Cube & cube, vector<R3> & pnts);
Quad_3D retrieve_planeX(SI_Cube & cube, vector<R3> & pnts);
Quad_3D retrieve_planeZ(SI_Cube & cube, vector<R3> & pnts);
Quad_3D retrieve_plane_q(SI_Cube & cube, vector<R3> & pnts);
SpecialPlane retrieve_plane(SI_Cube & cube, vector<R3> & pnts);
vector<R3> grab_points_basic(SIObj & ob, SI_Cube & cube);
vector<R3> grab_points_interpolate(vector<SI_FullTriangle> & triangles, float sample_frequency = 4.0f);
vector<R3> grab_points_interpolate(SIObj & ob, SI_Cube & cube);
vector<R3> grab_points_interpolate(SIObj & ob, SI_Cube & cube, vector<int> & triangle_list, float sample_frequency = 4.0f);

#endif