#pragma once
#include "Vector4.h"
#include "engine\utils\LOpenGL.h"
#include <stdlib.h>
#include <math.h>

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
	
	Color() {
		r = g = b = a = 1.f;
	}
	Color(int hex) {
		r = ((hex & 0x00FF0000) >> 16) / 255.f;
		g = ((hex & 0x0000FF00) >> 8) / 255.f;
		b = ((hex & 0x000000FF) >> 0) / 255.f;
		a = 1.f;
	}
	Color(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.f) {
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}

	Color applyScale(Color p_scalar) {
		return Color(r * p_scalar.r,
			g * p_scalar.g,
			b * p_scalar.b,
			a * p_scalar.a);
	}

	void useColor(GLfloat p_mod = 1.f) const {
		glColor4f(r * p_mod, g * p_mod, b * p_mod, a);
	}

	void useColor(GLfloat p_modR, GLfloat p_modG, GLfloat p_modB, GLfloat p_modA = 1.f) {
		glColor4f(r * p_modR, g * p_modG, b * p_modB, a * p_modA);
	}

	void operator=(Color color) {
		r = color.r;
		g = color.g;
		b = color.b;
		a = color.a;
	}

	Color operator/(GLfloat value) {
		return Color((GLfloat(r) / value), (GLfloat(g) / value), (GLfloat(b) / value), a);
	}

	Color operator*(float v) {
		return Color(GLfloat(r * v), GLfloat(g * v), GLfloat(b * v), a);
	}

	Color operator+(Color c) {
		return Color(c.r >= 0 ? add(r, c.r) : sub(r, c.r), c.g >= 0 ? add(g, c.g) : sub(g, c.g), c.b >= 0 ? add(b, c.b) : sub(b, c.b), c.a >= 0 ? add(a, c.a) : sub(a, c.a));
	}
	Color operator+(GLfloat v) {
		if(v >= 0)
			return Color(add(r, v), add(g, v), add(b, v), a);
		else
			return Color(sub(r, v), sub(g, v), sub(b, v), a);
	}

	Color operator-(GLfloat v) {
		if(v >= 0)
			return Color(sub(r, v), sub(g, v), sub(b, v), a);
		else
			return Color(add(r, v), add(g, v), add(b, v), a);
	}

	bool operator==(Color c) const {
		return (r == c.r && g == c.g && b == c.b && a == c.a);
	}
};
