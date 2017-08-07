#pragma once

#include "..\datatype\Texture.h"
#include "Manager.h"

class MTexture : public Manager<Texture>
{
public:
	Texture& findUnit(std::string p_unitName);
	Uint32 findUnitID(std::string p_unitName);
};
