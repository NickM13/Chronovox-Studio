#pragma once

#include "engine\utils\Utilities.h"
#include "..\Component.h"
#include "engine\gfx\font\Font.h"

class CIcon : public Component
{
public:
	CIcon(std::string p_compName, Texture p_icon, Vector2<Sint32> p_pos, Vector2<Sint32> p_size);

	void render();
};

