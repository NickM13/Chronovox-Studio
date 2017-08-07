#pragma once
#include <math.h>

template< class T >
struct Vector4
{
	typedef float GLfloat;
	typedef unsigned short Uint16;

	T x1, y1, x2, y2;

	Vector4<T>()
	{
		x1 = y1 = x2 = y2 = T(0);
	}

	Vector4<T>(T p_x1, T p_y1, T p_x2, T p_y2) :
		x1(p_x1), y1(p_y1), x2(p_x2), y2(p_y2) {}

	template<class C>
	Vector4<T>(Vector4<C> p_v)
	{
		x1 = T(p_v.x1);
		y1 = T(p_v.y1);
		x2 = T(p_v.x2);
		y2 = T(p_v.y2);
	}

	Vector4<T> operator+(T v) const
	{ return Vector4<T>(x1 + v, y1 + v, x2 + v, y2 + v); }
	Vector4<T> operator+(Vector4 v) const
	{ return Vector4<T>(x1 + v.x1, y1 + v.y1, x2 + v.x2, y2 + v.y2); }

	Vector4<T> operator-(T v) const
	{ return Vector4<T>(x1 - v, y1 - v, x2 - v, y2 - v); }
	Vector4<T> operator-(Vector4 v) const
	{ return Vector4<T>(x1 - v.x1, y1 - v.y1, x2 - v.x2, y2 - v.y2); }

	Vector4<T> operator/(T v) const
	{ return Vector4<T>(x1 / v, y1 / v, x2 / v, y2 / v); }
	Vector4<T> operator/(Vector4 v) const
	{ return Vector4<T>(x1 / v.x1, y1 / v.y1, x2 / v.x2, y2 / v.y2); }

	Vector4<T> operator*(T v) const
	{ return Vector4<T>(x1 * v, y1 * v, x2 * v, y2 * v); }
	Vector4<T> operator*(Vector4 v) const
	{ return Vector4<T>(x1 * v.x1, y1 * v.y1, x2 * v.x2, y2 * v.y2); }

	Vector4<T> operator%(Uint16 v) const
	{ return Vector4<T>(T(fmod(Uint16(x1), v)), T(fmod(Uint16(y1), v)), T(fmod(Uint16(x2), v)), T(fmod(Uint16(y2), v))); }
	Vector4<T> operator%(Vector4< Uint16 > v) const
	{ return Vector4<T>(T(fmod(Uint16(x1), v.x1)), T(fmod(Uint16(y1), v.y1)), T(fmod(Uint16(x2), v.x2)), T(fmod(Uint16(y2), v.y2))); }

	bool operator==(Vector4 v) const
	{ return bool(x1 == v.x1 && y1 == v.y1 && x2 == v.x2 && y2 == v.y2); }

	Vector4<T> abs(GLfloat p_denominator = 1) const
	{ return Vector4<T>(T(fabsf(GLfloat(x1) / p_denominator)), T(fabsf(GLfloat(y1) / p_denominator)), T(fabsf(GLfloat(x2) / p_denominator)), T(fabsf(GLfloat(y2) / p_denominator))); }

	Vector4<T> floor(GLfloat p_denominator = 1) const
	{ return Vector4<T>(T(floorf(GLfloat(x1) / p_denominator)), T(floorf(GLfloat(y1) / p_denominator)), T(floorf(GLfloat(x2) / p_denominator)), T(floorf(GLfloat(y2) / p_denominator))); }

	Vector4<T> round(GLfloat p_denominator = 1) const
	{ return Vector4<T>(T(roundf(GLfloat(x1) / p_denominator)), T(roundf(GLfloat(y1) / p_denominator)), T(roundf(GLfloat(x2) / p_denominator)), T(roundf(GLfloat(y2) / p_denominator))); }

	Vector4<T> ceil(GLfloat p_denominator = 1) const
	{ return Vector4<T>(T(ceilf(GLfloat(x1) / p_denominator)), T(ceilf(GLfloat(y1) / p_denominator)), T(ceilf(GLfloat(x2) / p_denominator)), T(ceilf(GLfloat(y2) / p_denominator))); }

	Vector4<T> mod(T p_value) const
	{ return Vector4<T>(T(modf(x1, p_value)), T(modf(y1, p_value)), T(modf(x2, p_value)), T(modf(y2, p_value))); }

	void print() const
	{ std::cout << x1 << ", " << y1 << ", " << x2 << ", " << y2 << std::endl; }
};
