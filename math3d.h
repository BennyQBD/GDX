#ifndef MATH3D_H_INCLUDED
#define MATH3D_H_INCLUDED

#define PI 3.1415926536f
#define DEG2RAD PI/ 180.0f
#define RAD2DEG 180.0f / PI

#define ToRadians(x) (float)(((x) * DEG2RAD))
#define ToDegrees(x) (float)(((x) * RAD2DEG))


#include <math.h>

class Quaternion;

class Vector3f
{
public:
        static const Vector3f UP;
        static const Vector3f DOWN;
        static const Vector3f LEFT;
        static const Vector3f RIGHT;
        static const Vector3f FORWARD;
        static const Vector3f BACK;
        static const Vector3f ONE;
        static const Vector3f ZERO;

        float x,y,z;

        Vector3f(float x = 0.0f, float y = 0.0f, float z = 0.0f);

        float Length() const;
        float Dot(const Vector3f& v) const;
        Vector3f Cross(const Vector3f& v) const;
        Vector3f Rotate(const Vector3f& axis, float angle) const;
        Vector3f Rotate(const Quaternion& rotation) const;
        Vector3f Normalized() const;

        inline Vector3f operator+=(const Vector3f& r)
        {
            x += r.x; 
            y += r.y;
            z += r.z;
            return *this;
        }
        
        inline Vector3f operator-=(const Vector3f& r)
        {
            x -= r.x;
            y -= r.y;
            z -= r.z;

            return *this;
        }
        
        inline Vector3f operator*=(float f)
        {
            x *= f;
            y *= f;
            z *= f;

            return *this;
        }
        
        inline Vector3f operator/=(float f)
        {
            x /= f;
            y /= f;
            z /= f;

            return *this;
        }

        inline Vector3f operator+(const Vector3f& r) const {return Vector3f(x + r.x, y + r.y, z + r.z);}
        inline Vector3f operator-(const Vector3f& r) const {return Vector3f(x - r.x, y - r.y, z - r.z);}
        inline Vector3f operator*(float f) const {return Vector3f(x * f, y * f, z * f);}
        inline Vector3f operator/(float f) const {return Vector3f(x / f, y / f, z / f);}

        inline bool operator==(const Vector3f& r) const {return x == r.x && y == r.y && z == r.z;}
        inline bool operator!=(const Vector3f& r) const {return !operator==(r);}
};

class Vector2f
{
public:
    float x;
    float y;

    Vector2f(float x = 0.0f, float y = 0.0f)
    {
        this->x = x;
        this->y = y;
    }

    //TODO: Functions!

    inline Vector2f operator+=(const Vector2f& r)
    {
        x += r.x;
        y += r.y;

        return *this;
    }
    
    inline Vector2f operator-=(const Vector2f& r)
    {
        x -= r.x;
        y -= r.y;

        return *this;
    }
    
    inline Vector2f operator*=(float f)
    {
        x *= f;
        y *= f;

        return *this;
    }

    inline Vector2f operator/=(float f)
    {
        x /= f;
        y /= f;

        return *this;
    }

    inline Vector2f operator+(const Vector2f& r) const {return Vector2f(x + r.x, y + r.y);}
    inline Vector2f operator-(const Vector2f& r) const {return Vector2f(x - r.x, y - r.y);}
    inline Vector2f operator*(float f) const {return Vector2f(x * f, y * f);}
    inline Vector2f operator/(float f) const {return Vector2f(x / f, y / f);}

    inline bool operator==(const Vector2f& r) const {return x == r.x && y == r.y;}
    inline bool operator!=(const Vector2f& r) const {return !operator==(r);}
};

//TODO: Finish implementing matrix class!
class Matrix4f 
{
public:
    static const Matrix4f IDENTITY;

	static Matrix4f InitTranslation(const Vector3f& translation);
	static Matrix4f InitRotation(const Vector3f& eulerAngles);
	static Matrix4f InitRotation(const Vector3f& axis, float angle);
	static Matrix4f InitRotation(const Vector3f& forward, const Vector3f& up);
	static Matrix4f InitScale(const Vector3f& scale);

	static Matrix4f InitLookAt(const Vector3f& eye, const Vector3f& at, const Vector3f& up);
	static Matrix4f InitLookTo(const Vector3f& eye, const Vector3f& direction, const Vector3f& up);
	static Matrix4f InitPerspective(float angle, float aspect, float zNear, float zFar);

    Matrix4f();
    
    Matrix4f Transpose() const;             //TODO: Implement!
    
    inline Matrix4f operator*(const Matrix4f& right) const
    {
        Matrix4f ret;
        for (unsigned int i = 0 ; i < 4 ; i++) 
        {
            for (unsigned int j = 0 ; j < 4 ; j++)
            {
                ret.m[i][j] = m[i][0] * right.m[0][j] +
                              m[i][1] * right.m[1][j] +
                              m[i][2] * right.m[2][j] +
                              m[i][3] * right.m[3][j];
            }
        }
        return ret;
    }
    
    const float* operator[](int index) const {return m[index];}
    float* operator[](int index) {return m[index];}

protected:
private:
    float m[4][4];
        
};

//TODO: Finish importing quaternion methods from 3DEngineDX!
class Quaternion
{
public:
    float x, y, z, w;

    Quaternion(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f, float _w = 0.0f);
    Quaternion(const Vector3f& axis, float angle);

    float Length() const;
	float LengthSq() const;
	float Dot(const Quaternion& r) const;

    Quaternion Normalized() const;
    Quaternion Conjugate() const; 
    Quaternion Slerp(const Quaternion& dest, float amt) const;

    inline Vector3f GetRight() const   {return Vector3f(1.0f - 2.0f * (y * y + z * z), 2.0f * (x * y - w * z), 2.0f * (x * z + w * y));}
    inline Vector3f GetUp() const      {return Vector3f(2.0f * (x * y + w * z), 1.0f - 2.0f * (x * x + z * z), 2.0f * (y * z - w * x));}
    inline Vector3f GetForward() const {return Vector3f(2.0f * (x * z - w * y), 2.0f * (y * z + w * x), 1.0f - 2.0f * (x * x + y * y));}
    
    inline Vector3f GetLeft() const    {return Vector3f(-(1.0f - 2.0f * (y * y + z * z)), -2.0f * (x * y - w * z), -2.0f * (x * z + w * y));}
    inline Vector3f GetDown() const    {return Vector3f(-2.0f * (x * y + w * z), -(1.0f - 2.0f * (x * x + z * z)), -2.0f * (y * z - w * x));}
    inline Vector3f GetBack() const    {return Vector3f(-2.0f * (x * z - w * y), -2.0f * (y * z + w * x), -(1.0f - 2.0f * (x * x + y * y)));}

    inline Matrix4f ToMatrix() const
    {
        Matrix4f r;
        
        Vector3f right = GetRight();
        Vector3f up = GetUp();
        Vector3f forward = GetForward();
            
        r[0][0] = right.x;   r[0][1] = right.y;    r[0][2] = right.z;    r[0][3] = 0.0f;
        r[1][0] = up.x;      r[1][1] = up.y;       r[1][2] = up.z;       r[1][3] = 0.0f;
        r[2][0] = forward.x; r[2][1] = forward.y;  r[2][2] = forward.z;  r[2][3] = 0.0f;
        r[3][0] = 0.0f;      r[3][1] = 0.0f;       r[3][2] = 0.0f;       r[3][3] = 1.0f;
        
        return r;
    }

    inline Vector3f ToEulerAngles() const
    {
        float sqx = x * x;
		float sqy = y * y;
		float sqz = z * z;
		float sqw = w * w;
		float unit = sqx + sqy + sqz + sqw;
		float test = x * y + z * w;
		
		float heading = 0;
		float attitude = 0;
		float bank = 0;
		
		if (test > 0.499f * unit)
		{ // singularity at north pole
			heading = 2.0f * (float)atan2(x, w);
			attitude = (float)PI / 2.0f;
			bank = 0.0f;
		}
		else if (test < -0.499 * unit)
		{ // singularity at south pole
			heading = -2.0f * (float)atan2(x, w);
			attitude = -(float)PI / 2.0f;
			bank = 0.0f;
		}
		else
		{
			heading = (float)atan2(2.0f * y * w - 2.0f * x * z, sqx - sqy - sqz + sqw);
			attitude = (float)asin(2.0f * test / unit);
			bank = (float)atan2(2.0f * x * w - 2.0f * y * z, -sqx + sqy - sqz + sqw);
		}
		
		return Vector3f((float)ToDegrees(bank), (float)ToDegrees(heading), (float)ToDegrees(attitude));
    }

    inline Quaternion operator+(const Quaternion& r) const {return Quaternion(x + r.x, y + r.y, z + r.z, w + r.w);}
    inline Quaternion operator-(const Quaternion& r) const {return Quaternion(x - r.x, y - r.y, z - r.z, w - r.w);}
    inline Quaternion operator*(const Quaternion& r) const
    {
        const float _w = (w * r.w) - (x * r.x) - (y * r.y) - (z * r.z);
        const float _x = (x * r.w) + (w * r.x) + (y * r.z) - (z * r.y);
        const float _y = (y * r.w) + (w * r.y) + (z * r.x) - (x * r.z);
        const float _z = (z * r.w) + (w * r.z) + (x * r.y) - (y * r.x);

        return Quaternion(_x, _y, _z, _w);     
    }
    
    inline Quaternion operator*(const Vector3f& v) const
    {
        const float _w = - (x * v.x) - (y * v.y) - (z * v.z);
        const float _x =   (w * v.x) + (y * v.z) - (z * v.y);
        const float _y =   (w * v.y) + (z * v.x) - (x * v.z);
        const float _z =   (w * v.z) + (x * v.y) - (y * v.x);

        return Quaternion(_x, _y, _z, _w);
    }
    
    inline Quaternion operator*(const float r) const {return Quaternion(x * r, y  * r, z * r, w * r);}
    inline Quaternion operator/(const float r) const {return Quaternion(x / r, y  / r, z / r, w / r);}
};

#endif // MATH3D_H_INCLUDED
