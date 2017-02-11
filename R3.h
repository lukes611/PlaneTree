#ifndef LINCOLN_R3_H
#define LINCOLN_R3_H

#include <iostream>
#include <math.h>
#define RAD2DEG 57.2957795f



namespace LINCOLN
{

	void GetUnitPointFromAngle(float angle, float& x, float& y);
	float getAngle(float x, float y);
	
	class R3
	{
	public:
		float x, y, z;
		R3(); // initializes the vector to the origin
		R3(float scalar); //initializes all three elements to this value
		R3(float xin, float yin, float zin); 
		R3(float xin, float yin); //first two elems become xin and yin
		R3(const R3& other_vector);
		R3(const R3* other_vector);
		R3(R3 center, float angle1, float angle2, float radius);
		R3 getCopy();
		void setAs(const R3& inp);
		//I/O
		void print();
		friend std::ostream& operator<<(std::ostream& stream, const R3& input);

		//functions
		bool operator == (const R3 & rs);
		friend R3 operator+(const R3& leftside, const R3& rightside);
		friend R3 operator-(const R3& leftside, const R3& rightside);
		friend float operator*(const R3& leftside, const R3& rightside); // dot product
		friend R3 operator^(const R3& leftside, const R3& rightside); // cross product
		friend R3 operator*(const R3& leftside, float rightside); // scale
		friend R3 operator/(const R3& leftside, float rightside); //attenuate
		friend R3 operator!(const R3& leftside); //invert

		void scale(float s);
		void add(R3 inp);
		void subtract(R3 inp);
		void crossProduct(R3 inp);
		void crossProduct_s(R3 inp);
		float DotProduct(R3 inp) const;
		void invert();
		void Normalize();
		float Magnitude() const;
		R3 getNormalizedCopy() const;


		void operator+=(R3 inp);
		void operator-=(R3 inp);
		void operator*=(R3 inp);
		void operator^=(R3 inp);
		void operator*=(float inp);
		void operator/=(float inp);
		float& operator[](int index);

		R3& operator=(float f);
		R3& operator=(const R3& inp);
		R3& operator=(R3* inp);
		
		R3 interpolateTo(R3 target, float time);




	};


	class Quarternion
	{
	public:
		float x, y, z, w;
		Quarternion();
		Quarternion(float xin, float yin, float zin, float win);
		Quarternion(float angle1, float angle2);
		Quarternion(R3 pole, float angle);
		Quarternion(Quarternion& inp);
		Quarternion(Quarternion* inp);
		Quarternion getCopy() const;
		void setAs(Quarternion& inp);
		Quarternion& operator=(const Quarternion& inp);
		float DotProduct(Quarternion inp) const;
		Quarternion getConjugate();
		Quarternion getInverse();
		void MultiplyBy(Quarternion inp);
		Quarternion getMultiplicationBy(Quarternion inp);
		float getMagnitude() const;
		Quarternion interpolateTo(Quarternion target, float time) const;
		R3 rotate(R3 inp);
		void print();
		friend std::ostream& operator<<(std::ostream& stream, const Quarternion& input);
		friend Quarternion operator*(const Quarternion& a, const Quarternion& b);
		void operator*=(Quarternion inp);
		void add(Quarternion inp);
		void scale(float inp);
	};
	

	









}



#endif