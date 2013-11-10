#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

#include "math3d.h"

class Camera
{
public:
	Camera(const Vector3f& pos = Vector3f(0,0,0), const Vector3f& forward = Vector3f(0,0,1), const Vector3f& up = Vector3f(0,1,0));

	void Move(const Vector3f& amt);
	void RotateX(float angle);
	void RotateY(float angle);
	void RotateZ(float angle);

	void Roll(float angle);
	void Pitch(float angle);
	void Yaw(float angle);

	Matrix4f ToMatrix();

	Vector3f GetForward();
	Vector3f GetBack();
	Vector3f GetRight();
	Vector3f GetLeft();
	Vector3f GetUp();
	Vector3f GetDown();
protected:
private:
	Vector3f m_Pos;
	Vector3f m_Forward;
	Vector3f m_Up;
};

#endif // CAMERA_H_INCLUDED
