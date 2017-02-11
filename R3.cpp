#include "R3.h"

namespace LINCOLN
{
	void GetUnitPointFromAngle(float angle, float& x, float& y){
		angle /= RAD2DEG;
		x = cos(angle);
		y = sin(angle);
	}

	float getAngle(float x, float y){
		float angle = RAD2DEG * atan(y / x);
		if (x < 0.0f){
			angle += 180.0f;
		}
		else if (y < 0.0f){
			angle += 360.0f;
		}
		return angle;
	}

}


LINCOLN::R3::R3()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}
LINCOLN::R3::R3(float scalar)
{
	x = scalar;
	y = scalar;
	z = scalar;

}
LINCOLN::R3::R3(float xin, float yin)
{
	x = xin;
	y = yin;
	z = 0.0f;
}

LINCOLN::R3::R3(float xin, float yin, float zin)
{
	x = xin;
	y = yin;
	z = zin;
}

LINCOLN::R3::R3(const R3& other_vector)
{
	x = other_vector.x;
	y = other_vector.y;
	z = other_vector.z;
}

LINCOLN::R3::R3(const R3* other_vector)
{
	x = other_vector->x;
	y = other_vector->y;
	z = other_vector->z;
}

LINCOLN::R3::R3(R3 center, float angle1, float angle2, float radius)
{
	float x = 0.0f, y = 0.0f, x2 = 0.0f, y2 = 0.0f;
	GetUnitPointFromAngle(angle1, x, y);
	GetUnitPointFromAngle(angle2, x2, y2);
	R3 v1(x, 0.0f, y);
	R3 up(0.0f, 1.0f, 0.0f);
	v1 *= x2;
	up *= y2;
	R3 tv = (((v1 + up) * radius) + center);
	this->setAs(tv);
}

bool LINCOLN::R3::operator == (const R3 & rs)
{
	return x==rs.x && y==rs.y && z==rs.z;
}

LINCOLN::R3 LINCOLN::R3::getCopy()
{
	R3 rv = this;
	return rv;
}

void LINCOLN::R3::setAs(const R3& inp)
{
	x = inp.x;
	y = inp.y;
	z = inp.z;
}

//I/O
void LINCOLN::R3::print()
{
	printf("%.2f, %.2f %.2f\n", x, y, z);
}

namespace LINCOLN{

	std::ostream& operator<<(std::ostream& stream, const R3& input)
	{
		stream << input.x << ", " << input.y << ", " << input.z;
		return stream;
	}

	R3 operator+(const R3& leftside, const R3& rightside)
	{
		R3 rv = leftside;
		rv.add(rightside);
		return rv;
	}

	R3 operator-(const R3& leftside, const R3& rightside)
	{
		R3 rv = leftside;
		rv.subtract(rightside);
		return rv;
	}
	float operator*(const R3& leftside, const R3& rightside)
	{
		return leftside.DotProduct(rightside);
	}
	R3 operator^(const R3& leftside, const R3& rightside)
	{
		R3 rv = leftside;
		rv.crossProduct_s(rightside);
		return rv;
	}
	R3 operator*(const R3& leftside, float rightside)
	{
		R3 rv = leftside;
		rv.scale(rightside);
		return rv;
	}
	R3 operator/(const R3& leftside, float rightside)
	{
		R3 rv = leftside;
		rv.scale(1.0f/rightside);
		return rv;
	}
	R3 operator!(const R3& leftside)
	{
		R3 rv = leftside;
		rv.invert();
		return rv;
	}

}



void LINCOLN::R3::scale(float s)
{
	x *= s;
	y *= s;
	z *= s;
}
void LINCOLN::R3::add(R3 inp)
{
	x += inp.x;
	y += inp.y;
	z += inp.z;
}
void LINCOLN::R3::subtract(R3 inp)
{
	x -= inp.x;
	y -= inp.y;
	z -= inp.z;
}
void LINCOLN::R3::crossProduct(R3 inp)
{
	R3 cp = this;
	x = cp.y*inp.z - cp.z*inp.y;
	y = cp.z*inp.x - cp.x*inp.z;
	z = cp.x*inp.y - cp.y*inp.x;
}
void LINCOLN::R3::crossProduct_s(R3 inp)
{

	R3 cp = this;
	cp.Normalize();
	inp.Normalize();
	x = cp.y*inp.z - cp.z*inp.y;
	y = cp.z*inp.x - cp.x*inp.z;
	z = cp.x*inp.y - cp.y*inp.x;
}
float LINCOLN::R3::DotProduct(R3 inp) const
{
	return x*inp.x + y*inp.y + z*inp.z;
}
void LINCOLN::R3::invert()
{
	x = -x;
	y = -y;
	z = -z;
}
void LINCOLN::R3::Normalize()
{
	float s = 1.0f / this->Magnitude();
	this->scale(s);
}
float LINCOLN::R3::Magnitude() const
{
	return sqrt(pow(x, 2.0f) + pow(y, 2.0f) + pow(z, 2.0f));
}
LINCOLN::R3 LINCOLN::R3::getNormalizedCopy() const
{
	R3 rv = this;
	rv.Normalize();
	return rv;
}

void LINCOLN::R3::operator+=(R3 inp)
{
	this->add(inp);
}
void LINCOLN::R3::operator-=(R3 inp)
{
	this->subtract(inp);
}
void LINCOLN::R3::operator*=(R3 inp)
{
	this->DotProduct(inp);
}
void LINCOLN::R3::operator^=(R3 inp)
{
	this->crossProduct_s(inp);
}
void LINCOLN::R3::operator*=(float inp)
{
	this->scale(inp);
}
void LINCOLN::R3::operator/=(float inp)
{
	x /= inp;
	y /= inp;
	z /= inp;
}


LINCOLN::R3& LINCOLN::R3::operator=(float f)
{
	x = f;
	y = f;
	z = f;
	return *this;
}
LINCOLN::R3& LINCOLN::R3::operator=(const R3& inp)
{
	x = inp.x;
	y = inp.y;
	z = inp.z;
	return *this;
}
LINCOLN::R3& LINCOLN::R3::operator=(R3* inp)
{
	x = inp->x;
	y = inp->y;
	z = inp->z;
	return *this;
}

LINCOLN::R3 LINCOLN::R3::interpolateTo(R3 target, float time)
{
	R3 to = target;
	to -= (*this);
	return (*this) + (to * time);

}

float& LINCOLN::R3::operator[](int index)
{
	switch (index)
	{
		case 1: return y;
		case 2: return z;
		default: return x;
	}
}


//Quarternion Class:

LINCOLN::Quarternion::Quarternion()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	w = 0.0f;
}
LINCOLN::Quarternion::Quarternion(float xin, float yin, float zin, float win)
{
	x = xin;
	y = yin;
	z = zin;
	w = win;
}
LINCOLN::Quarternion::Quarternion(float angle1, float angle2)
{
	R3 z_axis(0.0f, 0.0f, 1.0f);
	Quarternion q1 = Quarternion(R3(0.0f, 1.0f, 0.0f), angle1);
	R3 rotation_number_2_direction = q1.rotate(z_axis);
	Quarternion q2 = Quarternion(rotation_number_2_direction, angle2);
	this->setAs(q2);
}
LINCOLN::Quarternion LINCOLN::Quarternion::getCopy() const
{
	Quarternion qt(x,y,z,w);
	return qt;
}
LINCOLN::Quarternion::Quarternion(R3 pole, float angle)
{
	angle /= (2.0f*RAD2DEG);
	w = cos(angle);
	x = sin(angle);
	pole = pole * x;
	x = pole.x;
	y = pole.y;
	z = pole.z;
}
LINCOLN::Quarternion::Quarternion(Quarternion& inp)
{
	x = inp.x;
	y = inp.y;
	z = inp.z;
	w = inp.w;
}
LINCOLN::Quarternion::Quarternion(Quarternion* inp)
{
	x = inp->x;
	y = inp->y;
	z = inp->z;
	w = inp->w;
}
LINCOLN::Quarternion& LINCOLN::Quarternion::operator=(const Quarternion& inp)
{
	x = inp.x;
	y = inp.y;
	z = inp.z;
	w = inp.w;
	return (*this);
}
void LINCOLN::Quarternion::setAs(Quarternion& inp)
{
	x = inp.x;
	y = inp.y;
	z = inp.z;
	w = inp.w;
}
float LINCOLN::Quarternion::DotProduct(Quarternion inp) const
{
	return x*inp.x + y*inp.y + z*inp.z + w*inp.w;
}
LINCOLN::Quarternion LINCOLN::Quarternion::getConjugate()
{
	Quarternion qt = this;
	qt.x = -qt.x;
	qt.y = -qt.y;
	qt.z = -qt.z;
	return qt;
}
LINCOLN::Quarternion LINCOLN::Quarternion::getInverse()
{
	Quarternion q1 = this;
	float dp = q1.DotProduct(q1);
	if (dp == 0.0f){
		return Quarternion(0.0f, 0.0f, 0.0f, 0.0f);
	}
	float mag = 1.0f / dp;
	//float mag = 1.0f / qt_dot(q1,q1);
	q1 = q1.getConjugate();
	q1.x *= mag;
	q1.y *= mag;
	q1.z *= mag;
	q1.w *= mag;
	return q1;
}
void LINCOLN::Quarternion::MultiplyBy(Quarternion inp)
{
	this->setAs(this->getMultiplicationBy(inp));
}
LINCOLN::Quarternion LINCOLN::Quarternion::getMultiplicationBy(Quarternion q2)
{
	Quarternion rv, q1 = this;
	rv.w = q1.w*q2.w - q1.x*q2.x - q1.y*q2.y - q1.z*q2.z;
	rv.x = q1.w*q2.x + q1.x*q2.w + q1.y*q2.z - q1.z*q2.y;
	rv.y = q1.w*q2.y - q1.x*q2.z + q1.y*q2.w + q1.z*q2.x;
	rv.z = q1.w*q2.z + q1.x*q2.y - q1.y*q2.x + q1.z*q2.w;
	return rv;
}
float LINCOLN::Quarternion::getMagnitude() const
{
	return sqrt(pow(x, 2.0f) + pow(y, 2.0f) + pow(z, 2.0f) + pow(w, 2.0f));
}
LINCOLN::Quarternion LINCOLN::Quarternion::interpolateTo(Quarternion target, float time) const
{
	Quarternion from(x,y,z,w);
	float t2 = 1.0f - time;
	from.scale(t2);
	target.scale(time);
	Quarternion out = from;
	out.add(target);
	float re_scale = out.getMagnitude();
	out.scale(1.0f / re_scale);
	return out;
}
LINCOLN::R3 LINCOLN::Quarternion::rotate(R3 v)
{
	Quarternion q1 = this;
	Quarternion q1_inv = q1.getInverse();
	Quarternion v_q(v.x, v.y, v.z, 0.0f);
	Quarternion m1 = q1.getMultiplicationBy(v_q);
	v_q = m1.getMultiplicationBy(q1_inv);
	v.x = v_q.x;
	v.y = v_q.y;
	v.z = v_q.z;
	return v;
}
void LINCOLN::Quarternion::print()
{
	printf("%.2f %.2f %.2f %.2f\n", x,y,z,w);
}
void LINCOLN::Quarternion::operator*=(Quarternion inp)
{
	this->MultiplyBy(inp);
}
void LINCOLN::Quarternion::add(Quarternion inp)
{
	x += inp.x;
	y += inp.y;
	z += inp.z;
	w += inp.w;
}
void LINCOLN::Quarternion::scale(float inp)
{
	x *= inp;
	y *= inp;
	z *= inp;
	w *= inp;
}
namespace LINCOLN
{

	std::ostream& operator<<(std::ostream& stream, const Quarternion& input)
	{
		stream << input.x << " " << input.y << " " << input.z << " " << input.w;
		return stream;
	}
	Quarternion operator*(const Quarternion& a, const Quarternion& b)
	{
		Quarternion cpa = a.getCopy();
		return cpa.getMultiplicationBy(b.getCopy());
	}
	

}