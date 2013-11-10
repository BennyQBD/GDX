#include "camera.h"

Camera::Camera(const Vector3f& pos, const Vector3f& forward, const Vector3f& up)
{
	m_Pos = pos;
	m_Forward = forward.Normalized();
	m_Up = up.Normalized();
}

void Camera::Move(const Vector3f& amt)
{
	m_Pos += amt;
}

void Camera::RotateX(float angle)
{
	m_Forward = m_Forward.Rotate(Vector3f::RIGHT, angle).Normalized();
	m_Up = m_Up.Rotate(Vector3f::RIGHT, angle).Normalized();
}

void Camera::RotateY(float angle)
{
	m_Forward = m_Forward.Rotate(Vector3f::UP, angle).Normalized();
	m_Up = m_Up.Rotate(Vector3f::UP, angle).Normalized();
}

void Camera::RotateZ(float angle)
{
	m_Forward = m_Forward.Rotate(Vector3f::FORWARD, angle).Normalized();
	m_Up = m_Up.Rotate(Vector3f::FORWARD, angle).Normalized();
}

void Camera::Roll(float angle)
{
	m_Up = m_Up.Rotate(m_Forward, angle).Normalized();
};

void Camera::Pitch(float angle)
{
	Vector3f right = m_Up.Cross(m_Forward).Normalized();

	m_Forward = m_Forward.Rotate(right, angle).Normalized();
	m_Up = m_Forward.Cross(right).Normalized();
};

void Camera::Yaw(float angle)
{
	m_Forward = m_Forward.Rotate(m_Up, angle).Normalized();
};

Matrix4f Camera::ToMatrix()
{
	return Matrix4f::InitLookTo(m_Pos * -1, m_Forward, m_Up);
}

Vector3f Camera::GetForward()
{
	return m_Forward;
}

Vector3f Camera::GetBack()
{
	return m_Forward * -1;
}

Vector3f Camera::GetRight()
{
	return m_Up.Cross(m_Forward).Normalized();
}

Vector3f Camera::GetLeft()
{
	return m_Forward.Cross(m_Up).Normalized();
}

Vector3f Camera::GetUp()
{
	return m_Up;
}

Vector3f Camera::GetDown()
{
	return m_Up * -1;
}
