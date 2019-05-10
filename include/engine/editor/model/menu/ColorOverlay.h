#pragma once

#include "engine\gfx\gui\component\Component.h"
#include <iomanip>

class ColorOverlay : public Component {
private:
	Texture* m_background1, * m_background2, * m_background3;
	Texture* m_selectTexOut, * m_selectTexFill;

	Sint32 hue, sat, val;
	Sint32 phue, psat, pval;
	Color color;
	Sint32 pr, pg, pb;
	Sint32 hr, hg, hb; // Colors without saturation or value

	struct HueBar {
		Texture* hueTex, *pickTex;
		Sint32 x, y, w, h;
		bool held;
	} m_hueBar;

	class ColorSquare : public Component {
	private:
		Color color;

	public:
		ColorSquare(Sint32 p_index, Color p_color, Vector2<Sint32> p_offset, Vector2<Sint32> p_size, Sint32 p_columns) {
			setColor(p_color);
			m_pos = p_offset;
			m_pos.x = m_pos.x + (p_index % p_columns) * p_size.x;
			m_pos.y = m_pos.y + (p_index / p_columns) * p_size.y;
			m_size = p_size;
		}

		void input(Sint8& p_interactFlags) {
			Vector2<Sint32> _mousePos = GMouse::getMousePos() - m_pos;
			
		}

		Color getColor() const {
			return color;
		}
		void setColor(Color p_color) {
			color = p_color;
			std::stringstream ss;
			ss << "0x" << std::hex << std::setfill('0') << std::setw(6) << std::uppercase <<
				(static_cast<Sint32>(p_color.r * 255) << 16) +
				(static_cast<Sint32>(p_color.g * 255) << 8) +
				(static_cast<Sint32>(p_color.b * 255) << 0);
			setTooltip(ss.str());
		}
	};

	struct SavedColors {
		Texture* highlightTex;
		Sint32 nColors;
		ColorSquare** colors;
		Vector2<Sint32> offset;
		Vector2<Sint32> size;
		Sint32 columns, rows;
		Sint32 hovered = -1;

		// Ease of use variables
		Vector2<Sint32> colorSize;
	} m_savedColors;

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
