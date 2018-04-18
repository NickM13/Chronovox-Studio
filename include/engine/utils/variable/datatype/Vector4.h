#pragma once
#include <math.h>

template<class T>
struct Vector4 {
	T x1, y1, x2, y2;

	Vector4<T>()
	{
		x1 = y1 = x2 = y2 = T(0);
	}

	Vector4<T>(T p_x1, T p_y1, T p_x2, T p_y2) :
		x1(p_x1), y1(p_y1), x2(p_x2), y2(p_y2) {}

	template<class C>
	Vector4<T>(Vector4<C> p_v) {
		x1 = T(p_v.x1);
		y1 = T(p_v.y1);
		x2 = T(p_v.x2);
		y2 = T(p_v.y2);
	}
	Vector4<T>(T p_v) {
		x1 = p_v;
		y1 = p_v;
		x2 = p_v;
		y2 = p_v;
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

	Vector4<T> operator%(int v) const
	{ return Vector4<T>(T(fmod(int(x1), v)), T(fmod(int(y1), v)), T(fmod(int(x2), v)), T(fmod(int(y2), v))); }
	Vector4<T> operator%(Vector4< int > v) const
	{ return Vector4<T>(T(fmod(int(x1), v.x1)), T(fmod(int(y1), v.y1)), T(fmod(int(x2), v.x2)), T(fmod(int(y2), v.y2))); }

	bool operator==(Vector4 v) const
	{ return bool(x1 == v.x1 && y1 == v.y1 && x2 == v.x2 && y2 == v.y2); }

	Vector4<T> abs(float p_denominator = 1) const
	{ return Vector4<T>(T(fabsf(float(x1) / p_denominator)), T(fabsf(float(y1) / p_denominator)), T(fabsf(float(x2) / p_denominator)), T(fabsf(float(y2) / p_denominator))); }

	Vector4<T> floor(float p_denominator = 1) const
	{ return Vector4<T>(T(floorf(float(x1) / p_denominator)), T(floorf(float(y1) / p_denominator)), T(floorf(float(x2) / p_denominator)), T(floorf(float(y2) / p_denominator))); }

	Vector4<T> round(float p_denominator = 1) const
	{ return Vector4<T>(T(roundf(float(x1) / p_denominator)), T(roundf(float(y1) / p_denominator)), T(roundf(float(x2) / p_denominator)), T(roundf(float(y2) / p_denominator))); }

	Vector4<T> ceil(float p_denominator = 1) const
	{ return Vector4<T>(T(ceilf(float(x1) / p_denominator)), T(ceilf(float(y1) / p_denominator)), T(ceilf(float(x2) / p_denominator)), T(ceilf(float(y2) / p_denominator))); }

	Vector4<T> mod(T p_value) const
	{ return Vector4<T>(T(modf(x1, p_value)), T(modf(y1, p_value)), T(modf(x2, p_value)), T(modf(y2, p_value))); }

	void print() const
	{ std::cout << x1 << ", " << y1 << ", " << x2 << ", " << y2 << std::endl; }
};
