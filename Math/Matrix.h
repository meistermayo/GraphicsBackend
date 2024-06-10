#pragma once

#include "../Align16.h"
#include "./Vect.h"

#include <corecrt_math.h>
#include <xmmintrin.h>
#include <smmintrin.h> 
#include <cassert>

#pragma warning(disable : 4201)

class Quat;

class Matrix : public Align16
{
public:
	static const Matrix Identity;

	Matrix()
	{
	}

	Matrix(const Quat& inQuat);

	Matrix(const Matrix& inTmp)
		:v0(inTmp.v0), v1(inTmp.v1), v2(inTmp.v2), v3(inTmp.v3)
	{
	}

	Matrix(__m128 inV0, __m128 inV1, __m128 inV2, __m128 inV3)
	{
		this->v0 = Vect(inV0);
		this->v1 = Vect(inV1);
		this->v2 = Vect(inV2);
		this->v3 = Vect(inV3);
	}

	Matrix operator=(const Matrix& inTmp)
	{
		v0 = inTmp.v0;
		v1 = inTmp.v1;
		v2 = inTmp.v2;
		v3 = inTmp.v3;

		return *this;
	}

	~Matrix()
	{
	}

	Matrix(const Vect& inV0, const Vect& inV1, const Vect& inV2, const Vect& inV3)
		:v0(inV0), v1(inV1), v2(inV2), v3(inV3)
	{
	}

	static Matrix Trans(const Vect& inTrans) { return Matrix(Vect::Right_0, Vect::Up_0, Vect::Forward_0, inTrans); }
	static Matrix Trans(float x, float y, float z) { return Matrix(Vect::Right_0, Vect::Up_0, Vect::Forward_0, Vect(x,y,z)); }
	static Matrix Scale(float s) { return Matrix(Vect::Right_0 * s, Vect::Up_0 * s, Vect::Forward_0 * s, Vect::Zero); }
	static Matrix Scale(float x, float y, float z) { return Matrix(Vect::Right_0 * x, Vect::Up_0 * y, Vect::Forward_0 * z, Vect::Zero); }
	static Matrix Scale(const Vect& inScale) { return Matrix(Vect::Right_0 * inScale.x, Vect::Up_0 * inScale.y, Vect::Forward_0 * inScale.z, Vect::Zero); }

	static Matrix RotAxisAngle(const Vect& inAxis, float inAngle) {
		
		float c = cosf(inAngle);
		float s = sinf(inAngle);
		float t = 1.f - c;

		Vect naxis = inAxis.GetNormalized();
		float x = naxis.x, y = naxis.y, z = naxis.z;

		// opt me
		return Matrix(
			Vect(t*x*x + c,		t*x*y + s*z,	t*x*z - s*y,	0.f),
			Vect(t*x*y - s*z,	t*y*y + c,		t*y*z + s*x,	0.f),
			Vect(t*x*z + s*y,	t*y*z - s*x,	t*z*z + c,		0.f),
			Vect::Zero
		);
	}
	
	static Matrix Matrix::RotOrient(const Vect& newFwd, const Vect& newUp)
	{
		return Matrix (
			newUp.cross(newFwd),
			newUp,
			newFwd,
			Vect::Zero
		);
	}

	Matrix GetOrthoNormalInverse() const
	{
		Matrix m = *this;
		m.Flip(m.m01, m.m10);
		m.Flip(m.m02, m.m20);
		m.Flip(m.m21, m.m12);
		return m;
	}

	Matrix GetWorldInverse() const
	{
		Matrix m = GetOrthoNormalInverse();
		m.v3 = -m.v3;
		return m;
	}

	Matrix GetTranspose() const
	{
		Matrix m = *this;
		m.Transpose();
		return m;
	}

	void Transpose()
	{
		Flip(m01, m10);
		Flip(m02, m20);
		Flip(m03, m30);

		Flip(m21, m12);
		Flip(m31, m13);

		Flip(m32, m23);
	}

	void Flip(float& a, float& b)
	{
		float tmp = a;
		a = b;
		b = tmp;
	}

	// opt me
	static Matrix RotX(float inAngle) { return RotAxisAngle(Vect::Right, inAngle); }
	static Matrix RotY(float inAngle) { return RotAxisAngle(Vect::Up, inAngle); }
	static Matrix RotZ(float inAngle) { return RotAxisAngle(Vect::Forward, inAngle); }

	void SetTrans(const Vect& inTrans) { v3 = inTrans; }
	const Vect& GetTrans() const { return v3; }

	friend Matrix operator * (const Matrix& l, const Matrix& r);
	Matrix operator * (const Matrix& t);

	Matrix operator * (float inScale)
	{
		return Matrix(
			v0 * inScale,
			v1 * inScale,
			v2 * inScale,
			v3 * inScale
		);
	}

	Matrix& operator *= (const Matrix& t);

	const Vect& GetRow0() const { return v0; }
	const Vect& GetRow1() const { return v1; }
	const Vect& GetRow2() const { return v2; }

	void SetRow0(const Vect& inRow) { v0 = inRow; }
	void SetRow1(const Vect& inRow) { v1 = inRow; }
	void SetRow2(const Vect& inRow) { v2 = inRow; }

	operator Vect() = delete;

	float& operator[](int i)
	{
		assert(i >= 0 && i < 16 && "Matrix was indexed outside of its range!");
		return (&m00)[i];
	}

	union
	{
		struct
		{
			Vect v0;
			Vect v1;
			Vect v2;
			Vect v3;
		};

		struct
		{
			float m00;
			float m01;
			float m02;
			float m03;
			float m10;
			float m11;
			float m12;
			float m13;
			float m20;
			float m21;
			float m22;
			float m23;
			float m30;
			float m31;
			float m32;
			float m33;
		};
	};
};

#pragma warning(default : 4201)
