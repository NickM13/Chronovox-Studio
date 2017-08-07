#pragma once

#include "..\Component.h"

class ColorPanel : public Component
{
public:
	ColorPanel(Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Sint32& p_r, Sint32& p_g, Sint32& p_b, Texture p_texture = Texture());
	void input(Sint8& p_interactFlags, Sint8* p_keyStates, Sint8* p_mouseStates, Vector2<Sint32> p_mousePos);
	void render();

private:
	Texture m_texture;
	Sint32 *r, *g, *b;
};

// "Static" Colored Panel
class SColorPanel : public Component
{
public:
	SColorPanel(std::string p_compName, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Color p_color, Texture p_texture = Texture());
	SColorPanel(Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Color p_color, Texture p_texture = Texture());

	void input(Sint8& p_interactFlags, Sint8* p_keyStates, Sint8* p_mouseStates, Vector2<Sint32> p_mousePos);
	void render();

	Color getColor();
private:
	Texture m_texture;
	Color m_color;
};
