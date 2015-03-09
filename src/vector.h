#ifndef VECTOR
#define VECTOR

#include <math.h>
#include <iostream>


template<typename T>
struct TVec2
{
	TVec2()
	{

	}
	TVec2(T parVal)
	{
		x = parVal;
		y = parVal;
	}
	TVec2(T parX, T parY)
	{
		x = parX;
		y = parY;
	}
	TVec2(const TVec2& parVec)
	{
		x = parVec.x;
		y = parVec.y;
	}
	T norm() const
	{
		return sqrt(x*x + y*y );
	}
	T x,y;
};


template<typename T>
struct TVec3
{
	TVec3()
	{

	}
	TVec3(T parVal)
	{
		x = parVal;
		y = parVal;
		z = parVal;
	}
	TVec3(T parX, T parY, T parZ)
	{
		x = parX;
		y = parY;
		z = parZ;
	}
	TVec3(const TVec3& parVec)
	{
		x = parVec.x;
		y = parVec.y;
		z = parVec.z;
	}
	T norm() const
	{
		return sqrt(x*x + y*y +z*z);
	}
	T x,y,z;
};

template<typename T>
struct TVec4
{
	TVec4(T parVal)
	{
		x = parVal;
		y = parVal;
		z = parVal;
		w = parVal;
	}
	TVec4(const TVec4& parV)
	{
		x = parV.x;
		y = parV.y;
		z = parV.z;
		w = parV.w;
	}
	TVec4(T parX, T parY, T parZ, T parW)
	{
		x = parX;
		y = parY;
		z = parZ;
		w = parW;
	}
	T norm() const
	{
		return sqrt(x*x + y*y + z*z + w*w);
	}
	T x,y,z,w;
};

#include "vector.ih"

typedef TVec2<double> TVec2d;
typedef TVec2<float> TVec2f;

typedef TVec3<double> TVec3d;
typedef TVec3<float> TVec3f;

typedef TVec4<double> TVec4d;
typedef TVec4<float> TVec4f;

#endif // VECTOR