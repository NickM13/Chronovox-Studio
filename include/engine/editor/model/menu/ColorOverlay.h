#pragma once

#include "engine\gfx\gui\Component.h"

class ColorOverlay : public Component {
private:
	Texture* m_background1, *m_background2, *m_selectTex;

	Sint32 hue, sat, val;
	Sint32 phue, psat, pval;
	Color color;
	Sint32 pr, pg, pb;

	bool held;
public:
	ColorOverlay(Vector2<Sint32> p_pos, Vector2<Sint32> p_size);

	void setColorRGB(Sint32 r, Sint32 g, Sint32 b);
	void setColorHSV(Sint32 hue, Sint32 sat, Sint32 val);
	
	Color& getColor() { return color; }

	void setR(Sint32 r) { color.r = r / 255.f; }
	Sint32 getR() { return color.r * 255; }
	void setG(Sint32 g) { color.g = g / 255.f; }
	Sint32 getG() { return color.g * 255; }
	void setB(Sint32 b) { color.b = b / 255.f; }
	Sint32 getB() { return color.b * 255; }
	void setHue(Sint32 hue) { this->hue = hue; }
	Sint32 getHue() { return hue; }
	void setSat(Sint32 sat) { this->sat = sat; }
	Sint32 getSat() { return sat; }
	void setVal(Sint32 val) { this->val = val; }
	Sint32 getVal() { return val; }

	void input(Sint8& p_interactFlags);
	void update(GLfloat p_deltaUpdate);
	void render();
};
