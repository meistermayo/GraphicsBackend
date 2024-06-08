#pragma once
#pragma warning(disable : 4201)

#include "../Align16.h"
#include "./Vect.h"

#include <corecrt_math.h>
#include <xmmintrin.h>
#include <smmintrin.h> 
#include <cassert>

class Quat;

class Matrix : public Align16
{
	/*/
	friend struct vM;
	friend struct vM2;
	friend struct vM3;
	friend struct vM4;

	friend inline vM operator * (const Vect& _v1, const Matrix& _m1);
	friend inline vM2 operator * (const vM& _vM, const Matrix& _m1);
	friend inline vM3 operator * (const vM2& _vM, const Matrix& _m1);
	friend inline vM4 operator * (const vM3& _vM, const Matrix& _m1);

	friend struct Mv;
	friend struct MM;
	friend struct MMM;
	friend struct MMMM;
	friend struct MMMMM;
	friend struct MMMMMv;

	friend inline Mv operator * (const Matrix &_m1, const Vect &_v1);
	friend inline MM operator * (const Matrix &_m1, const Matrix &_m2);
	friend inline MMM operator * (const MM &_mm, const Matrix &_m2);
	friend inline MMMM operator * (const MMM &_mm, const Matrix &_m2);
	friend inline MMMMM operator * (const MMMM &_mm, const Matrix &_m2);
	friend inline MMMMMv operator * (const MMMMM &_mm, const Vect &_v1);
	//*/

public:
	static const Matrix Identity;

	Matrix()
	{
	}

	Matrix(const Quat& tmp);

	Matrix(const Matrix& tmp)
		:v0(tmp.v0), v1(tmp.v1), v2(tmp.v2), v3(tmp.v3)
	{
	}

	Matrix(__m128 _v0, __m128 _v1, __m128 _v2, __m128 _v3)
	{
		this->v0 = Vect(_v0);
		this->v1 = Vect(_v1);
		this->v2 = Vect(_v2);
		this->v3 = Vect(_v3);
	}

	Matrix operator=(const Matrix& tmp)
	{
		v0 = tmp.v0;
		v1 = tmp.v1;
		v2 = tmp.v2;
		v3 = tmp.v3;

		return *this;
	}

	~Matrix()
	{
	}

	//optimize me
	Matrix(const Vect& tV0, const Vect& tV1, const Vect& tV2, const Vect& tV3)
		:v0(tV0), v1(tV1), v2(tV2), v3(tV3)
	{
	}

	static Matrix Trans(const Vect& t) { return Matrix(Vect::Right_0, Vect::Up_0, Vect::Forward_0, t); }
	static Matrix Trans(float x, float y, float z) { return Matrix(Vect::Right_0, Vect::Up_0, Vect::Forward_0, Vect(x,y,z)); }
	static Matrix Scale(float s) { return Matrix(Vect::Right_0 * s, Vect::Up_0 * s, Vect::Forward_0 * s, Vect::Zero); }
	static Matrix Scale(float x, float y, float z) { return Matrix(Vect::Right_0 * x, Vect::Up_0 * y, Vect::Forward_0 * z, Vect::Zero); }
	static Matrix Scale(const Vect& s) { return Matrix(Vect::Right_0 * s.x, Vect::Up_0 * s.y, Vect::Forward_0 * s.z, Vect::Zero); }

	static Matrix RotAxisAngle(const Vect& axis, float angle) {
		
		float c = cosf(angle);
		float s = sinf(angle);
		float t = 1.f - c;

		Vect naxis = axis.GetNormalized();
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
		// maybe need to uhhhhhh this? todo
		return Matrix (
			newUp.cross(newFwd),
			newUp,
			newFwd,
			Vect::Zero
		);
	}

	Matrix GetInverse() const
	{
		return Matrix(); // to dooo
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
	static Matrix RotX(float angle) { return RotAxisAngle(Vect::Right, angle); }
	static Matrix RotY(float angle) { return RotAxisAngle(Vect::Up, angle); }
	static Matrix RotZ(float angle) { return RotAxisAngle(Vect::Forward, angle); }

	void SetTrans(const Vect& t) { v3 = t; }
	const Vect& GetTrans() const { return v3; }

	friend Matrix operator * (const Matrix& l, const Matrix& r);
	Matrix operator * (const Matrix& t);

	Matrix operator * (float s)
	{
		return Matrix(
			v0 * s,
			v1 * s,
			v2 * s,
			v3 * s
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
/*
#pragma region
struct vM {
	const Vect v1;
	const Matrix m1;

	vM(const Vect& _v1, const Matrix& _m1)
		: v1(_v1), m1(_m1) {

	}

	vM(const vM& tmp) = default;
	vM& operator = (const vM& tmp) = delete;
	~vM() = default;

	operator Vect()
	{
		return Vect
		(
			_mm_add_ps(
				_mm_add_ps(
					_mm_mul_ps(
						_mm_set_ps1(v1.x),
						m1.v0._m
					),
					_mm_mul_ps(
						_mm_set_ps1(v1.y),
						m1.v1._m
					)
				),
				_mm_add_ps(
					_mm_mul_ps(
						_mm_set_ps1(v1.z),
						m1.v2._m
					),
					_mm_mul_ps(
						_mm_set_ps1(v1.w),
						m1.v3._m
					)
				)
			)
		);
	}
};

struct vM2 {
	const Vect v1;
	const Matrix m1, m2;

	vM2(const vM _vM, const Matrix& _m1)
		: v1(_vM.v1), m1(_vM.m1), m2(_m1) {

	}

	vM2(const vM2& tmp) = default;
	vM2& operator = (const vM2& tmp) = delete;
	~vM2() = default;

	operator Vect()
	{
		Vect tmp
		(_mm_add_ps(
			_mm_add_ps(
				_mm_mul_ps(
					_mm_set_ps1(v1.x),
					_mm_set_ps(m1.m0, m1.m4, m1.m8, m1.m12)),
				_mm_mul_ps(
					_mm_set_ps1(v1.y),
					_mm_set_ps(m1.m1, m1.m5, m1.m9, m1.m13))
			),
			_mm_add_ps(
				_mm_mul_ps(
					_mm_set_ps1(v1.z),
					_mm_set_ps(m1.m2, m1.m6, m1.m10, m1.m14)),
				_mm_mul_ps(
					_mm_set_ps1(v1.w),
					_mm_set_ps(m1.m3, m1.m7, m1.m11, m1.m15))
			)
		)
		);

		return Vect(
			_mm_add_ps(
				_mm_add_ps(
					_mm_mul_ps(
						_mm_set_ps1(v1.x),
						_mm_set_ps(m2.m0, m2.m4, m2.m8, m2.m12)),
					_mm_mul_ps(
						_mm_set_ps1(v1.y),
						_mm_set_ps(m2.m1, m2.m5, m2.m9, m2.m13))
				),
				_mm_add_ps(
					_mm_mul_ps(
						_mm_set_ps1(v1.z),
						_mm_set_ps(m2.m2, m2.m6, m2.m10, m2.m14)),
					_mm_mul_ps(
						_mm_set_ps1(v1.w),
						_mm_set_ps(m2.m3, m2.m7, m2.m11, m2.m15))
				)
			)
		);
	}
};

struct vM3 {
	const Vect v1;
	const Matrix m1, m2, m3;

	vM3(const vM2 _vM, const Matrix& _m1)
		: v1(_vM.v1), m1(_vM.m1), m2(_vM.m2), m3(_m1) {

	}

	vM3(const vM3& tmp) = default;
	vM3& operator = (const vM3& tmp) = delete;
	~vM3() = default;

	operator Vect()
	{
		Vect tmp
		(
			_mm_add_ps(
				_mm_add_ps(
					_mm_mul_ps(
						_mm_set_ps1(v1.x),
						_mm_set_ps(m1.m0, m1.m4, m1.m8, m1.m12)),
					_mm_mul_ps(
						_mm_set_ps1(v1.y),
						_mm_set_ps(m1.m1, m1.m5, m1.m9, m1.m13))
				),
				_mm_add_ps(
					_mm_mul_ps(
						_mm_set_ps1(v1.z),
						_mm_set_ps(m1.m2, m1.m6, m1.m10, m1.m14)),
					_mm_mul_ps(
						_mm_set_ps1(v1.w),
						_mm_set_ps(m1.m3, m1.m7, m1.m11, m1.m15))
				)
			)
		);

		tmp = Vect
		(
			_mm_add_ps(
				_mm_add_ps(
					_mm_mul_ps(
						_mm_set_ps1(v1.x),
						_mm_set_ps(m2.m0, m2.m4, m2.m8, m2.m12)),
					_mm_mul_ps(
						_mm_set_ps1(v1.y),
						_mm_set_ps(m2.m1, m2.m5, m2.m9, m2.m13))
				),
				_mm_add_ps(
					_mm_mul_ps(
						_mm_set_ps1(v1.z),
						_mm_set_ps(m2.m2, m2.m6, m2.m10, m2.m14)),
					_mm_mul_ps(
						_mm_set_ps1(v1.w),
						_mm_set_ps(m2.m3, m2.m7, m2.m11, m2.m15))
				)
			)
		);

		return Vect(
			_mm_add_ps(
				_mm_add_ps(
					_mm_mul_ps(
						_mm_set_ps1(v1.x),
						_mm_set_ps(m3.m0, m3.m4, m3.m8, m3.m12)),
					_mm_mul_ps(
						_mm_set_ps1(v1.y),
						_mm_set_ps(m3.m1, m3.m5, m3.m9, m3.m13))
				),
				_mm_add_ps(
					_mm_mul_ps(
						_mm_set_ps1(v1.z),
						_mm_set_ps(m3.m2, m3.m6, m3.m10, m3.m14)),
					_mm_mul_ps(
						_mm_set_ps1(v1.w),
						_mm_set_ps(m3.m3, m3.m7, m3.m11, m3.m15))
				)
			)
		);
	}
};

struct vM4 {
	const Vect v1;
	const Matrix m1, m2, m3, m4;

	vM4(const vM3 _vM, const Matrix& _m1)
		: v1(_vM.v1), m1(_vM.m1), m2(_vM.m2), m3(_vM.m3), m4(_m1) {

	}

	vM4(const vM4& tmp) = default;
	vM4& operator = (const vM4& tmp) = delete;
	~vM4() = default;

	operator Vect()
	{
		Vect tmp
		(
			_mm_add_ps(
				_mm_add_ps(
					_mm_mul_ps(
						_mm_set_ps1(v1.x),
						_mm_set_ps(m1.m0, m1.m4, m1.m8, m1.m12)),
					_mm_mul_ps(
						_mm_set_ps1(v1.y),
						_mm_set_ps(m1.m1, m1.m5, m1.m9, m1.m13))
				),
				_mm_add_ps(
					_mm_mul_ps(
						_mm_set_ps1(v1.z),
						_mm_set_ps(m1.m2, m1.m6, m1.m10, m1.m14)),
					_mm_mul_ps(
						_mm_set_ps1(v1.w),
						_mm_set_ps(m1.m3, m1.m7, m1.m11, m1.m15))
				)
			)
		);

		tmp = Vect
		(
			_mm_add_ps(
				_mm_add_ps(
					_mm_mul_ps(
						_mm_set_ps1(v1.x),
						_mm_set_ps(m2.m0, m2.m4, m2.m8, m2.m12)),
					_mm_mul_ps(
						_mm_set_ps1(v1.y),
						_mm_set_ps(m2.m1, m2.m5, m2.m9, m2.m13))
				),
				_mm_add_ps(
					_mm_mul_ps(
						_mm_set_ps1(v1.z),
						_mm_set_ps(m2.m2, m2.m6, m2.m10, m2.m14)),
					_mm_mul_ps(
						_mm_set_ps1(v1.w),
						_mm_set_ps(m2.m3, m2.m7, m2.m11, m2.m15))
				)
			)
		);

		tmp = Vect
		(
			_mm_add_ps(
				_mm_add_ps(
					_mm_mul_ps(
						_mm_set_ps1(v1.x),
						_mm_set_ps(m3.m0, m3.m4, m3.m8, m3.m12)),
					_mm_mul_ps(
						_mm_set_ps1(v1.y),
						_mm_set_ps(m3.m1, m3.m5, m3.m9, m3.m13))
				),
				_mm_add_ps(
					_mm_mul_ps(
						_mm_set_ps1(v1.z),
						_mm_set_ps(m3.m2, m3.m6, m3.m10, m3.m14)),
					_mm_mul_ps(
						_mm_set_ps1(v1.w),
						_mm_set_ps(m3.m3, m3.m7, m3.m11, m3.m15))
				)
			)
		);
		return Vect(
			_mm_add_ps(
				_mm_add_ps(
					_mm_mul_ps(
						_mm_set_ps1(v1.x),
						_mm_set_ps(m4.m0, m4.m4, m4.m8, m4.m12)),
					_mm_mul_ps(
						_mm_set_ps1(v1.y),
						_mm_set_ps(m4.m1, m4.m5, m4.m9, m4.m13))
				),
				_mm_add_ps(
					_mm_mul_ps(
						_mm_set_ps1(v1.z),
						_mm_set_ps(m4.m2, m4.m6, m4.m10, m4.m14)),
					_mm_mul_ps(
						_mm_set_ps1(v1.w),
						_mm_set_ps(m4.m3, m4.m7, m4.m11, m4.m15))
				)
			)
		);
	}
};

struct vM5 {
	const Vect v1;
	const Matrix m1, m2, m3, m4, m5;

	vM5(const vM4 _vM, const Matrix& _m1)
		: v1(_vM.v1), m1(_vM.m1), m2(_vM.m2), m3(_vM.m3), m4(_vM.m4), m5(_m1) {

	}

	vM5(const vM5& tmp) = default;
	vM5& operator = (const vM5& tmp) = delete;
	~vM5() = default;

	operator Vect()
	{
		Vect tmp
		(
			_mm_add_ps(
				_mm_add_ps(
					_mm_mul_ps(
						_mm_set_ps1(v1.x),
						m1.v0._m
					),
					_mm_mul_ps(
						_mm_set_ps1(v1.y),
						m1.v1._m
					)
				),
				_mm_add_ps(
					_mm_mul_ps(
						_mm_set_ps1(v1.z),
						m1.v2._m
					),
					_mm_mul_ps(
						_mm_set_ps1(v1.w),
						m1.v3._m
					)
				)
			)
		);

		tmp = Vect
		(

			_mm_add_ps(
				_mm_add_ps(
					_mm_mul_ps(
						_mm_set_ps1(tmp.x),
						m2.v0._m
					),
					_mm_mul_ps(
						_mm_set_ps1(tmp.y),
						m2.v1._m
					)
				),
				_mm_add_ps(
					_mm_mul_ps(
						_mm_set_ps1(tmp.z),
						m2.v2._m
					),
					_mm_mul_ps(
						_mm_set_ps1(tmp.w),
						m2.v3._m
					)
				)
			)
		);

		tmp = Vect
		(
			_mm_add_ps(
				_mm_add_ps(
					_mm_mul_ps(
						_mm_set_ps1(tmp.x),
						m3.v0._m
					),
					_mm_mul_ps(
						_mm_set_ps1(tmp.y),
						m3.v1._m
					)
				),
				_mm_add_ps(
					_mm_mul_ps(
						_mm_set_ps1(tmp.z),
						m3.v2._m
					),
					_mm_mul_ps(
						_mm_set_ps1(tmp.w),
						m3.v3._m
					)
				)
			)
		);

		tmp = Vect
		(
			_mm_add_ps(
				_mm_add_ps(
					_mm_mul_ps(
						_mm_set_ps1(tmp.x),
						m4.v0._m
					),
					_mm_mul_ps(
						_mm_set_ps1(tmp.y),
						m4.v1._m
					)
				),
				_mm_add_ps(
					_mm_mul_ps(
						_mm_set_ps1(tmp.z),
						m4.v2._m
					),
					_mm_mul_ps(
						_mm_set_ps1(tmp.w),
						m4.v3._m
					)
				)
			)
		);

		return Vect(
			_mm_add_ps(
				_mm_add_ps(
					_mm_mul_ps(
						_mm_set_ps1(tmp.x),
						m5.v0._m
					),
					_mm_mul_ps(
						_mm_set_ps1(tmp.y),
						m5.v1._m
					)
				),
				_mm_add_ps(
					_mm_mul_ps(
						_mm_set_ps1(tmp.z),
						m5.v2._m
					),
					_mm_mul_ps(
						_mm_set_ps1(tmp.w),
						m5.v3._m
					)
				)
			)
		);
	}
};

inline vM operator * (const Vect& _v1, const Matrix& _m1)
{
	return vM(_v1, _m1);
}
inline vM2 operator * (const vM& _vM, const Matrix& _m1)
{
	return vM2(_vM, _m1);
}
inline vM3 operator * (const vM2& _vM, const Matrix& _m1)
{
	return vM3(_vM, _m1);
}
inline vM4 operator * (const vM3& _vM, const Matrix& _m1)
{
	return vM4(_vM, _m1);
}

inline vM5 operator * (const vM4& _vM, const Matrix& _m1)
{
	return vM5(_vM, _m1);
}
#pragma endregion

#pragma region
struct MM {
	Matrix m1, m2;

	MM() = default;
	~MM() = default;
	MM(const MM &tmp) = default;
	MM & operator = (const MM &tmp) = default;

	MM(const Matrix &_m1, const Matrix &_m2)
		: m1(_m1), m2(_m2)
	{}

	operator Matrix()
	{
		return Matrix(
			Vect(
				m1.v0.dot(Vect(m2.m0, m2.m4, m2.m8, m2.m12)),
				m1.v0.dot(Vect(m2.m1, m2.m5, m2.m9, m2.m13)),
				m1.v0.dot(Vect(m2.m2, m2.m6, m2.m10, m2.m14)),
				m1.v0.dot(Vect(m2.m3, m2.m7, m2.m11, m2.m15))),
			Vect(
				m1.v1.dot(Vect(m2.m0, m2.m4, m2.m8, m2.m12)),
				m1.v1.dot(Vect(m2.m1, m2.m5, m2.m9, m2.m13)),
				m1.v1.dot(Vect(m2.m2, m2.m6, m2.m10, m2.m14)),
				m1.v1.dot(Vect(m2.m3, m2.m7, m2.m11, m2.m15))),
			Vect(
				m1.v2.dot(Vect(m2.m0, m2.m4, m2.m8, m2.m12)),
				m1.v2.dot(Vect(m2.m1, m2.m5, m2.m9, m2.m13)),
				m1.v2.dot(Vect(m2.m2, m2.m6, m2.m10, m2.m14)),
				m1.v2.dot(Vect(m2.m3, m2.m7, m2.m11, m2.m15))),
			Vect(
				m1.v3.dot(Vect(m2.m0, m2.m4, m2.m8, m2.m12)),
				m1.v3.dot(Vect(m2.m1, m2.m5, m2.m9, m2.m13)),
				m1.v3.dot(Vect(m2.m2, m2.m6, m2.m10, m2.m14)),
				m1.v3.dot(Vect(m2.m3, m2.m7, m2.m11, m2.m15)))
		);
	}
};

struct MMM {
	Matrix m1, m2, m3;

	MMM() = default;
	~MMM() = default;
	MMM(const MMM &tmp) = default;
	MMM & operator = (const MMM &tmp) = default;

	MMM(const MM &_m1, const Matrix &_m2)
		: m1(_m1.m1), m2(_m1.m2), m3 (_m2)
	{}

};
struct MMMM {
	Matrix m1, m2, m3, m4;

	MMMM() = default;
	~MMMM() = default;
	MMMM(const MMMM &tmp) = default;
	MMMM & operator = (const MMMM &tmp) = default;

	MMMM(const MMM &_m1, const Matrix &_m2)
		: m1(_m1.m1), m2(_m1.m2), m3(_m1.m3), m4(_m2)
	{}

};
struct MMMMM {
	Matrix m1, m2, m3, m4, m5;

	MMMMM() = default;
	~MMMMM() = default;
	MMMMM(const MMMMM &tmp) = default;
	MMMMM & operator = (const MMMMM &tmp) = default;

	MMMMM(const MMMM &_m1, const Matrix &_m2)
		: m1(_m1.m1), m2(_m1.m2), m3(_m1.m3), m4(_m1.m4), m5(_m2)
	{}

};
struct MMMMMv {
	Matrix m1, m2, m3, m4, m5;
	Vect v1;

	MMMMMv() = default;
	~MMMMMv() = default;
	MMMMMv(const MMMMMv &tmp) = default;
	MMMMMv & operator = (const MMMMMv &tmp) = default;

	MMMMMv(const MMMMM &_m1, const Vect&_v1)
		: m1(_m1.m1), m2(_m1.m2), m3(_m1.m3), m4(_m1.m4), m5(_m1.m5), v1(_v1)
	{}

	operator Vect()
	{

		Matrix tmp
		(
			_mm_add_ps( _mm_add_ps(_mm_mul_ps(_mm_set_ps1(m1.v0.x), _mm_set_ps(m2.v0.x, m2.v0.y, m2.v0.z, m2.v0.w)), _mm_mul_ps(_mm_set_ps1(m1.v0.y), _mm_set_ps(m2.v1.x,m2.v1.y,m2.v1.z,m2.v1.w))),
						_mm_add_ps(_mm_mul_ps(_mm_set_ps1(m1.v0.z), _mm_set_ps(m2.v2.x, m2.v2.y, m2.v2.z, m2.v2.w)), _mm_mul_ps(_mm_set_ps1(m1.v0.w), _mm_set_ps(m2.v3.x, m2.v3.y, m2.v3.z, m2.v3.w)))),
			_mm_add_ps( _mm_add_ps(_mm_mul_ps(_mm_set_ps1(m1.v1.x), _mm_set_ps(m2.v0.x, m2.v0.y, m2.v0.z, m2.v0.w)), _mm_mul_ps(_mm_set_ps1(m1.v1.y), _mm_set_ps(m2.v1.x,m2.v1.y,m2.v1.z,m2.v1.w))),
						_mm_add_ps(_mm_mul_ps(_mm_set_ps1(m1.v1.z), _mm_set_ps(m2.v2.x, m2.v2.y, m2.v2.z, m2.v2.w)), _mm_mul_ps(_mm_set_ps1(m1.v1.w), _mm_set_ps(m2.v3.x, m2.v3.y, m2.v3.z, m2.v3.w)))),
			_mm_add_ps( _mm_add_ps(_mm_mul_ps(_mm_set_ps1(m1.v2.x), _mm_set_ps(m2.v0.x, m2.v0.y, m2.v0.z, m2.v0.w)), _mm_mul_ps(_mm_set_ps1(m1.v2.y), _mm_set_ps(m2.v1.x,m2.v1.y,m2.v1.z,m2.v1.w))),
						_mm_add_ps(_mm_mul_ps(_mm_set_ps1(m1.v2.z), _mm_set_ps(m2.v2.x, m2.v2.y, m2.v2.z, m2.v2.w)), _mm_mul_ps(_mm_set_ps1(m1.v2.w), _mm_set_ps(m2.v3.x, m2.v3.y, m2.v3.z, m2.v3.w)))),
			_mm_add_ps( _mm_add_ps(_mm_mul_ps(_mm_set_ps1(m1.v3.x), _mm_set_ps(m2.v0.x, m2.v0.y, m2.v0.z, m2.v0.w)), _mm_mul_ps(_mm_set_ps1(m1.v3.y), _mm_set_ps(m2.v1.x,m2.v1.y,m2.v1.z,m2.v1.w))),
						_mm_add_ps(_mm_mul_ps(_mm_set_ps1(m1.v3.z), _mm_set_ps(m2.v2.x, m2.v2.y, m2.v2.z, m2.v2.w)), _mm_mul_ps(_mm_set_ps1(m1.v3.w), _mm_set_ps(m2.v3.x,m2.v3.y,m2.v3.z,m2.v3.w))))
		);

		tmp = Matrix
		(
			_mm_add_ps( _mm_add_ps(_mm_mul_ps(_mm_set_ps1(tmp.v0.x), _mm_set_ps(m3.v0.x, m3.v0.y, m3.v0.z, m3.v0.w)), _mm_mul_ps(_mm_set_ps1(tmp.v0.y), _mm_set_ps(m3.v1.x, m3.v1.y, m3.v1.z, m3.v1.w))),
						_mm_add_ps(_mm_mul_ps(_mm_set_ps1(tmp.v0.z), _mm_set_ps(m3.v2.x, m3.v2.y, m3.v2.z, m3.v2.w)), _mm_mul_ps(_mm_set_ps1(tmp.v0.w), _mm_set_ps(m3.v3.x, m3.v3.y, m3.v3.z, m3.v3.w)))),
			_mm_add_ps( _mm_add_ps(_mm_mul_ps(_mm_set_ps1(tmp.v1.x), _mm_set_ps(m3.v0.x, m3.v0.y, m3.v0.z, m3.v0.w)), _mm_mul_ps(_mm_set_ps1(tmp.v1.y), _mm_set_ps(m3.v1.x, m3.v1.y, m3.v1.z, m3.v1.w))),
						_mm_add_ps(_mm_mul_ps(_mm_set_ps1(tmp.v1.z), _mm_set_ps(m3.v2.x, m3.v2.y, m3.v2.z, m3.v2.w)), _mm_mul_ps(_mm_set_ps1(tmp.v1.w), _mm_set_ps(m3.v3.x, m3.v3.y, m3.v3.z, m3.v3.w)))),
			_mm_add_ps( _mm_add_ps(_mm_mul_ps(_mm_set_ps1(tmp.v2.x), _mm_set_ps(m3.v0.x, m3.v0.y, m3.v0.z, m3.v0.w)), _mm_mul_ps(_mm_set_ps1(tmp.v2.y), _mm_set_ps(m3.v1.x, m3.v1.y, m3.v1.z, m3.v1.w))),
						_mm_add_ps(_mm_mul_ps(_mm_set_ps1(tmp.v2.z), _mm_set_ps(m3.v2.x, m3.v2.y, m3.v2.z, m3.v2.w)), _mm_mul_ps(_mm_set_ps1(tmp.v2.w), _mm_set_ps(m3.v3.x, m3.v3.y, m3.v3.z, m3.v3.w)))),
			_mm_add_ps( _mm_add_ps(_mm_mul_ps(_mm_set_ps1(tmp.v3.x), _mm_set_ps(m3.v0.x, m3.v0.y, m3.v0.z, m3.v0.w)), _mm_mul_ps(_mm_set_ps1(tmp.v3.y), _mm_set_ps(m3.v1.x, m3.v1.y, m3.v1.z, m3.v1.w))),
						_mm_add_ps(_mm_mul_ps(_mm_set_ps1(tmp.v3.z), _mm_set_ps(m3.v2.x, m3.v2.y, m3.v2.z, m3.v2.w)), _mm_mul_ps(_mm_set_ps1(tmp.v3.w), _mm_set_ps(m3.v3.x, m3.v3.y, m3.v3.z, m3.v3.w))))
		);

		tmp = Matrix
		(
			_mm_add_ps( _mm_add_ps(_mm_mul_ps(_mm_set_ps1(tmp.v0.x), _mm_set_ps(m4.v0.x, m4.v0.y, m4.v0.z, m4.v0.w)), _mm_mul_ps(_mm_set_ps1(tmp.v0.y), _mm_set_ps(m4.v1.x, m4.v1.y, m4.v1.z, m4.v1.w))),
						_mm_add_ps(_mm_mul_ps(_mm_set_ps1(tmp.v0.z), _mm_set_ps(m4.v2.x, m4.v2.y, m4.v2.z, m4.v2.w)), _mm_mul_ps(_mm_set_ps1(tmp.v0.w), _mm_set_ps(m4.v3.x, m4.v3.y, m4.v3.z, m4.v3.w)))),
			_mm_add_ps( _mm_add_ps(_mm_mul_ps(_mm_set_ps1(tmp.v1.x), _mm_set_ps(m4.v0.x, m4.v0.y, m4.v0.z, m4.v0.w)), _mm_mul_ps(_mm_set_ps1(tmp.v1.y), _mm_set_ps(m4.v1.x, m4.v1.y, m4.v1.z, m4.v1.w))),
						_mm_add_ps(_mm_mul_ps(_mm_set_ps1(tmp.v1.z), _mm_set_ps(m4.v2.x, m4.v2.y, m4.v2.z, m4.v2.w)), _mm_mul_ps(_mm_set_ps1(tmp.v1.w), _mm_set_ps(m4.v3.x, m4.v3.y, m4.v3.z, m4.v3.w)))),
			_mm_add_ps( _mm_add_ps(_mm_mul_ps(_mm_set_ps1(tmp.v2.x), _mm_set_ps(m4.v0.x, m4.v0.y, m4.v0.z, m4.v0.w)), _mm_mul_ps(_mm_set_ps1(tmp.v2.y), _mm_set_ps(m4.v1.x, m4.v1.y, m4.v1.z, m4.v1.w))),
						_mm_add_ps(_mm_mul_ps(_mm_set_ps1(tmp.v2.z), _mm_set_ps(m4.v2.x, m4.v2.y, m4.v2.z, m4.v2.w)), _mm_mul_ps(_mm_set_ps1(tmp.v2.w), _mm_set_ps(m4.v3.x, m4.v3.y, m4.v3.z, m4.v3.w)))),
			_mm_add_ps( _mm_add_ps(_mm_mul_ps(_mm_set_ps1(tmp.v3.x), _mm_set_ps(m4.v0.x, m4.v0.y, m4.v0.z, m4.v0.w)), _mm_mul_ps(_mm_set_ps1(tmp.v3.y), _mm_set_ps(m4.v1.x, m4.v1.y, m4.v1.z, m4.v1.w))),
						_mm_add_ps(_mm_mul_ps(_mm_set_ps1(tmp.v3.z), _mm_set_ps(m4.v2.x, m4.v2.y, m4.v2.z, m4.v2.w)), _mm_mul_ps(_mm_set_ps1(tmp.v3.w), _mm_set_ps(m4.v3.x, m4.v3.y, m4.v3.z, m4.v3.w))))
		);

		tmp = Matrix
		(
			_mm_add_ps( _mm_add_ps(_mm_mul_ps(_mm_set_ps1(tmp.v0.x), _mm_set_ps(m5.v0.x, m5.v0.y, m5.v0.z,m5.v0.w)), _mm_mul_ps(_mm_set_ps1(tmp.v0.y), _mm_set_ps(m5.v1.x, m5.v1.y, m5.v1.z, m5.v1.w))),
						_mm_add_ps(_mm_mul_ps(_mm_set_ps1(tmp.v0.z), _mm_set_ps(m5.v2.x, m5.v2.y, m5.v2.z,m5.v2.w)), _mm_mul_ps(_mm_set_ps1(tmp.v0.w), _mm_set_ps(m5.v3.x, m5.v3.y, m5.v3.z, m5.v3.w)))),
			_mm_add_ps( _mm_add_ps(_mm_mul_ps(_mm_set_ps1(tmp.v1.x), _mm_set_ps(m5.v0.x, m5.v0.y, m5.v0.z,m5.v0.w)), _mm_mul_ps(_mm_set_ps1(tmp.v1.y), _mm_set_ps(m5.v1.x, m5.v1.y, m5.v1.z, m5.v1.w))),
						_mm_add_ps(_mm_mul_ps(_mm_set_ps1(tmp.v1.z), _mm_set_ps(m5.v2.x, m5.v2.y, m5.v2.z,m5.v2.w)), _mm_mul_ps(_mm_set_ps1(tmp.v1.w), _mm_set_ps(m5.v3.x, m5.v3.y, m5.v3.z, m5.v3.w)))),
			_mm_add_ps( _mm_add_ps(_mm_mul_ps(_mm_set_ps1(tmp.v2.x), _mm_set_ps(m5.v0.x, m5.v0.y, m5.v0.z,m5.v0.w)), _mm_mul_ps(_mm_set_ps1(tmp.v2.y), _mm_set_ps(m5.v1.x, m5.v1.y, m5.v1.z, m5.v1.w))),
						_mm_add_ps(_mm_mul_ps(_mm_set_ps1(tmp.v2.z), _mm_set_ps(m5.v2.x, m5.v2.y, m5.v2.z,m5.v2.w)), _mm_mul_ps(_mm_set_ps1(tmp.v2.w), _mm_set_ps(m5.v3.x, m5.v3.y, m5.v3.z, m5.v3.w)))),
			_mm_add_ps( _mm_add_ps(_mm_mul_ps(_mm_set_ps1(tmp.v3.x), _mm_set_ps(m5.v0.x, m5.v0.y, m5.v0.z,m5.v0.w)), _mm_mul_ps(_mm_set_ps1(tmp.v3.y), _mm_set_ps(m5.v1.x, m5.v1.y, m5.v1.z, m5.v1.w))),
						_mm_add_ps(_mm_mul_ps(_mm_set_ps1(tmp.v3.z), _mm_set_ps(m5.v2.x, m5.v2.y, m5.v2.z,m5.v2.w)), _mm_mul_ps(_mm_set_ps1(tmp.v3.w), _mm_set_ps(m5.v3.x, m5.v3.y, m5.v3.z, m5.v3.w))))
		);

		return  Vect
		(
			_mm_add_ps(
				_mm_add_ps(
					_mm_mul_ps(
						_mm_set_ps1(v1.x),
						_mm_set_ps(tmp.v0.x,tmp.v0.y,tmp.v0.z,tmp.v0.w)
					),
					_mm_mul_ps(
						_mm_set_ps1(v1.y),
						_mm_set_ps(tmp.v1.x, tmp.v1.y, tmp.v1.z, tmp.v1.w)
					)
				),
				_mm_add_ps(
					_mm_mul_ps(
						_mm_set_ps1(v1.z),
						_mm_set_ps(tmp.v2.x, tmp.v2.y, tmp.v2.z, tmp.v2.w)
					),
					_mm_mul_ps(
						_mm_set_ps1(v1.w),
						_mm_set_ps(tmp.v3.x, tmp.v3.y, tmp.v3.z, tmp.v3.w)
					)
				)
			)
		);

	}
};

struct Mv {
	Matrix m1;
	Vect v1;

	Mv() = default;
	~Mv() = default;
	Mv(const Mv &tmp) = default;
	Mv & operator = (const Mv &tmp) = default;

	Mv(const Matrix &_m1, const Vect &_v1)
		: m1(_m1), v1(_v1)
	{}

	operator Vect()
	{
		return  Vect
		(
			_mm_add_ps(
				_mm_add_ps(
					_mm_mul_ps(
						_mm_set_ps1(v1.x),
						_mm_set_ps(m1.v0.x, m1.v0.y, m1.v0.z, m1.v0.w)
					),
					_mm_mul_ps(
						_mm_set_ps1(v1.y),
						_mm_set_ps(m1.v1.x, m1.v1.y, m1.v1.z, m1.v1.w)
					)
				),
				_mm_add_ps(
					_mm_mul_ps(
						_mm_set_ps1(v1.z),
						_mm_set_ps(m1.v2.x, m1.v2.y, m1.v2.z, m1.v2.w)
					),
					_mm_mul_ps(
						_mm_set_ps1(v1.w),
						_mm_set_ps(m1.v3.x, m1.v3.y, m1.v3.z, m1.v3.w)
					)
				)
			)
		);
	}
};

inline Mv operator * (const Matrix &_m1, const Vect &_v1)
{
	return Mv(_m1, _v1);
}
inline MM operator * (const Matrix &_m1, const Matrix &_m2)
{
	return MM(_m1, _m2);
}
inline MMM operator * (const MM &_mm, const Matrix &_m2)
{
	return MMM(_mm, _m2);
}
inline MMMM operator * (const MMM &_mm, const Matrix &_m2)
{
	return MMMM(_mm, _m2);
}
inline MMMMM operator * (const MMMM &_mm, const Matrix &_m2)
{
	return MMMMM(_mm, _m2);
}
inline MMMMMv operator * (const MMMMM &_mm, const Vect &_v1)
{
	return MMMMMv(_mm, _v1);
}
*/
#pragma endregion
//*/

// ---  End of File ---------------

#pragma warning(default : 4201)
