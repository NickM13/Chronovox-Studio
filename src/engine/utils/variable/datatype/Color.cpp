#include "engine\utils\variable\datatype\Color.h"
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <string>

Color::Color() {
	r = g = b = a = 1.f;
}
Color::Color(std::string hex) {
	if (hex.substr(0, 2) == "0x") {
		hex = hex.substr(2);
	}
	if (hex.length() == 6) {
		r = static_cast<GLfloat>(std::stol(hex.substr(0, 2), nullptr, 16) / 255.f);
		g = static_cast<GLfloat>(std::stol(hex.substr(2, 2), nullptr, 16) / 255.f);
		b = static_cast<GLfloat>(std::stol(hex.substr(4, 2), nullptr, 16) / 255.f);
		a = 1.f;
	} else if (hex.length() == 8) {
		r = static_cast<GLfloat>(std::stol(hex.substr(0, 2), nullptr, 16) / 255.f);
		g = static_cast<GLfloat>(std::stol(hex.substr(2, 2), nullptr, 16) / 255.f);
		b = static_cast<GLfloat>(std::stol(hex.substr(4, 2), nullptr, 16) / 255.f);
		a = static_cast<GLfloat>(std::stol(hex.substr(6, 2), nullptr, 16) / 255.f);
	} else {
		std::cout << "Unknown hexadecimal color format" << std::endl;
		r = g = b = a = 1.f;
		return;
	}
}
Color::Color(GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}

Color Color::applyScale(GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
	return Color(	static_cast<GLfloat>(this->r * r),
					static_cast<GLfloat>(this->g * g),
					static_cast<GLfloat>(this->b * b),
					static_cast<GLfloat>(this->a * a));
}

void Color::useColor(GLfloat p_mod) {
	glColor4f(r * p_mod, g * p_mod, b * p_mod, a);
}

void Color::useColor(GLfloat p_modR, GLfloat p_modG, GLfloat p_modB, GLfloat p_modA) {
	glColor4f(r * p_modR, g * p_modG, b * p_modB, a * p_modA);
}

void Color::operator=(Color color) {
	r = color.r;
	g = color.g;
	b = color.b;
	a = color.a;
}

Color Color::operator/(GLfloat value) {
	return Color(	static_cast<GLfloat>(r / value),
					static_cast<GLfloat>(g / value),
					static_cast<GLfloat>(b / value),
					a);
}

Color Color::operator*(GLfloat v) {
	return Color(	static_cast<GLfloat>(r * v),
					static_cast<GLfloat>(g * v),
					static_cast<GLfloat>(b * v),
					a);
}

Color Color::operator+(Color c) {
	return Color(	c.r >= 0 ? add(r, c.r) : sub(r, c.r),
					c.g >= 0 ? add(g, c.g) : sub(g, c.g),
					c.b >= 0 ? add(b, c.b) : sub(b, c.b),
					c.a >= 0 ? add(a, c.a) : sub(a, c.a));
}
Color Color::operator+(GLfloat v) {
	if (v >= 0)	return Color(add(r, v), add(g, v), add(b, v), a);
	else		return Color(sub(r, v), sub(g, v), sub(b, v), a);
}

Color Color::operator-(GLfloat v) {
	if (v >= 0)	return Color(sub(r, v), sub(g, v), sub(b, v), a);
	else		return Color(add(r, v), add(g, v), add(b, v), a);
}

bool Color::operator<(const Color& c) const {
	return r < c.r || (r == c.r &&
		(g < c.g || (g == c.g &&
		(b < c.b || (b == c.b &&
			a < c.a)))));
}
bool Color::operator==(const Color& c) const {
	return (r == c.r &&
			g == c.g &&
			b == c.b &&
			a == c.a);
}
bool Color::operator!=(const Color& c) const {
	return (r != c.r ||
			g != c.g ||
			b != c.b ||
			a != c.a);
}
