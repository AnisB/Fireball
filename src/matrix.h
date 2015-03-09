#ifndef MATRIX_TEMPLATE
#define MATRIX_TEMPLATE

#include "debug.h"
#include "vector.h"

// Include STL
#include <string.h>
#include <iostream>

template<typename T>
class TMatrix3
{
public:
	TMatrix3();
	TMatrix3(const TMatrix3& parMatrix);
	TMatrix3(TVec3<T> parVec0, TVec3<T> parVec1, TVec3<T> parVec2);
	~TMatrix3();

	T det() const;

public:
	T m[9];
};


// Flag d'initialisation des matrices4
namespace MatrixInit
{
	enum Type
	{
		Zero,
		Identity,
		None
	};
}


template<typename T>
class TMatrix4
{
	public:
		TMatrix4(MatrixInit::Type reset = MatrixInit::Zero);
		TMatrix4(const TMatrix4<T>& parMatrix);
		TMatrix4(T a00, T a01, T a02, T a03, T a10, T a11, T a12, T a13, T a20, T a21, T a22, T a23, T a30, T a31, T a32, T a33);
		TMatrix4(const TVec3<T>& parPosition, const TVec4<T>& parQuaternion);
		~TMatrix4();
		
		void setIdentity();
		void resetToZero();
		TVec3<T> getTranslate() const;
		TVec3<T> xAxis() const;
		TVec3<T> yAxis() const;
		TVec3<T> zAxis() const;
		TMatrix4<T> transpose();

		T det() const;
		static TMatrix4<T> inverse(const TMatrix4<T>& parMatrix);
		static TMatrix4<T> translate(const TVec4<T>& parVector);
		static TMatrix4<T> translate(const TVec3<T>& parVector);
		static TMatrix4<T> rotate(T parAngle, const TVec3<T>& parAxis);
		void toTable(float* content) const;
		std::string toString() const;
		void AsPerspective(T parFovy, T parAspect, T parNear, T parFar);

		static TMatrix4<T> rotateXAxis(T parAngle);
		static TMatrix4<T> rotateYAxis(T parAngle);
		static TMatrix4<T> rotateZAxis(T parAngle);

		TMatrix4<T> operator*(const TMatrix4<T>& parMatrix) const;
		TMatrix4<T>& operator=(const TMatrix4<T>& parMatrix);

		
	public:
		T m[16];
};

#include "matrix.ih"

#endif //MATRIX_TEMPLATE
