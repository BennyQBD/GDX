#ifndef TRANSFORM_H_INCLUDED
#define TRANSFORM_H_INCLUDED

#include "math3d.h"

class Transform
{
public:
	Vector3f			Pos;
	Quaternion			Rotation;
	Vector3f			Scale;

	static Matrix4f	View;
	static Matrix4f	Projection;

	Transform(const Vector3f& pos = Vector3f(0,0,0), const Quaternion& rotation = Quaternion(0,0,0,1), const Vector3f& scale = Vector3f(1,1,1));

	Matrix4f& GetModel(bool calcModel = true);
	Matrix4f& GetMVP(bool calcMVP = true, bool calcModel = true);

	void SetChildModel(const Matrix4f& childModel);

	static void CalcViewProjection();
protected:
private:
	static Matrix4f ViewProjection;
	void CalcModel();

	Matrix4f			m_Model;
	Matrix4f			m_MVP;
	Matrix4f			m_ChildModel;
};

#endif // TRANSFORM_H_INCLUDED
