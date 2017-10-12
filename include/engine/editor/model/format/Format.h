#pragma once

#include "..\Matrix.h"
#include "NvmFormat.h"
#include "QbclFormat.h"
#include "QbFormat.h"
#include "VoxFormat.h"

class Format {
private:
	static Sint32 strToNum(std::string str);
public:
	enum FormatType {
		NONE = 0,
		NVM = 1,
		QBCL = 2,
		QB = 3,
		VOX = 4
	};
	static FormatType valid(std::string p_fileName);
	static bool load(std::string p_fileName, std::vector<Matrix*>& p_matrixList, FormatType p_formatType);
	static bool save(std::string p_fileName, std::vector<Matrix*>& p_matrixList);
};
