#pragma once
#include <math.h>

template<typename T>
struct Vector2 {
	T x, y;

	Vector2()
	{ x = y = 0; }

	Vector2(T p_x, T p_y)
		: x(p_x), y(p_y) {}

	template<class C>
	Vector2<T>(Vector2<C> p_v) {
		x = T(p_v.x);
		y = T(p_v.y);
	}
	Vector2<T>(T p_v) {
		x = p_v;
		y = p_v;
	}

	Vector2 operator+(T v) const
	{ return Vector2<T>(x + v, y + v); }
	Vector2 operator+(Vector2 v) const
	{ return Vector2<T>(x + v.x, y + v.y); }

	Vector2 operator-(T v) const
	{ return Vector2<T>(x - v, y - v); }
	Vector2 operator-(Vector2 v) const
	{ return Vector2<T>(x - v.x, y - v.y); }

	Vector2 operator/(T v) const
	{ return Vector2<T>(x / v, y / v); }
	Vector2 operator/(Vector2 v) const
	{ return Vector2<T>(x / v.x, y / v.y); }

	Vector2 operator*(T v) const
	{ return Vector2<T>(x*v, y*v); }
	Vector2 operator*(Vector2 v) const
	{ return Vector2<T>(x*v.x, y*v.y); }

	Vector2 operator%(int v) const
	{ return Vector2<T>(fmod(x, v), fmod(y, v)); }
	Vector2 operator%(Vector2<int> v) const
	{ return Vector2<T>(fmod(x, v.x), fmod(y, v.y)); }

	bool operator==(Vector2 v) const
	{ return bool(x == v.x && y == v.y); }

	Vector2<T> abs(float p_denominator = 1)
	{ return Vector2<T>(fabsf(float(x) / p_denominator), fabsf(float(y) / p_denominator)); }

	Vector2<T> floor(float p_denominator = 1)
	{ return Vector2<T>(floorf(float(x) / p_denominator), floorf(float(y) / p_denominator)); }

	Vector2<T> round(float p_denominator = 1)
	{ return Vector2<T>(roundf(float(x) / p_denominator), roundf(float(y) / p_denominator)); }

	Vector2<T> ceil(float p_denominator = 1)
	{ return Vector2<T>(ceilf(float(x) / p_denominator), ceilf(float(y) / p_denominator)); }

	Vector2<T> mod(T p_val)
	{ return Vector2<T>(modf(x, p_val), modf(y, p_val)); }

	float getLength()
	{ return float(sqrt(x*x + y*y)); }

	void print() const
	{ std::cout << x << ", " << y << std::endl; }
};
