#include "math3d.h"

//#include "quaternion.h"
//#include "util.h"

const Vector3f Vector3f::UP(0,1,0);
const Vector3f Vector3f::DOWN(0,-1,0);
const Vector3f Vector3f::LEFT(-1,0,0);
const Vector3f Vector3f::RIGHT(1,0,0);
const Vector3f Vector3f::FORWARD(0,0,1);
const Vector3f Vector3f::BACK(0,0,-1);
const Vector3f Vector3f::ONE(1,1,1);
const Vector3f Vector3f::ZERO(0,0,0);

Vector3f::Vector3f(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

float Vector3f::Length() const
{
    return sqrtf(x * x + y * y + z * z);
}

float Vector3f::Dot(const Vector3f& v) const
{
    return x * v.x + y * v.y + z * v.z;
}

Vector3f Vector3f::Cross(const Vector3f& v) const
{
    const float _x = y * v.z - z * v.y;
    const float _y = z * v.x - x * v.z;
    const float _z = x * v.y - y * v.x;

    return Vector3f(_x, _y, _z);
}

Vector3f Vector3f::Normalized() const
{
    return *this/Length();
}

Vector3f Vector3f::Rotate(const Vector3f& axis, float angle) const
{
    float sinAngle = sinf(angle);
	float cosAngle = cosf(angle);

	return this->Cross(axis * sinAngle) +           //Rotation on local X
		   (*this * cosAngle) +                     //Rotation on local Z
		   axis * this->Dot(axis * (1 - cosAngle)); //Rotation on local Y
}

Vector3f Vector3f::Rotate(const Quaternion& rotation) const
{
    Quaternion rotatedVector = rotation * *this * rotation.Conjugate();
    
    return Vector3f(rotatedVector.x, rotatedVector.y, rotatedVector.z);
}

///////////////////////////////////////////////////////////////////////////////
//MATRIX SECTION
///////////////////////////////////////////////////////////////////////////////
const Matrix4f Matrix4f::IDENTITY =  Matrix4f();

Matrix4f Matrix4f::InitTranslation(const Vector3f& translation)
{
    Matrix4f r;
    
	r.m[0][0] = 1.0f; r.m[0][1] = 0.0f; r.m[0][2] = 0.0f; r.m[0][3] = translation.x;
    r.m[1][0] = 0.0f; r.m[1][1] = 1.0f; r.m[1][2] = 0.0f; r.m[1][3] = translation.y;
    r.m[2][0] = 0.0f; r.m[2][1] = 0.0f; r.m[2][2] = 1.0f; r.m[2][3] = translation.z;
    r.m[3][0] = 0.0f; r.m[3][1] = 0.0f; r.m[3][2] = 0.0f; r.m[3][3] = 1.0f;
    
    return r;
}

Matrix4f Matrix4f::InitRotation(const Vector3f& eulerAngles)
{
	Matrix4f rx, ry, rz;

    const float x = ToRadians(eulerAngles.x);
    const float y = ToRadians(eulerAngles.y);
    const float z = ToRadians(eulerAngles.z);

    rx.m[0][0] = 1.0f; rx.m[0][1] = 0.0f   ; rx.m[0][2] = 0.0f    ; rx.m[0][3] = 0.0f;
    rx.m[1][0] = 0.0f; rx.m[1][1] = cosf(x); rx.m[1][2] = -sinf(x); rx.m[1][3] = 0.0f;
    rx.m[2][0] = 0.0f; rx.m[2][1] = sinf(x); rx.m[2][2] = cosf(x) ; rx.m[2][3] = 0.0f;
    rx.m[3][0] = 0.0f; rx.m[3][1] = 0.0f   ; rx.m[3][2] = 0.0f    ; rx.m[3][3] = 1.0f;

    ry.m[0][0] = cosf(y); ry.m[0][1] = 0.0f; ry.m[0][2] = -sinf(y); ry.m[0][3] = 0.0f;
    ry.m[1][0] = 0.0f   ; ry.m[1][1] = 1.0f; ry.m[1][2] = 0.0f    ; ry.m[1][3] = 0.0f;
    ry.m[2][0] = sinf(y); ry.m[2][1] = 0.0f; ry.m[2][2] = cosf(y) ; ry.m[2][3] = 0.0f;
    ry.m[3][0] = 0.0f   ; ry.m[3][1] = 0.0f; ry.m[3][2] = 0.0f    ; ry.m[3][3] = 1.0f;

    rz.m[0][0] = cosf(z); rz.m[0][1] = -sinf(z); rz.m[0][2] = 0.0f; rz.m[0][3] = 0.0f;
    rz.m[1][0] = sinf(z); rz.m[1][1] = cosf(z) ; rz.m[1][2] = 0.0f; rz.m[1][3] = 0.0f;
    rz.m[2][0] = 0.0f   ; rz.m[2][1] = 0.0f    ; rz.m[2][2] = 1.0f; rz.m[2][3] = 0.0f;
    rz.m[3][0] = 0.0f   ; rz.m[3][1] = 0.0f    ; rz.m[3][2] = 0.0f; rz.m[3][3] = 1.0f;

    return rz * ry * rx;
}

//Matrix4f Matrix4f::InitRotation(const Vector3f& axis, float angle)
//{
//	return Matrix4f(DirectX::XMMatrixRotationAxis(DirectX::XMVectorSet(axis.x, axis.y, axis.z, 0), angle));
//}
//
Matrix4f Matrix4f::InitRotation(const Vector3f& forward, const Vector3f& up)
{
    Matrix4f r;
    
	Vector3f n = forward.Normalized();
    Vector3f u = up.Normalized().Cross(n);
    Vector3f v = n.Cross(u);
       
    r.m[0][0] = u.x;   r.m[0][1] = u.y;   r.m[0][2] = u.z;   r.m[0][3] = 0.0f;
    r.m[1][0] = v.x;   r.m[1][1] = v.y;   r.m[1][2] = v.z;   r.m[1][3] = 0.0f;
    r.m[2][0] = n.x;   r.m[2][1] = n.y;   r.m[2][2] = n.z;   r.m[2][3] = 0.0f;
    r.m[3][0] = 0.0f;  r.m[3][1] = 0.0f;  r.m[3][2] = 0.0f;  r.m[3][3] = 1.0f;   
      
    return r;   
}

Matrix4f Matrix4f::InitScale(const Vector3f& scale)
{
    Matrix4f r;
    
	r.m[0][0] = scale.x; r.m[0][1] = 0.0f;    r.m[0][2] = 0.0f;    r.m[0][3] = 0.0f;
    r.m[1][0] = 0.0f;    r.m[1][1] = scale.y; r.m[1][2] = 0.0f;    r.m[1][3] = 0.0f;
    r.m[2][0] = 0.0f;    r.m[2][1] = 0.0f;    r.m[2][2] = scale.z; r.m[2][3] = 0.0f;
    r.m[3][0] = 0.0f;    r.m[3][1] = 0.0f;    r.m[3][2] = 0.0f;    r.m[3][3] = 1.0f;
    
    return r;
}

Matrix4f Matrix4f::InitLookAt(const Vector3f& eye, const Vector3f& at, const Vector3f& up)
{
	return InitLookTo(eye, eye - at, up);
}

Matrix4f Matrix4f::InitLookTo(const Vector3f& eye, const Vector3f& direction, const Vector3f& up)
{
    Matrix4f rotation = InitRotation(direction, up);
    Matrix4f position = InitTranslation(eye);
    return rotation * position;
}

Matrix4f Matrix4f::InitPerspective(float angle, float aspect, float zNear, float zFar)
{
    Matrix4f r;
    
	const float ar         = aspect;
    const float zRange     = zNear - zFar;
    const float tanHalfFOV = tanf(angle / 2.0f);

    r.m[0][0] = 1.0f/(tanHalfFOV * ar); r.m[0][1] = 0.0f;            r.m[0][2] = 0.0f;            r.m[0][3] = 0.0;
    r.m[1][0] = 0.0f;                   r.m[1][1] = 1.0f/tanHalfFOV; r.m[1][2] = 0.0f;            r.m[1][3] = 0.0;
    r.m[2][0] = 0.0f;                   r.m[2][1] = 0.0f;            r.m[2][2] = (-zNear - zFar)/zRange ; r.m[2][3] = 2.0f*zFar*zNear/zRange;
    r.m[3][0] = 0.0f;                   r.m[3][1] = 0.0f;            r.m[3][2] = 1.0f;            r.m[3][3] = 0.0;    
    
    return r;
}

Matrix4f::Matrix4f()
{
	m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
    m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
    m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;     
    m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
}

///////////////////////////////////////////////////////////////////////////////
//QUATERNION SECTION
///////////////////////////////////////////////////////////////////////////////

Quaternion::Quaternion(float _x, float _y, float _z, float _w)
{
    x = _x;
    y = _y;
    z = _z;
    w = _w;
}

Quaternion::Quaternion(const Vector3f& axis, float angle)
{
    float sinHalfAngle = sinf(angle / 2);
	float cosHalfAngle = cosf(angle / 2);

    x = axis.x * sinHalfAngle;
    y = axis.y * sinHalfAngle;
    z = axis.z * sinHalfAngle;
    w = cosHalfAngle;
}

Quaternion Quaternion::Normalized() const
{
    return *this/Length();
}


Quaternion Quaternion::Conjugate() const
{
    Quaternion ret(-x, -y, -z, w);
    return ret;
}

float Quaternion::Length() const
{
    return sqrtf(x * x + y * y + z * z + w * w);
}

float Quaternion::LengthSq() const
{
    return x * x + y * y + z * z + w * w;
}

float Quaternion::Dot(const Quaternion& r) const
{
    return x * r.x + y * r.y + z * r.z + w * r.w;
}

#include <cassert>

Quaternion Quaternion::Slerp(const Quaternion& dest, float amt) const
{
    //TODO: Slerp implementation!
    assert(0==0);
    return Quaternion(0,0,0,0);
}
