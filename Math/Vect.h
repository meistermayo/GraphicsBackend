#pragma once

#include "../Align16.h"

#include <xmmintrin.h>
#include <smmintrin.h>  
#include <math.h>

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
	{
	};

	Vect(const Vect& tmp)
	{
		this->_m = _mm_set_ps(tmp.w, tmp.z, tmp.y, tmp.x);
	}

	Vect(__m128 _m)
	{
		this->_m = _m;
	}

	Vect& operator = (const Vect& tmp)
	{
		this->_m = _mm_set_ps(tmp.w, tmp.z, tmp.y, tmp.x);
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
		//return Vect(tmp.x + x, tmp.y + y, tmp.z + z, tmp.w + w); // ?
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
		/*return Vect(
			x - tmp.x,
			y - tmp.y,
			z - tmp.z,
			w - tmp.w); */
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

	/* .. dooo we need this?...
	Vect operator * (const Vect& tmp) const
	{
		//__m128 teMp = _mm_mul_ps(this->_m, tmp._m);
		//return Vect(teMp);

		Vect _tmp = Vect();
		_tmp.x = (y * tmp.z) - (z * tmp.y);
		_tmp.y = (z * tmp.x) - (x * tmp.z);
		_tmp.z = (x * tmp.y) - (y * tmp.x);
		_tmp.w = 1.0f;
		return _tmp;
	}

	Vect& operator *= (const Vect& tmp)
	{
		//	this->_m = _mm_mul_ps(this->_m, tmp._m);
		Vect t = Vect(x, y, z, w);
		x = (t.y * tmp.z) - (t.z * tmp.y);
		y = (t.z * tmp.x) - (t.x * tmp.z);
		z = (t.x * tmp.y) - (t.y * tmp.x);
		w = 1.0f;
		return *this;
	}

	Vect operator / (const Vect& tmp) const
	{
		__m128 teMp = _mm_div_ps(this->_m, tmp._m);
		Vect v(teMp);
		v.w = 1.0f;
		return v;

		//return Vect(x / tmp.x, y / tmp.y, z / tmp.z, w / tmp.w);
	}

	Vect& operator /= (const Vect& tmp)
	{
		this->_m = _mm_div_ps(this->_m, tmp._m);
		this->w = 1.0f;
		return *this;
	}
	*/

	// Vect operator * (const Vect& v);

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
		// check back on this
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
	/*
		__m128 v1 = _mm_set_ps(0.0f, x, z, y); // 1,2,0 ordering to save a shuffle
		__m128 v2 = _mm_set_ps(0.0f, inVect.x, inVect.z, inVect.y);
		__m128 result = _mm_sub_ps(
			_mm_mul_ps(v1, _mm_shuffle_ps(v2, v2, _MM_SHUFFLE(3, 1, 0, 2))),
			_mm_mul_ps(_mm_shuffle_ps(v1, v1, _MM_SHUFFLE(3, 1, 0, 2)), v2)
		);

		Vect cross;
		_mm_store_ps(&cross.x, result);

		return cross;
		*/ // lookin a tthis later OL:::)
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

	//Vect operator * (const Matrix &m) const;
	
	friend Vect operator * (float scale, const Vect& inV); // noted...

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
