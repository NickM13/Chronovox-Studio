#pragma once

#include "..\..\Matrix.h"

// Avoiding this for now

class VoxFormat {
private:
	static Sint32 strToNum(std::string str);

	static Uint32 m_defaultColors[256];

	static Sint32 VOX_;
	static Sint32 MAIN;
	static Sint32 PACK;
	static Sint32 SIZE;
	static Sint32 XYZI;
	static Sint32 RGBA;
public:
	static bool load(std::string p_fileName, std::vector<Matrix*>* p_matrixList);
};
