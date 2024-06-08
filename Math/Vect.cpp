#include "./Vect.h"
#include "./Matrix.h"

const Vect Vect::Right		(1.f, 0.f, 0.f, 1.f);
const Vect Vect::Up			(0.f, 1.f, 0.f, 1.f);
const Vect Vect::Forward	(0.f, 0.f, 1.f, 1.f);
const Vect Vect::Zero		(0.f, 0.f, 0.f, 1.f);
const Vect Vect::One		(1.f, 1.f, 1.f, 1.f);

const Vect Vect::Right_0	(1.f, 0.f, 0.f, 0.f);
const Vect Vect::Up_0		(0.f, 1.f, 0.f, 0.f);
const Vect Vect::Forward_0	(0.f, 0.f, 1.f, 0.f);
const Vect Vect::Zero_0		(0.f, 0.f, 0.f, 0.f);
const Vect Vect::One_0		(1.f, 1.f, 1.f, 0.f);

/*
Vect Vect::operator * (const Vect& v)
{
	return Vect(v.dot(v0), v.dot(v1), v.dot(v2), v.dot(v3));
}
*/
Vect Vect::operator*(const Matrix& tmp) const
{
	return Vect
	(
		_mm_add_ps(
			_mm_add_ps(
				_mm_mul_ps(
					_mm_set_ps1(x),
					tmp.v0._m
				),
				_mm_mul_ps(
					_mm_set_ps1(y),
					tmp.v1._m
				)
			),
			_mm_add_ps(
				_mm_mul_ps(
					_mm_set_ps1(z),
					tmp.v2._m
				),
				_mm_mul_ps(
					_mm_set_ps1(w),
					tmp.v3._m
				)
			)
		)
	);
}

Vect Vect::operator*=(const Matrix& tmp) const
{
	return Vect
	(
		_mm_add_ps(
			_mm_add_ps(
				_mm_mul_ps(
					_mm_set_ps1(x),
					tmp.v0._m
				),
				_mm_mul_ps(
					_mm_set_ps1(y),
					tmp.v1._m
				)
			),
			_mm_add_ps(
				_mm_mul_ps(
					_mm_set_ps1(z),
					tmp.v2._m
				),
				_mm_mul_ps(
					_mm_set_ps1(w),
					tmp.v3._m
				)
			)
		)
	);
}

Vect operator*(const float scale, const Vect& inV)
{
	return inV * scale;
}
