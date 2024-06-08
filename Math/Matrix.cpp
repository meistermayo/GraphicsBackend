//----------------------------------------------------------------------------
// Copyright Ed Keenan 2017
// Optimized C++
//----------------------------------------------------------------------------

#include "Vect.h"
#include "Matrix.h"
#include "Quat.h"

const Matrix Matrix::Identity(
	Vect(1.0f, 0.0f, 0.0f, 0.0f),
	Vect(0.0f, 1.0f, 0.0f, 0.0f),
	Vect(0.0f, 0.0f, 1.0f, 0.0f),
	Vect(0.0f, 0.0f, 0.0f, 1.0f)
);

//*/
Matrix Matrix::operator * (const Matrix& t)
{
	return Matrix(
		Vect(
			this->v0.dot(Vect(t.m00, t.m10, t.m20, t.m30)),
			this->v0.dot(Vect(t.m01, t.m11, t.m21, t.m31)),
			this->v0.dot(Vect(t.m02, t.m12, t.m22, t.m32)),
			this->v0.dot(Vect(t.m03, t.m13, t.m23, t.m33))),
		Vect(
			this->v1.dot(Vect(t.m00, t.m10, t.m20, t.m30)),
			this->v1.dot(Vect(t.m01, t.m11, t.m21, t.m31)),
			this->v1.dot(Vect(t.m02, t.m12, t.m22, t.m32)),
			this->v1.dot(Vect(t.m03, t.m13, t.m23, t.m33))),
		Vect(
			this->v2.dot(Vect(t.m00, t.m10, t.m20, t.m30)),
			this->v2.dot(Vect(t.m01, t.m11, t.m21, t.m31)),
			this->v2.dot(Vect(t.m02, t.m12, t.m22, t.m32)), 
			this->v2.dot(Vect(t.m03, t.m13, t.m23, t.m33))),
		Vect(
			this->v3.dot(Vect(t.m00, t.m10, t.m20, t.m30)),
			this->v3.dot(Vect(t.m01, t.m11, t.m21, t.m31)),
			this->v3.dot(Vect(t.m02, t.m12, t.m22, t.m32)),
			1.0f)
	);
}

Matrix& Matrix::operator *= (const Matrix& t)
{
	*this = *this * t;
	return *this;
}

Matrix::Matrix(const Quat& tmp)
{
	// todo : optimize
	float x = tmp.x;
	float y = tmp.y;
	float z = tmp.z;
	float w = tmp.w;

	v0 = Vect(1.f - 2.f*y*y - 2.f*z*z,	2.f*x*y + 2.f*w*z,			2.f*x*z - 2.f*w*y,			0.f);
	v1 = Vect(2.f*x*y - 2.f*w*z,		1.f - 2.f*x*x - 2.f*z*z,	2.f*y*z + 2.f*w*x,			0.f);
	v2 = Vect(2.f*x*z - 2.f*w*y,		2.f*y*z - 2.f*w*x,			1.f -2.f*x*x - 2.f*y*y,		0.f);
	v3 = Vect::Zero;
}
//*/


// ---  End of File ---------------

Matrix operator*(const Matrix& l, const Matrix& r)
{
	return Matrix(
		Vect(
			l.v0.dot(Vect(r.m00, r.m10, r.m20, r.m30)),
			l.v0.dot(Vect(r.m01, r.m11, r.m21, r.m31)),
			l.v0.dot(Vect(r.m02, r.m12, r.m22, r.m32)),
			l.v0.dot(Vect(r.m03, r.m13, r.m23, r.m33))),
		Vect(
			l.v1.dot(Vect(r.m00, r.m10, r.m20, r.m30)),
			l.v1.dot(Vect(r.m01, r.m11, r.m21, r.m31)),
			l.v1.dot(Vect(r.m02, r.m12, r.m22, r.m32)),
			l.v1.dot(Vect(r.m03, r.m13, r.m23, r.m33))),
		Vect(
			l.v2.dot(Vect(r.m00, r.m10, r.m20, r.m30)),
			l.v2.dot(Vect(r.m01, r.m11, r.m21, r.m31)),
			l.v2.dot(Vect(r.m02, r.m12, r.m22, r.m32)),
			l.v2.dot(Vect(r.m03, r.m13, r.m23, r.m33))),
		Vect(
			l.v3.dot(Vect(r.m00, r.m10, r.m20, r.m30)),
			l.v3.dot(Vect(r.m01, r.m11, r.m21, r.m31)),
			l.v3.dot(Vect(r.m02, r.m12, r.m22, r.m32)),
			l.v3.dot(Vect(r.m03, r.m13, r.m23, r.m33)))
	);
}
