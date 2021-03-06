#pragma once

struct Rect
{
public:
	float x, y, w, h;

	Rect() { x = y = w = h = 0; };
	Rect(float p_x, float p_y, float p_w, float p_h) {
		x = p_x;
		y = p_y;
		w = p_w;
		h = p_h;
	}

	bool checkPoint(float p_x, float p_y) {
		return(p_x >= x && p_x <= x + w
			&& p_y >= y && p_y <= y + h);
	}
};
