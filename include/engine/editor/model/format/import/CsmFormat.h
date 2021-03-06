#pragma once
#define MODEL_VERSION 4

#include "..\..\Matrix.h"

class CsmFormat {
private:
	static Uint32 m_index;
	static char* m_data;
public:
	static bool save(std::string p_fileName, std::vector<Matrix*>* p_matrixList);
	static bool load(std::string p_fileName, std::vector<Matrix*>* p_matrixList);
	static bool load1(std::vector<Matrix*>* p_matrixList);
	static bool load2(std::vector<Matrix*>* p_matrixList);
	static bool load3(std::vector<Matrix*>* p_matrixList);
	static bool load4(std::vector<Matrix*>* p_matrixList);
};
