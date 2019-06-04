#pragma once

#include "..\..\Matrix.h"

// Avoiding this for now

class VoxFormat {
private:
	static Sint32 strToNum(std::string str);
public:
	static bool load(std::string p_fileName, std::vector<Matrix*>& p_matrixList);
};
