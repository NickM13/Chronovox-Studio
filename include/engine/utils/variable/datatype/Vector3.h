#pragma once
#include <math.h>
#include <string>

template<class T>
struct Vector3 {
	T x, y, z;

	Vector3<T>()
	{ x = y = z = T(0); }

	Vector3<T>(T p_x, T p_y, T p_z) :
		x(p_x), y(p_y), z(p_z) {}

	template<class C>
	Vector3<T>(Vector3<C> p_v)
	{
		x = T(p_v.x);
		y = T(p_v.y);
		z = T(p_v.z);
	}
	Vector3<T>(T p_v) {
		x = p_v;
		y = p_v;
		z = p_v;
	}

	Vector3<T> operator+(T v) const
	{ return Vector3<T>(x + v, y + v, z + v); }
	Vector3<T> operator+(Vector3 v) const
	{ return Vector3<T>(x + v.x, y + v.y, z + v.z); }

	Vector3<T> operator-(T v) const
	{ return Vector3<T>(x - v, y - v, z - v); }
	Vector3<T> operator-(Vector3 v) const
	{ return Vector3<T>(x - v.x, y - v.y, z - v.z); }

	Vector3<T> operator/(T v) const
	{ return Vector3<T>(x / v, y / v, z / v); }
	Vector3<T> operator/(Vector3 v) const
	{ return Vector3<T>(x / v.x, y / v.y, z / v.z); }

	Vector3<T> operator*(T v) const
	{ return Vector3<T>(x*v, y*v, z*v); }
	Vector3<T> operator*(Vector3 v) const
	{ return Vector3<T>(x*v.x, y*v.y, z*v.z); }

	Vector3<T> operator%(int v) const
	{ return Vector3<T>(T(fmod(int(x), v)), T(fmod(int(y), v)), T(fmod(int(z), v))); }
	Vector3<T> operator%(Vector3< int > v) const
	{ return Vector3<T>(T(fmod(int(x), v.x)), T(fmod(int(y), v.y)), T(fmod(int(z), v.z))); }

	bool operator==(Vector3 v) const
	{ return bool(x == v.x && y == v.y && z == v.z); }

	Vector3<T> abs(float p_denominator = 1) const
	{ return Vector3<T>(T(fabsf(float(x) / p_denominator)), T(fabsf(float(y) / p_denominator)), T(fabsf(float(z) / p_denominator))); }

	Vector3<T> floor(float p_denominator = 1) const
	{ return Vector3<T>(T(floorf(float(x + 0.00000001f) / p_denominator)), T(floorf(float(y + 0.00000001f) / p_denominator)), T(floorf(float(z + 0.00000001f) / p_denominator))); }

	Vector3<T> round(float p_denominator = 1) const
	{ return Vector3<T>(T(roundf(float(x) / p_denominator)), T(roundf(float(y) / p_denominator)), T(roundf(float(z) / p_denominator))); }

	Vector3<T> ceil(float p_denominator = 1) const
	{ return Vector3<T>(T(ceilf(float(x - 0.00000001f) / p_denominator)), T(ceilf(float(y - 0.00000001f) / p_denominator)), T(ceilf(float(z - 0.00000001f) / p_denominator))); }

	Vector3<T> mod(float p_value) const
	{ return Vector3<T>(T(std::fmod(x, p_value)), T(std::fmod(y, p_value)), T(std::fmod(z, p_value))); }

	Vector3<T> getNormal() const
	{
		float len = getLength();
		return Vector3<T>((x == 0 ? 0 : T(x / len)), (y == 0 ? 0 : T(y / len)), (z == 0 ? 0 : T(z / len)));
	}

	Vector3<T> cross(Vector3 p_vec) const
	{
		Vector3<T> c;
		c.x = p_vec.y * z - p_vec.z * y;
		c.y = p_vec.z * x - p_vec.x * z;
		c.z = p_vec.x * y - p_vec.y * x;
		return c;
	}

	float dot(Vector3 p_vec) const
	{ return (p_vec.x * x + p_vec.y * y + p_vec.z * z); }

	float getLength() const
	{ return sqrtf(float(x*x + y*y + z*z)); }

	std::string toString() const
	{ return std::string(std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z)); }
};
