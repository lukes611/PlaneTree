#include "LOcvHeaders.h"
#include "Plane_Tree_3d.h"
#include "libs\glut.h"
#include "libs\freeglut.h"
#include <stdlib.h>
#include <stdio.h>
#include "LukesCameraClass.h"
#include <math.h>
#include <stack>
#include <vector>
using namespace std;
#include "R3.h"
#include "LDatasetReader.h"
#include "SIObj.h"
using namespace LDR;
using namespace LINCOLN;
using namespace SIMPLE_OBJECT_LINCOLN;
using namespace LUKESCAMERACLASS;
#define RND(x) ((int)(x+0.5f))
#define RR(x) ((int)(x+0.5f))

int w = 500, h = 500;

#define CAD_CAM

class LcadCam
{
public:
	float angleX, angleY, rad;
	float zloc;
	LcadCam(float x = 0.0f, float y = 0.0f, float r = 500.0f, float zac = 0.0f)
	{
		angleX = x; angleY = y; rad = r;
		zloc = zac;
	}

	LcadCam(const LcadCam & c)
	{
		angleX = c.angleX;
		angleY = c.angleY;
		rad = c.rad;
		zloc = c.zloc;
	}
	LcadCam & operator = (const LcadCam & c)
	{
		angleX = c.angleX;
		angleY = c.angleY;
		rad = c.rad;
		zloc = c.zloc;
		return *this;
	}

	R3 point()
	{
		float x, y, x2, y2;
		GetUnitPointFromAngle(angleX, x, y);
		GetUnitPointFromAngle(angleY, x2, y2);
		R3 up(0.0f, 1.0f, 0.0f);
		R3 right(x, 0.0f, y);
		right *= x2;
		up *= y2;
		return ( ( right + up ) * rad ) + R3(0.0f, 0.0f, zloc);
	}

	R3 point_()
	{
		float x, y, x2, y2;
		GetUnitPointFromAngle(angleX, x, y);
		GetUnitPointFromAngle(angleY, x2, y2);
		R3 up(0.0f, 1.0f, 0.0f);
		R3 right(x, 0.0f, y);
		right *= x2;
		up *= y2;
		return ( ( right + up ) * rad );
	}
	R3 at()
	{
		return point() + (R3()-point_()).getNormalizedCopy();
	}
	R3 atDir()
	{
		return (R3()-point_()).getNormalizedCopy();
	}
	R3 right()
	{
		R3 fakeUp(0.0f, 1.0f, 0.0f);
		return (fakeUp ^ atDir()).getNormalizedCopy();
	}
	R3 up()
	{
		return atDir() ^ right();
	}
	
	void inc_(float & val, float am, float max = 1000000.0f, float min = 1000000.0f)
	{
		float nval = val + am;
		if(nval >= min && nval <= max)
		{
			val = nval;
		}
	}

	void incX(float am=0.1f)
	{
		angleX += am;
		angleX = fmod(angleX, 360.0f);
	}
	void decX(float am=0.1f)
	{
		angleX -= am;
		angleX = (angleX < 0.0f) ? 360.0f + angleX : angleX;
	}

	void incY(float am=0.1f)
	{
		angleY = (angleY+am > 80.0f) ? angleY : angleY+am;
	}
	void decY(float am=0.1f)
	{
		angleY = (angleY-am < -80.0f) ? angleY : angleY-am;
	}

	void incR(float am=0.1f)
	{
		inc_(rad, am, 80000.0f, 50.0f);
	}
	void decR(float am=0.1f)
	{
		inc_(rad, -am, 80000.0f, 50.0f);
	}

	void incZ(float am=0.1f)
	{
		zloc += am;
		//inc_(zloc, am, 80000.0f, -80000.0f);
	}
	void decZ(float am=0.1f)
	{
		//inc_(zloc, -am, 80000.0f, -80000.0f);
	}


};

#ifdef CAD_CAM
LcadCam ccam;
#else
LCAM ccam;
#endif

SIObj obj;
int MODE = 0; // 0 display object, 1 display points within, 2 estimate and display plane, 3 display plane and points, 4 display obj + points + plane
SI_Cube cu(R3(-120.0f, -80.0f, -130.0f), 64.0f);
vector<R3> points;
SpecialPlane plane_out;


class PlaneTreeNode
{
public:
	SpecialPlane plane;
	PlaneTreeNode * children[8];
	PlaneTreeNode * parent;
	bool is_leaf;
	SI_Cube cube;
	vector<int> list_of_triangles;
	PlaneTreeNode()
	{
		for(int i = 0; i < 8; i++)
			children[i] = NULL;
		parent = NULL;
		is_leaf = false;
	}
	PlaneTreeNode(SI_Cube & cu, PlaneTreeNode * parent_)
	{
		cube = cu;
		for(int i = 0; i < 8; i++)
			children[i] = NULL;
		parent = parent_;
		is_leaf = false;
	}
	void free_children()
	{
		for(int i = 0; i < 8; i++)
		{
			if(children[i])
			{
				delete children[i];
				children[i] = NULL;
			}
		}
	}
	PlaneTreeNode(const PlaneTreeNode & node)
	{
		copy_in(node);
	}
	PlaneTreeNode & operator = (const PlaneTreeNode & node)
	{
		if(&node == this) return *this;
		copy_in(node);
		return *this;
	}
	void copy_in(const PlaneTreeNode & node)
	{
		plane = node.plane;
		for(int i = 0; i < 8; i++)
			children[i] = node.children[i];
		parent = node.parent;
		is_leaf = node.is_leaf;
		cube = node.cube;
		list_of_triangles = node.list_of_triangles;
	}
	bool has_children()
	{
		for(int i = 0; i < 8; i++)
		{
			if(children[i]) return true;
		}
		return false;
	}
};

class PlaneTree
{
public:
	float threshold;
	int max_depth;
	PlaneTreeNode root;
	SIObj obj;
	vector<SI_FullTriangle> tri_list;
	
	PlaneTree(SIObj & object_in, float THRESHOLD = 20.0f, int max_depth_in = 3)
	{
		obj = object_in;
		threshold = THRESHOLD;
		max_depth = max_depth_in;

		//setup root node
		root = PlaneTreeNode(SI_Cube(R3(0.0f, 0.0f, 0.0f), 512.0f), NULL);
		root.list_of_triangles.clear();
		//add in its triangles
		for(int i = 0; i < obj.numTriangles; i++)
		{
			root.list_of_triangles.push_back(i);
		}

		build_tree(&root, 0);
	}

	void build_tree(PlaneTreeNode * node, int depth)
	{
		//get sample points:
		vector<R3> points = grab_points_interpolate(obj, node->cube, node->list_of_triangles);

		//set up node setsup(plane)
		setup_node(node, points);

		if(!node->plane.edge_found || points.size() <= 3)
		{
			if(node->parent)
			{
				for(int i = 0; i < 8; i++)
				{
					if(node->parent->children[i] == node)
					{
						PlaneTreeNode * parent = node->parent;
						delete parent->children[i];
						parent->children[i] = NULL;
						return;
					}
				}
			}
			return;
		}

		//check error
		float error = mse(node, points);
		if(depth <= max_depth && error > threshold)
		{
			//if error too high, split setsup(parent, cube, listoftriangles, checks whether any points are within), do this function for each child
			divide_node(node, points);
			if(!node->has_children())
			{
				return;
				cout << "forced leaf!" << endl;
				node->is_leaf = true;
				vector<SI_FullTriangle> tl = node->plane.get_triangles();
				for(int i = 0; i < tl.size(); i++) tri_list.push_back(tl[i]);
				return;
			}
			for(int i = 0; i < 8; i++)
			{
				if(node->children[i])
				{
					build_tree(node->children[i], depth+1);
				}
			}
		}
		else
		{
			//else set as leaf
			//cout << "caught a leaf!" << endl;
			node->is_leaf = true;
			vector<SI_FullTriangle> tl = node->plane.get_triangles();
			for(int i = 0; i < tl.size(); i++) tri_list.push_back(tl[i]);
		}
	}

	//parent[DONE], cube[DONE], listoftriangles, checks whether any points are within[DONE]
	void divide_node(PlaneTreeNode * node, vector<R3> & points)
	{
		float hs = node->cube.size * 0.5f;
		SI_Cube cubes[8] = {SI_Cube(R3(0.0f,0.0f,0.0f) + node->cube.pos, hs),
			SI_Cube(R3(hs,0.0f,0.0f) + node->cube.pos, hs),
			SI_Cube(R3(hs,hs,0.0f) + node->cube.pos, hs),
			SI_Cube(R3(0.0f,hs,0.0f) + node->cube.pos, hs),
			SI_Cube(R3(0.0f,0.0f,hs) + node->cube.pos, hs),
			SI_Cube(R3(hs,0.0f,hs) + node->cube.pos, hs),
			SI_Cube(R3(hs,hs,hs) + node->cube.pos, hs),
			SI_Cube(R3(0.0f,hs,hs) + node->cube.pos, hs)};
		for(int i = 0; i < 8; i++)
		{
			bool contains_point = false;
			for(int p = 0; p < points.size(); p++)
			{
				if(cubes[i].containsPoint(points[p]))
				{
					contains_point = true;
					break;
				}
			}
			if(contains_point)
			{
				node->children[i] = new PlaneTreeNode(cubes[i], node);
				node->children[i]->list_of_triangles.clear();
				float max_distance = 1.5f * node->children[i]->cube.size;
				R3 center = node->children[i]->cube.get_center();
				for(int ti = 0; ti < node->list_of_triangles.size(); ti++)
				{
					int tri_index = node->list_of_triangles[ti];
					if(triangle_within_distance(tri_index, center, max_distance))
					{
						node->children[i]->list_of_triangles.push_back(tri_index);
					}
				}
			}else
			{
				node->children[i] = NULL;
			}
		}
	}

	void setup_node(PlaneTreeNode * node, vector<R3> & points)
	{
		node->plane = retrieve_plane(node->cube, points);
	}

	float mse_basic(PlaneTreeNode * node, vector<R3> & points)
	{
		float rv = 0.0f;
		float tmp = 0.0f;
		for(int i = 0; i < points.size(); i++)
		{
			tmp = node->plane.distance(points[i]);
			tmp *= tmp;
			rv += tmp;
		}
		return rv / (float)points.size();
	}

	float mse(PlaneTreeNode * node, vector<R3> & points)
	{
		vector<SI_FullTriangle> triangles = node->plane.get_triangles();
		vector<R3> points2 = grab_points_interpolate(triangles);
		float rv1 = 0.0f, rv2 = 0.0f;
		float tmp = 0.0f;
		for(int i = 0; i < points.size(); i++)
		{
			float smallest = (points[i]-points2[0]).Magnitude();
			for(int j = 1; j < points2.size(); j++)
			{
				tmp = (points[i]-points2[j]).Magnitude();
				smallest = (smallest > tmp)? tmp : smallest;
			}

			smallest *= smallest;
			rv1 += smallest;
		}

		for(int i = 0; i < points2.size(); i++)
		{
			float smallest = (points2[i]-points[0]).Magnitude();
			for(int j = 1; j < points.size(); j++)
			{
				tmp = (points2[i]-points[j]).Magnitude();
				smallest = (smallest > tmp)? tmp : smallest;
			}

			smallest *= smallest;
			rv2 += smallest;
		}
		rv1 /= (float)points.size();
		rv2 /= (float)points2.size();
		return ( rv1 > rv2 ) ? rv1 : rv2 ;
	}

	bool triangle_within_distance(int triangle, R3 & center, float max_dist)
	{
		if((obj.vertList[obj.triangleList[triangle].a]-center).Magnitude() < max_dist) return true;
		if((obj.vertList[obj.triangleList[triangle].b]-center).Magnitude() < max_dist) return true;
		if((obj.vertList[obj.triangleList[triangle].c]-center).Magnitude() < max_dist) return true;
		return false;
	}

};


PlaneTree * pt;

void init(void) 
{
#ifndef BACKGROUND_COLOR_WHITE
	glClearColor (0.0, 0.0, 0.0, 0.0);
#else
	glClearColor (1.0, 1.0, 1.0, 0.0);
#endif
	glShadeModel (GL_SMOOTH);
	//glShadeModel (GL_FLAT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);//engable lighting
	glEnable(GL_LIGHT0);//enable each light GL_LIGHT0, GL_LIGHT1, GL_LIGHT2, etc...
}

void set_material_color(R3 material_color, float shininess){
	//material
	
	//usually objects have a white specular reflection
	GLfloat material_specular[] = { 1.0, 1.0, 1.0, 1.0f };

	GLfloat material_diffuse_and_ambient[] = { material_color.x, material_color.y, material_color.z, 1.0f };
	GLfloat material_shininess[] = { shininess };
   
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material_diffuse_and_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse_and_ambient);
	//glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
	//glMaterialfv(GL_FRONT, GL_SHININESS, material_diffuse_and_ambient);
}

void glR3(R3 & inp)
{
	glVertex3f(inp.x, inp.y, inp.z);
}

void glR3Line(R3 & a, R3 & b)
{
	glBegin(GL_LINES);
		glR3(a);
		glR3(b);
	glEnd();
}

void glQuadR3(R3 & a, R3 & b, R3 & c, R3 & d, R3 & normal)
{
	glBegin(GL_QUADS);
		glNormal3f(normal.x, normal.y, normal.z);
		glR3(a);
		glR3(b);
		glR3(c);
		glR3(d);
	glEnd();
}

void glQuadR3(R3 & a, R3 & b, R3 & c, R3 & d)
{
	glBegin(GL_QUADS);
		glR3(a);
		glR3(b);
		glR3(c);
		glR3(d);
	glEnd();
}

void draw_cube(R3 & corner, float size, R3 & colour)
{
	set_material_color(colour, 1.0f);
	R3 a,b,c,d, zax, yax, xax;
	
	zax = R3(0.0f, 0.0f, size);
	yax = R3(0.0f, size, 0.0f);
	xax = R3(size, 0.0f, 0.0f);
	R3 nx = xax.getNormalizedCopy();
	R3 ny = yax.getNormalizedCopy();
	R3 nz = zax.getNormalizedCopy();
	a = corner;
	b = a + xax;
	c = b + yax;
	d = a + yax;
	glQuadR3(a,b,c,d, (nz*-1.0f)); //front
	a += zax;
	b += zax;
	c += zax;
	d += zax;
	glQuadR3(a,d,c,b, (nz)); //back
	a -= zax;
	d -= zax;
	b = corner + zax;
	c = b + yax;
	glQuadR3(a,d,c,b, (nx*-1.0f)); //left side
	a += xax;
	b += xax;
	c += xax;
	d += xax;
	glQuadR3(a,b,c,d, (nx)); //right side
	a = corner;
	d = corner + zax;
	c = d + xax;
	b = a + xax;
	glQuadR3(a,d,c,b,  (ny*-1.0f)); //bottom side
	a += yax;
	b += yax;
	c += yax;
	d += yax;
	glQuadR3(a,b,c,d, (ny)); //top side

}

void DC(R3 & center, float size, R3 & col)
{
	float hs = size / 2.0f;
	R3 outPos = center - R3(size, size, size);
	draw_cube(outPos, size, col);
}


void draw_quad2(R3 & a, R3 & b, R3 & c, R3 & d, float dist = 10.0f)
{
	R3 atob = b - a;
	R3 atod = d - a;
	float atob_m = atob.Magnitude();
	float atod_m = atod.Magnitude();
	int n1 = (int)abs(atob_m / dist);
	int n2 = (int)abs(atod_m / dist);
	atob.Normalize();
	atod.Normalize();
	R3 i_a = a;
	atob *= dist;
	atod *= dist;
	//loop a to b
	for(int i = 0; i < n1; i++, i_a += atob)
	{
		//loop a to d
		R3 j_a = i_a;
		for(int j = 0; j < n2; j++, j_a += atod)
		{
			glR3Line(j_a, j_a + atod);
			glR3Line(j_a + atod, j_a + atod + atob);
			glR3Line(j_a + atod + atob, j_a + atob);
			glR3Line(j_a, j_a + atob);
		}
	}
}

void draw_quad(R3 & col, R3 & a, R3 & b, R3 & c, R3 & d)
{
	GLfloat material_diffuse_and_ambient[] = { col.x, col.y, col.z, 1.0f };
	//GLfloat material_shininess[] = { 1.0f };
   glColor3f(col.x, col.y, col.z);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material_diffuse_and_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse_and_ambient);
	glDisable(GL_LIGHTING);
	glBegin(GL_QUADS);
		glR3(a);
		glR3(b);
		glR3(c);
		glR3(d);
	glEnd();
	glEnable(GL_LIGHTING);
}

float luke_cube_verts[8][3] = 
{
    {-0.5,-0.5,0.5},
    {-0.5,0.5,0.5},
    {0.5,0.5,0.5},
    {0.5,-0.5,0.5},
    {-0.5,-0.5,-0.5},
    {-0.5,0.5,-0.5},
    {0.5,0.5,-0.5},
    {0.5,-0.5,-0.5},
};

void quad(int a,int b,int c,int d)
{
    glBegin(GL_QUADS);
    glVertex3fv(luke_cube_verts[a]);

    glVertex3fv(luke_cube_verts[b]);

    glVertex3fv(luke_cube_verts[c]);

    glVertex3fv(luke_cube_verts[d]);
    glEnd();
}

void DC_f2_lines(R3 & center, float size, R3 & col)
{
	glDisable(GL_LIGHTING);
	glColor3f(col.x, col.y, col.z);
	R3 c = center;;
	R3 x = R3(size, 0.0f, 0.0f);
	R3 y = R3(0.0f, size, 0.0f);
	R3 z = R3(0.0f, 0.0f, size);
	glR3Line(c, c + x);
	glR3Line(c, c + y);
	glR3Line(c, c + z);
	c += (x+y);
	glR3Line(c, c - x);
	glR3Line(c, c - y);
	glR3Line(c, c + z);
	c += (z-y);
	glR3Line(c, c - x);
	glR3Line(c, c + y);
	glR3Line(c, c - z);
	c += (y-x);
	glR3Line(c, c + x);
	glR3Line(c, c - y);
	glR3Line(c, c - z);
	glEnable(GL_LIGHTING);
}

void DC_f2(R3 & center, float size, R3 & col)
{
	//GLfloat material_specular[] = { 1.0, 1.0, 1.0, 1.0f };

	GLfloat material_diffuse_and_ambient[] = { col.x, col.y, col.z, 1.0f };
	//GLfloat material_shininess[] = { 1.0f };
   
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material_diffuse_and_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse_and_ambient);
	glPushMatrix();
	
	glTranslatef(center.x, center.y, center.z);
	glScalef(size, size, size);
	glBegin(GL_QUADS);
		glVertex3fv(luke_cube_verts[0]);
		glVertex3fv(luke_cube_verts[3]);
		glVertex3fv(luke_cube_verts[2]);
		glVertex3fv(luke_cube_verts[1]);

		glVertex3fv(luke_cube_verts[2]);
		glVertex3fv(luke_cube_verts[3]);
		glVertex3fv(luke_cube_verts[7]);
		glVertex3fv(luke_cube_verts[6]);

		glVertex3fv(luke_cube_verts[0]);
		glVertex3fv(luke_cube_verts[4]);
		glVertex3fv(luke_cube_verts[7]);
		glVertex3fv(luke_cube_verts[3]);

		glVertex3fv(luke_cube_verts[1]);
		glVertex3fv(luke_cube_verts[2]);
		glVertex3fv(luke_cube_verts[6]);
		glVertex3fv(luke_cube_verts[5]);

		glVertex3fv(luke_cube_verts[4]);
		glVertex3fv(luke_cube_verts[5]);
		glVertex3fv(luke_cube_verts[6]);
		glVertex3fv(luke_cube_verts[7]);

		glVertex3fv(luke_cube_verts[0]);
		glVertex3fv(luke_cube_verts[1]);
		glVertex3fv(luke_cube_verts[5]);
		glVertex3fv(luke_cube_verts[4]);
	glEnd();
    glPopMatrix();

}

void draw_bounding_box(R3 center, float size)
{
	float hw = size * 0.5f;
	R3 c = center - R3(hw, hw, hw);
	//cout << c << endl;
	glDisable(GL_LIGHTING);
	glColor3f(1.0f,0.0f,0.0f);
	glBegin(GL_LINE_LOOP);
		glR3(c);
		glR3(c + R3(size, 0.0f, 0.0f));
		glR3(c + R3(size, size, 0.0f));
		glR3(c + R3(0.0f, size, 0.0f));
		glR3(c);
		glR3(c + R3(0.0f, 0.0f, size));
		glR3(c + R3(size, 0.0f, size));
		glR3(c + R3(size, size, size));
		glR3(c + R3(0.0f, size, size));
		glR3(c + R3(0.0f, 0.0f, size));
	glEnd();
	glBegin(GL_LINE_LOOP);
		glR3(c + R3(0.0f, size, 0.0f));
		glR3(c + R3(0.0f, size, size));
		glR3(c + R3(size, size, size));
		glR3(c + R3(size, size, 0.0f));
		glR3(c + R3(size, 0.0f, 0.0f));
		glR3(c + R3(size, 0.0f, size));
		glR3(c + R3(0.0f, 0.0f, size));
		glR3(c + R3(0.0f, size, size));
	glEnd();
	glEnable(GL_LIGHTING);
	//cout << c << endl;
}

void DC_f(R3 & center, float size, R3 & col)
{
	//GLfloat material_specular[] = { 1.0, 1.0, 1.0, 1.0f };

	GLfloat material_diffuse_and_ambient[] = { col.x, col.y, col.z, 1.0f };
	//GLfloat material_shininess[] = { 1.0f };
   
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material_diffuse_and_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse_and_ambient);
	glPushMatrix();
	glTranslatef(center.x, center.y, center.z);
	glPushMatrix();
	glScalef(size, size, size);
	
    quad(0,3,2,1);
    quad(2,3,7,6);
    quad(0,4,7,3);
    quad(1,2,6,5);
    quad(4,5,6,7);
    quad(0,1,5,4);
	glPopMatrix();
	glPopMatrix();

}

void drawObject_SL(SIObj* ob){
	for(int i = 0; i < ob->numTriangles; i++){


			set_material_color(R3(0.5f, 0.5f, 0.5f), 128.0f);
			if(ob->numTriangles - i < 25250)
			{
				//set_material_color(0.8f, 0.0f, 0.0f, 128.0f);
			}

			glBegin(GL_POLYGON);
			SI_FullTriangle t = ob->getTriangle(i);
			R3 normal = (ob->normalList[i].getNormalizedCopy());
#ifdef CAD_CAM
			R3 eyev = ccam.point();
#else
			R3 eyev = R3(camera.cam.eye.x, camera.cam.eye.y, camera.cam.eye.z);
#endif
			R3 cvec = (eyev- t.a);
			cvec.Normalize();
			float angle = acos((cvec * normal));
			if(abs(angle) * 57.3 >= 90.0f){
				normal = !(normal);
			}

			
			glNormal3f(normal.x,normal.y,normal.z);
			glVertex3f(t.a.x, t.a.y, t.a.z);
			glNormal3f(normal.x,normal.y,normal.z);
			glVertex3f(t.b.x, t.b.y, t.b.z);
			glNormal3f(normal.x,normal.y,normal.z);
			glVertex3f(t.c.x, t.c.y, t.c.z);
		
		glEnd();
	}

}


void gluLookAt(R3 eye, R3 at, R3 up)
{
	gluLookAt(eye.x, eye.y, eye.z, at.x, at.y, at.z, up.x, up.y, up.z);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1,1,1);
	
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1, 1, -1, 1, 1.5, 2000000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
#ifndef CAD_CAM
	gluLookAt(ccam.cam.eye.x,ccam.cam.eye.y,ccam.cam.eye.z, ccam.cam.at.x,ccam.cam.at.y,ccam.cam.at.z, 0.0, 1.0, 0.0);
#else
	gluLookAt(ccam.point(), ccam.at(), ccam.up());
#endif
	//glScalef(1.0f, -w/(float)h, 1.0f);
	
	//Point Lighting
	R3 cl = ccam.point();
	GLfloat light_position[] = {cl.x,cl.y,cl.z, 1.0f};
	//GLfloat light_position[] = {300.0f, 600.0f, 500.0f, 1.0f};
	//GLfloat light_position[] = {300.0f, 300.0f, 300.1f, 1.0f};
	//GLfloat light_position[] = {30.6f, 6.0f, -56.2f, 1.0f};
	//The first 3 elements represent the position of the light
	//The fourth parameter must be set to 1, to indicate point lighting

	GLfloat light_diffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};//the diffuse values (rgba) 
	GLfloat light_ambient[] = {0.5f, 0.5f, 0.5f, 1.0f};//the ambient values, set darker than diffuse
	
	//Now pass parameters to OpenGL, we changing the settings for GL_LIGHT0, but 
	//these functions work with the other lights
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);//pass the position
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);//pass the diffuse values
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);//pass the ambient values

	if(MODE==0)
	{
		glDisable(GL_LIGHTING);
		glColor3f(1.0f, 0.5f, 0.5f);
		//cout << pt->tri_list.size() << endl;
		for(int i = 0; i < pt->tri_list.size(); i++)
		{
			glR3Line(pt->tri_list[i].a, pt->tri_list[i].b);
			glR3Line(pt->tri_list[i].a, pt->tri_list[i].c);
			glR3Line(pt->tri_list[i].c, pt->tri_list[i].b);
		}
		glEnable(GL_LIGHTING);
	}
	else
	{
		glBegin(GL_TRIANGLES);
		for(int i = 0; i < pt->tri_list.size(); i++)
		{
			SI_FullTriangle t = pt->tri_list[i];
			R3 normal = (t.b-t.a).getNormalizedCopy() ^ (t.c-t.a).getNormalizedCopy();
			normal.Normalize();
			R3 eyev = cl;
			R3 cvec = (eyev- t.a);
			cvec.Normalize();
			float angle = acos((cvec * normal));
			if(abs(angle) * 57.3 >= 90.0f) normal = !(normal);
			glNormal3f(normal.x,normal.y,normal.z);
			glVertex3f(t.a.x, t.a.y, t.a.z);
			glNormal3f(normal.x,normal.y,normal.z);
			glVertex3f(t.b.x, t.b.y, t.b.z);
			glNormal3f(normal.x,normal.y,normal.z);
			glVertex3f(t.c.x, t.c.y, t.c.z);
		}
		glEnd();
	}

	glutSwapBuffers();
	glutPostRedisplay();
}


void reshape(int wi, int hi)
{
	glViewport (0, 0, (GLsizei) wi, (GLsizei) hi);
	w = wi;
	h = hi;
}
bool leftPressed = false, rightPressed = false, middlePressed = false;
Point2i lpp, rpp, mpp;
void mouse(int button, int state, int x, int y) 
{
#ifdef CAD_CAM
	if(button == GLUT_LEFT_BUTTON)
	{
		if(state == GLUT_DOWN )
		{
			leftPressed = true;
			lpp = Point2i(x,y);
		}else if(state == GLUT_UP)
		{
			leftPressed = false;
		}
	}

	if(button == GLUT_RIGHT_BUTTON)
	{
		if(state == GLUT_DOWN )
		{
			rightPressed = true;
			rpp = Point2i(x,y);
		}else if(state == GLUT_UP)
		{
			rightPressed = false;
		}
	}

	if(button == GLUT_MIDDLE_BUTTON)
	{
		if(state == GLUT_DOWN )
		{
			middlePressed = true;
			mpp = Point2i(x,y);
		}else if(state == GLUT_UP)
		{
			middlePressed = false;
		}
	}
#endif
}

void mousem(int x, int y)
{
#ifdef CAD_CAM
	if(leftPressed)
	{
		Point2i np(x,y);
		Point2i dir(np.x-lpp.x, np.y-lpp.y);
		//cout << dir << endl;
		R3 dir3((float)dir.x, (float)dir.y, 0.0f);
		dir3 *= 0.5f;
		ccam.incX(dir3.x);
		ccam.incY(dir3.y);
		lpp = np;
	}

	if(rightPressed)
	{
		Point2i np(x,y);
		Point2i dir(np.x-rpp.x, np.y-rpp.y);
		R3 dir3((float)dir.x, (float)dir.y, 0.0f);
		ccam.incR(dir3.y);
		rpp = np;
	}

	if(middlePressed)
	{
		Point2i np(x,y);
		Point2i dir(np.x-mpp.x, np.y-mpp.y);
		R3 dir3((float)dir.x, (float)dir.y, 0.0f);
		ccam.incZ(dir3.y);
		mpp = np;
		

	}
#endif
	//cout << ccam.point() <<" : " << ccam.at() << endl;
}


void mousem2(int x, int y)
{
	int a =5;
	

}

float ma = 10.0f, la = 2.0f;

void kb(unsigned char key, int x, int y)
{
	


#ifndef CAD_CAM
	if(key == 'w')
	{
		ccam.moveForwards(ma);
	}else if(key == 's')
	{
		ccam.moveBackwards(ma);
	}else if(key == 'a')
	{
		ccam.moveLeft(ma);
	}else if(key == 'z')
	{
		ccam.moveUD(-ma);
	}else if(key == 'x')
	{
		ccam.moveUD(ma);
	}else if(key == 'd')
	{
		ccam.moveRight(ma);
	}
	if(key == 'i')
	{
		ccam.lookUp(la);
	}else if(key == 'j')
	{
		ccam.lookLeft(la);
	}else if(key == 'k')
	{
		ccam.lookDown(la);
	}else if(key == 'l')
	{
		ccam.lookRight(la);
	}
#else


#endif

	float am = 8.0f;
	switch(key)
	{
	case 'q': cu.pos.y += am;break;
	case 'a': cu.pos.y -= am;break;
	case 'w': cu.pos.x += am;break;
	case 's': cu.pos.x -= am;break;
	case 'e': cu.pos.z += am;break;
	case 'd': cu.pos.z -= am;break;
	}
	


	if(key == 'n')
	{
		switch(MODE)
		{
		case 0:
			{
				MODE = 1;
			}
			break;
		case 1:
			{
				MODE = 0;
			}
			break;
		case 2:
			{
				MODE = 0;
			}
			break;
		}
	}


	if(key == '5')
	{
#ifdef CAD_CAM
		cout << "xy angles: " << ccam.angleX << ", " << ccam.angleY << " ";
		cout << "rad,zax: " << ccam.rad << ", " << ccam.zloc << endl;
		cout << "cube: pos: " << cu.pos << ", size: " << cu.size << endl;
#else
		cout << "pos: " << R3(ccam.cam.eye.x, ccam.cam.eye.y, ccam.cam.eye.z) << endl;
#endif
	}
}


/* 
 *  Request double buffer display mode.
 *  Register mouse input callback functions
 */



void cameraCapL()
{
	

	
}

void ex()
{
	
	
	
}



int main(int argc, char * * argv)
{
#ifdef CAD_CAM
	//xy angles: -97.5, 9.5 rad,zax: 239, 63
	ccam = LcadCam(-97.5f, 9.5f, 239.0f, 63.0f); 
#else
	LC_Pointf newPos = LC_Pointf(-75.598587f,135.000000f,-155.177887f);
	ccam = LCAM(newPos, 46, 0);
#endif
	
	cout << "reading in bunny...";
	obj = SIObj("bunny.si");
	obj.normalize(512.0f);

	cout << "compressing...";
	pt = new PlaneTree(obj, 1.0f, 6); 
	cout << "[done]\n";

	obj.subToEachPoint(256.0f);
	cout << "[DONE]" <<endl;

	atexit(ex);
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize (w, h); 
	glutInitWindowPosition (100, 100);
	glutCreateWindow (argv[0]);
	init ();
	glutDisplayFunc(display); 
	glutKeyboardFunc(kb);
	glutReshapeFunc(reshape); 
	glutIdleFunc(cameraCapL);
	glutMotionFunc(mousem);
	glutPassiveMotionFunc(mousem2);
	glutMouseFunc(mouse);
	glutMainLoop();

	
	return 0;
}

