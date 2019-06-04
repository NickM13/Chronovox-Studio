#pragma once

#include "..\..\Matrix.h"

// Avoiding this for now, currently unsure whether it would cause any copyright issues or not

class QbclFormat {
public:
	static bool load(std::string p_fileName, std::vector<Matrix*>& p_matrixList);
};
