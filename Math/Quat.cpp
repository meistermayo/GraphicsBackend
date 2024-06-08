#include "Quat.h"
#include "Matrix.h"

const Quat Quat::Identity(Vect::Zero);

Quat operator * (float scale, Quat& inQ)
{
	return inQ * scale;
}

Quat::Quat(const Matrix& tmp)
{
	float trace = tmp.m00 + tmp.m11 + tmp.m22;

	if (trace > 0)
	{
		float s = 0.5f / sqrtf(trace + 1.0f);
		x = (tmp.m21 - tmp.m12) * s;
		y = (tmp.m02 - tmp.m20) * s;
		z = (tmp.m10 - tmp.m01) * s;
		w = 0.25f / s;
	}
	else
	{
		// if m00 is the biggest
		if (tmp.m00 > tmp.m11 && tmp.m00 > tmp.m22)
		{
			float s = 2.0f * sqrtf(1.0f + tmp.m00 - tmp.m11 - tmp.m22);
			float invS = 1.0f / s;
			x = 0.25f * s;
			y = (tmp.m01 + tmp.m10) * invS;
			z = (tmp.m02 + tmp.m20) * invS;
			w = (tmp.m21 + tmp.m12) * invS;
		}
		else if (tmp.m11 > tmp.m22) // if m11 is the biggest
		{
			float s = 2.0f * sqrtf(1.0f + tmp.m11 - tmp.m00 - tmp.m22);
			float invS = 1.0f / s;
			x = (tmp.m01 + tmp.m10) * invS;
			y = 0.25f * s;
			z = (tmp.m12 + tmp.m21) * invS;
			w = (tmp.m02 + tmp.m20) * invS;
		}
		else // m22 must be the biggest
		{
			float s = 2.0f * sqrtf(1.0f + tmp.m22 - tmp.m00 - tmp.m11);
			float invS = 1.0f / s;
			x = (tmp.m02 + tmp.m20) * invS;
			y = (tmp.m12 + tmp.m21) * invS;
			z = 0.25f * s;
			w = (tmp.m10 + tmp.m01) * invS;
		}
	}
}
