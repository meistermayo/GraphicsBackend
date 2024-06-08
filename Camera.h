#ifndef CAMERA_H
#define CAMERA_H

#include "Math/Vect.h"
#include "Math/Matrix.h"
#include "Align16.h"

class Camera : public Align16
{
	bool bIsPerspective = false;
public:

	// Default constructor
	Camera() = default;
	~Camera() = default;

	// Setup on single camera
	void setPerspective(float FieldOfView_Degs, float AspectRatio, float NearDist, float FarDist);
	void setViewport(int inX, int inY, int inWidth, int inHeight);
	void setOrthographic(float left, float right, float top, float bottom, float near, float far);
	void setOrientAndPosition(const Vect &Up_vect, const Vect &inLookAt_pt, const Vect &pos_pt);

	// update camera system
	void updateCamera(void);

	// Get the matrices for rendering
	Matrix& getViewMatrix();
	Matrix& getProjMatrix();

	// accessors
	void getPos(Vect &outPos) const;
	void getDir(Vect &outDir) const;
	void getUp(Vect &outUp) const;
	void getLookAt(Vect &outLookAt) const;
	void getRight(Vect &outRight) const;

	void TranslateLeftRight(float delta);
	void TranslateFwdBack(float delta);
	void TiltUpDown(float ang);
	void TurnLeftRight(float ang);

	// Why no SETS for Pos, Dir, Up, LookAt and Right?
	//   They have to be adjusted _together_ in setOrientAndPosition()

private:  // methods should never be public
	void privUpdateProjectionMatrix(void);
	void privUpdateViewMatrix(void);


private:  // data  (Keep it private)
	float x, y, width, height;

		  // Projection Matrix
	Matrix	projMatrix;
	Matrix	viewMatrix;

	// camera unit vectors (up, dir, right)
	Vect	vUp;
	Vect	vDir;
	Vect	vRight;  // derived by up and dir
	Vect	vPos;
	Vect	vLookAt;

	// Define the frustum inputs
	float	nearDist;
	float	farDist;
	float	fovy;  // aka view angle along y axis
	float	aspectRatio;
};

#endif