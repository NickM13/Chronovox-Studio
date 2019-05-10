#pragma once
#include "engine\utils\LOpenGL.h"
#include <string>

struct Color {
private:
	GLfloat sub(GLfloat a, GLfloat b) {
		return GLfloat(fmax(static_cast<double>(a) - b, 0));
	}

	GLfloat add(GLfloat a, GLfloat b) {
		return GLfloat(fmin(static_cast<double>(a) + b, 1.f));
	}
public:
	GLfloat r, g, b, a;
	
	Color();
	Color(std::string hex);
	Color(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.f);

	Color applyScale(Color p_scalar);

	void useColor(GLfloat p_mod = 1.f);

	void useColor(GLfloat p_modR, GLfloat p_modG, GLfloat p_modB, GLfloat p_modA = 1.f);

	void operator=(Color color);

	Color operator/(GLfloat value);

	Color operator*(float v);

	Color operator+(Color c);
	Color operator+(GLfloat v);

	Color operator-(GLfloat v);

	bool operator==(Color c);
};
