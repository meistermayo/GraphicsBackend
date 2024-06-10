#pragma once

#include "../Align16.h"

#include <xmmintrin.h>
#include <smmintrin.h>  
#include <math.h>

#pragma warning(disable : 4201)

class Matrix;

class Vect : public Align16
{
	/*
	friend struct vM;
	friend struct vM2;
	friend struct vM3;
	friend struct vM4;

	friend inline vM operator * (const Vect& _v1, const Matrix& _m1);
	friend inline vM2 operator * (const vM& _vM, const Matrix& _m1);
	friend inline vM3 operator * (const vM2& _vM, const Matrix& _m1);
	friend inline vM4 operator * (const vM3& _vM, const Matrix& _m1);
	*/

public:
	static const Vect Right;
	static const Vect Up;
	static const Vect Forward;
	static const Vect Zero;
	static const Vect One;

	static const Vect Right_0;
	static const Vect Up_0;
	static const Vect Forward_0;
	static const Vect Zero_0;
	static const Vect One_0;

	Vect()
		: x(0.f), y(0.f), z(0.f), w(0.f)
	{
	};

	Vect(const Vect& inVect)
	{
		this->_m = _mm_set_ps(inVect.w, inVect.z, inVect.y, inVect.x);
	}

	Vect(__m128 _m)
	{
		this->_m = _m;
	}

	Vect& operator = (const Vect& inVect)
	{
		this->_m = _mm_set_ps(inVect.w, inVect.z, inVect.y, inVect.x);
		return *this;
	}

	~Vect()
	{
	};


	Vect(float tx, float ty, float tz, float tw = 1.0f)
	{
		this->_m = _mm_set_ps(tw, tz, ty, tx);
	}

	void set(float xVal, float yVal, float zVal, float wVal)
	{
		this->_m = _mm_set_ps(wVal, zVal, yVal, xVal);
	}

	void set(float xVal, float yVal, float zVal)
	{
		this->_m = _mm_set_ps(w, zVal, yVal, xVal);
	}

	Vect operator + (const Vect& tmp) const
	{
		__m128 teMp = _mm_add_ps(this->_m, tmp._m);

		Vect v(teMp);
		v.w = 1.0f;
		return v;
	}

	Vect& operator += (const Vect& tmp)
	{
		this->_m = _mm_add_ps(this->_m, tmp._m);
		this->w = 1.0f;
		return *this;
	}

	Vect operator - (const Vect& tmp) const
	{
		__m128 teMp = _mm_sub_ps(this->_m, tmp._m);
		Vect v(teMp);
		v.w = 1.0f;
		return v;
	}

	Vect& operator -= (const Vect& tmp)
	{
		this->_m = _mm_sub_ps(this->_m, tmp._m);
		this->w = 1.0f;
		return *this;
	}

	float dot(const Vect& t) const
	{
		// Only do the inner product for {x,y,z}
		//_mm_dp_ps(this->_m, t._m, 0xE);
		return (x * t.x) + (y * t.y) + (z * t.z) + (w * t.w);
	}

	void Normalize()
	{
		float n = 1.0f / sqrtf(x * x + y * y + z * z);

		x *= n;
		y *= n;
		z *= n;
		w = 1.0f;
	}

	float GetMag() const { return sqrtf(x * x + y * y + z * z); }
	float GetSqrMag() const { return x * x + y * y + z * z; }

	Vect GetNormalized() const
	{
		Vect v = *this;
		v.Normalize();
		return v;
	}

	Vect cross(const Vect& inVect) const
	{
		return Vect(
			y * inVect.z - z * inVect.y,
			z * inVect.x - x * inVect.z,
			x * inVect.y - y * inVect.x
		);
	}

	static float fastInvSqrt(float x) {
		float xhalf = 0.5f * x;
		int i = *(int*)&x; // based floating point bit level magic
		i = 0x5f3759df - (i >> 1); // delightful! 
		x = *(float*)&i;
		x = x * (1.5f - xhalf * x * x); // 1st iteration
		return x;
	}

	Vect operator-() const { return { -x,-y,-z, w }; }

	Vect operator * (double s) const = delete;
	Vect operator * (float s) const
	{
		return Vect(x * s, y * s, z * s, w);
	}

	Vect operator * (const Matrix& tmp) const;
	Vect operator *= (const Matrix& tmp) const;

	friend Vect operator * (float scale, const Vect& inV);

	static void VectLerp(const Vect& a, const Vect& b, const float t, Vect& outVect)
	{
		outVect = a + ((b - a) * t);
	}

public:

	union
	{
		__m128	_m;

		struct
		{
			float x;
			float y;
			float z;
			float w;
		};
	};
};

#pragma warning(default : 4201)
