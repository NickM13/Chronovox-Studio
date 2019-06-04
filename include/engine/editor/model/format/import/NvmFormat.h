#pragma once

#include "..\..\Matrix.h"

class NvmFormat {
private:
	static Uint32 m_index;
	static char* m_data;
public:
	static bool load(std::string p_fileName, std::vector<Matrix*>* p_matrixList);
	static bool load1(std::vector<Matrix*>* p_matrixList);
	static bool load2(std::vector<Matrix*>* p_matrixList);
};
