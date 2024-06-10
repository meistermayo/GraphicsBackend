#include "Quat.h"
#include "Matrix.h"

const Quat Quat::Identity(Vect::Zero);

Quat operator * (float scale, Quat& inQuat)
{
	return inQuat * scale;
}

Quat::Quat(const Matrix& inMatrix)
{
	float trace = inMatrix.m00 + inMatrix.m11 + inMatrix.m22;

	if (trace > 0)
	{
		float s = 0.5f / sqrtf(trace + 1.0f);
		x = (inMatrix.m21 - inMatrix.m12) * s;
		y = (inMatrix.m02 - inMatrix.m20) * s;
		z = (inMatrix.m10 - inMatrix.m01) * s;
		w = 0.25f / s;
	}
	else
	{
		// if m00 is the biggest
		if (inMatrix.m00 > inMatrix.m11 && inMatrix.m00 > inMatrix.m22)
		{
			float s = 2.0f * sqrtf(1.0f + inMatrix.m00 - inMatrix.m11 - inMatrix.m22);
			float invS = 1.0f / s;
			x = 0.25f * s;
			y = (inMatrix.m01 + inMatrix.m10) * invS;
			z = (inMatrix.m02 + inMatrix.m20) * invS;
			w = (inMatrix.m21 + inMatrix.m12) * invS;
		}
		else if (inMatrix.m11 > inMatrix.m22) // if m11 is the biggest
		{
			float s = 2.0f * sqrtf(1.0f + inMatrix.m11 - inMatrix.m00 - inMatrix.m22);
			float invS = 1.0f / s;
			x = (inMatrix.m01 + inMatrix.m10) * invS;
			y = 0.25f * s;
			z = (inMatrix.m12 + inMatrix.m21) * invS;
			w = (inMatrix.m02 + inMatrix.m20) * invS;
		}
		else // m22 must be the biggest
		{
			float s = 2.0f * sqrtf(1.0f + inMatrix.m22 - inMatrix.m00 - inMatrix.m11);
			float invS = 1.0f / s;
			x = (inMatrix.m02 + inMatrix.m20) * invS;
			y = (inMatrix.m12 + inMatrix.m21) * invS;
			z = 0.25f * s;
			w = (inMatrix.m10 + inMatrix.m01) * invS;
		}
	}
}
