#include "Camera.h"
#include <assert.h>
#include <math.h>

void Camera::setPerspective(float Fovy, float Aspect, float NearDist, float FarDist)
{
	bIsPerspective = true;
	this->aspectRatio = Aspect;
	this->fovy = Fovy;
	this->nearDist = NearDist;
	this->farDist = FarDist;
}

void Camera::setViewport(int inX, int inY, int inWidth, int inHeight)
{
	x = static_cast<float>(inX);
	y = static_cast<float>(inY);

	width = static_cast<float>(inWidth);
	height = static_cast<float>(inHeight);

	this->aspectRatio = height / width;
}

void Camera::setOrthographic(float left, float right, float top, float bottom, float near, float far)
{
	bIsPerspective = false;
	setPerspective(this->fovy, (right-left)/(bottom-top), near, far); // todo : check me
}

void Camera::setOrientAndPosition(const Vect &inUp, const Vect &inLookAt, const Vect &inPos)
{
	Vect a(1, 1, 1, 1);
	Vect b(1, 1, 1, 1);
	Vect c(1, 1, 1, 0.5f);
	Vect d(1, 1, 1, 0.5f);

	Vect ab = a - b;
	Vect bc = b - c;
	Vect cd = c - d;
	Vect da = d - a;
	// Remember the up, dir and right are unit length, and are perpendicular.
	// Treat lookAt as king, find Right vect, then correct Up to insure perpendiculare.
	// Make sure that all vectors are unit vectors.

	this->vLookAt = inLookAt;
	this->vDir = -(inLookAt - inPos); // Right-Hand camera: vDir is flipped
	this->vDir.Normalize();

	// Clean up the vectors (Right hand rule)
	this->vRight = inUp.cross(this->vDir);
	this->vRight.Normalize();

	this->vUp = this->vDir.cross(this->vRight);
	this->vUp.Normalize();

	this->vPos = inPos;
};


// The projection matrix 
void Camera::privUpdateProjectionMatrix(void)
{
	/*/
	this->projMatrix[0] = 2.0f * nearDist / width;
	this->projMatrix[1] = 0.0f;
	this->projMatrix[2] = 0.0f;
	this->projMatrix[3] = 0.0f;

	this->projMatrix[4] = 0.0f;
	this->projMatrix[5] = 2.0f * nearDist / (height);
	this->projMatrix[6] = 0.0f;
	this->projMatrix[7] = 0.0f;

	this->projMatrix[8] = 0.0f;
	this->projMatrix[9] = 0.0f;
	this->projMatrix[10] = -(this->farDist + this->nearDist) / (this->farDist - this->nearDist);
	this->projMatrix[11] = -1.0f;

	this->projMatrix[12] = 0.0f;
	this->projMatrix[13] = 0.0f;
	this->projMatrix[14] = (-2.0f * this->farDist * this->nearDist) / (this->farDist - this->nearDist);
	this->projMatrix[15] = 0.0f;
	//*/
	if (bIsPerspective)
	{
		float d = 1.0f / tanf(fovy / 2.0f);
		this->projMatrix[0] = d / aspectRatio;
		this->projMatrix[1] = 0.0f;
		this->projMatrix[2] = 0.0f;
		this->projMatrix[3] = 0.0f;

		this->projMatrix[4] = 0.0f;
		this->projMatrix[5] = d;
		this->projMatrix[6] = 0.0f;
		this->projMatrix[7] = 0.0f;

		this->projMatrix[8] = 0.0f;
		this->projMatrix[9] = 0.0f;
		this->projMatrix[10] = -(this->farDist + this->nearDist) / (this->farDist - this->nearDist);
		this->projMatrix[11] = -1.0f;

		this->projMatrix[12] = 0.0f;
		this->projMatrix[13] = 0.0f;
		this->projMatrix[14] = (-2.0f * this->farDist * this->nearDist) / (this->farDist - this->nearDist);
		this->projMatrix[15] = 0.0f;
	}
	else
	{
		this->projMatrix[0] = 2.0f / width;
		this->projMatrix[1] = 0.0f;
		this->projMatrix[2] = 0.0f;
		this->projMatrix[3] = 0.0f;

		this->projMatrix[4] = 0.0f;
		this->projMatrix[5] = 2.0f / height;
		this->projMatrix[6] = 0.0f;
		this->projMatrix[7] = 0.0f;

		this->projMatrix[8] = 0.0f;
		this->projMatrix[9] = 0.0f;
		this->projMatrix[10] = -2.0f / (this->farDist - this->nearDist);
		this->projMatrix[11] = -1.0f;

		this->projMatrix[12] = -1.0f;
		this->projMatrix[13] = 1.0f;
		this->projMatrix[14] = (-1.0f * this->farDist * this->nearDist) / (this->farDist - this->nearDist);
		this->projMatrix[15] = 1.0f;
	}
	//*/

	// Converting from OpenGL-style NDC of [-1,1] to DX's [0,1].  See note: https://anteru.net/blog/2011/12/27/1830/
	// (Note: NDCConvert should be precomputed once and stored for reuse)
	Matrix B = Matrix::Trans(Vect(0, 0, 1.0f));
	Matrix S = Matrix::Scale(Vect(1, 1, 0.5f));
	Matrix NDCConvert = B * S;

	projMatrix = projMatrix * NDCConvert;
};


void Camera::privUpdateViewMatrix(void)
{
	// This functions assumes the your vUp, vRight, vDir are still unit
	// And perpendicular to each other

	// Set for DX Right-handed space
	this->viewMatrix[0] = this->vRight.x;
	this->viewMatrix[1] = this->vUp.x;
	this->viewMatrix[2] = this->vDir.x;
	this->viewMatrix[3] = 0.0f;

	this->viewMatrix[4] = this->vRight.y;
	this->viewMatrix[5] = this->vUp.y;
	this->viewMatrix[6] = this->vDir.y;
	this->viewMatrix[7] = 0.0f;

	this->viewMatrix[8] = this->vRight.z;
	this->viewMatrix[9] = this->vUp.z;
	this->viewMatrix[10] = this->vDir.z;
	this->viewMatrix[11] = 0.0f;

	// Apply R^t( -Pos ). Use dot-product with the basis vectors
	this->viewMatrix[12] = -vPos.dot(vRight) + 1.0f; // todo : check
	this->viewMatrix[13] = -vPos.dot(vUp) + 1.0f;
	this->viewMatrix[14] = -vPos.dot(vDir) + 1.0f;
	this->viewMatrix[15] = 1.0f;
};


// Update everything (make sure it's consistent)
void Camera::updateCamera(void)
{
	// update the projection matrix
	this->privUpdateProjectionMatrix();

	// update the view matrix
	this->privUpdateViewMatrix();
}

Matrix & Camera::getViewMatrix(void)
{
	return this->viewMatrix;
}

Matrix & Camera::getProjMatrix(void)
{
	return this->projMatrix;
}

void Camera::getPos(Vect &outPos) const
{
	outPos = this->vPos;
}

void  Camera::getDir(Vect &outDir) const
{
	outDir = this->vDir;
}

void  Camera::getUp(Vect &outUp) const
{
	outUp = this->vUp;
}

void Camera::getLookAt(Vect &outLookAt) const
{
	outLookAt = this->vLookAt;
}

void Camera::getRight(Vect &outRight) const
{
	outRight = this->vRight;
}

void Camera::TranslateLeftRight(float delta)
{
	vPos += vRight * delta;
}

void Camera::TranslateFwdBack(float delta)
{
	vPos -= vDir * delta;
}

void Camera::TiltUpDown(float ang)
{
	vDir = vDir * Matrix::RotAxisAngle(vRight, ang);
	setOrientAndPosition(Vect(0, 1, 0), vPos - vDir, vPos);
}

void Camera::TurnLeftRight(float ang)
{
	vDir = vDir * Matrix::RotY(ang);
	setOrientAndPosition(Vect(0, 1, 0), vPos - vDir, vPos);
}

