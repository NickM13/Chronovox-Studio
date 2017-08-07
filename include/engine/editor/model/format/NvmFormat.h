#pragma once

#include "..\Matrix.h"

class NvmFormat
{
public:
	static bool save(std::string p_fileName, std::vector<Matrix*>& p_matrixList);
	static bool load(std::string p_fileName, std::vector<Matrix*>& p_matrixList);
};
