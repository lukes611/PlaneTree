#ifndef LUKESCAMERACLASS_H
#define LUKESCAMERACLASS_H

namespace LUKESCAMERACLASS
{

class LC_Pointf{
public:
	float x, y, z;
	LC_Pointf()
	{
		x = y = z = 0.0f;
	}
	LC_Pointf(float xin, float yin, float zin)
	{
		x = xin;
		y = yin;
		z = zin;
	}
	LC_Pointf(float inp)
	{
		x = y = z = inp;
	}
	LC_Pointf(LC_Pointf & inp)
	{
		x = inp.x;
		y = inp.y;
		z = inp.z;
	}
	LC_Pointf & operator = (LC_Pointf & inp)
	{
		x = inp.x;
		y = inp.y;
		z = inp.z;
		return *this;
	}
};

class LC_Camera
{
public:
	LC_Pointf eye;
	LC_Pointf at;
	LC_Pointf up;
	float angley;
	float anglex;
	float radius;
	LC_Camera()
	{}
	LC_Camera(LC_Pointf eyein, LC_Pointf atin, LC_Pointf upin, float angleyIn, float anglexIn, float radin)
	{
		eye = eyein;
		at = atin;
		up = upin;
		angley = angleyIn;
		anglex = anglexIn;
		radius = radin;
	}
	LC_Camera(LC_Camera & c)
	{
		eye = c.eye;
		at = c.at;
		up = c.up;
		angley = c.angley;
		anglex = c.anglex;
		radius = c.radius;
	}
	LC_Camera & operator = (LC_Camera & c)
	{
		eye = c.eye;
		at = c.at;
		up = c.up;
		angley = c.angley;
		anglex = c.anglex;
		radius = c.radius;
		return *this;
	}
};



class LCAM{

public:
	LC_Camera cam;
	LC_Camera prevcam;
	LCAM()
	{
		LC_Pointf newPos(0.0f,0.0f,400.0f);
		setUpCamera(newPos, 46, 180);
	}
	LCAM(LC_Pointf pos, float left_right_look_angle, float up_down_look_angle)
	{
		setUpCamera(pos, left_right_look_angle, up_down_look_angle);
	}
	
	LCAM(LCAM & lcam)
	{
		cam = lcam.cam;
		prevcam = lcam.prevcam;
	}

	LCAM & operator = (LCAM & lcam)
	{
		cam = lcam.cam;
		prevcam = lcam.prevcam;
		return *this;
	}

	void setUpCamera(LC_Pointf pos, float LR_LOOK_ANGLE, float UD_LOOK_ANGLE)
	{
		cam = LC_Camera(pos, LC_Pointf(), LC_Pointf(0.0f, 1.0f, 0.0f), UD_LOOK_ANGLE, LR_LOOK_ANGLE, 30.0f);
		refreshCamera();
	}
	void refreshCamera()
	{
		cam.at.x = cam.eye.x + (float)(cam.radius*sin(((double)cam.angley)/57.3));
		cam.at.y = cam.eye.y+(float)(cam.radius*sin(((double)cam.anglex-60.0f)/57.3));
		cam.at.z = cam.eye.z+(float)(cam.radius*cos(((double)cam.angley)/57.3));
	}
	void moveFB(float amount)
	{
		this->backup();
		LC_Pointf incam;
		incam.y = 0.0f;
		incam.x = amount * (float)sin(((double)cam.angley)/57.3);
		incam.z = amount * (float)cos(((double)cam.angley)/57.3);
		cam.eye.x += incam.x;
		cam.eye.y += incam.y;
		cam.eye.z += incam.z;
		this->refreshCamera();
	}
	void moveLR(float amount)
	{
		LC_Pointf incam;
		float properAngle;
		properAngle = cam.angley+90.0f;
		properAngle = (properAngle > 359)?0+(properAngle-360.0f):properAngle;
		incam.y = 0.0f;
		incam.x = amount * (float)sin(((double)properAngle)/57.3);
		incam.z = amount * (float)cos(((double)properAngle)/57.3);
		cam.eye.x += incam.x;
		cam.eye.y += incam.y;
		cam.eye.z += incam.z;
		this->refreshCamera();
	}
	void moveUD(float amount)
	{
		this->backup();
		cam.eye.y += amount;
		this->refreshCamera();
	}
	void moveForwards(float amount)
	{
		this->moveFB(amount);
	}
	void moveBackwards(float amount)
	{
		this->moveFB(-amount);
	}
	void moveLeft(float amount)
	{
		this->moveLR(amount);
	}
	void moveRight(float amount)
	{
		this->moveLR(-amount);
	}
	void lookLeft(float amount)
	{
		cam.angley += 2.0f;
		cam.angley = (float)(((int)cam.angley)%360);
		this->refreshCamera();
	}
	void lookRight(float amount)
	{
		cam.angley -= 2.0f;
		cam.angley = (cam.angley < 0)?360.0f-2.0f:cam.angley;
		this->refreshCamera();
	}
	void lookUp(float amount)
	{
		if(cam.anglex+2.0f < 120)
			cam.anglex += 2.0f;
		this->refreshCamera();
	}
	void lookDown(float amount)
	{
		if(cam.anglex > 0.0f)
			cam.anglex -= 2.0f;
		this->refreshCamera();
	}
	void elavate(float amount)
	{
		this->moveUD(amount);
	}
	void drop(float amount)
	{
		this->moveUD(-amount);
	}
	void backup()
	{
		prevcam = cam;
	}
	void restore()
	{
		cam = prevcam;
	}
};


}


#endif