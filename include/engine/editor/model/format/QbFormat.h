#pragma once

#include "..\Matrix.h"

class QbFormat {
public:
	static bool load(std::string p_fileName, std::vector<Matrix*>& p_matrixList);
};
