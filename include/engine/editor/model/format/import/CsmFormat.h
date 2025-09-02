#pragma once
#define MODEL_VERSION 4

#include "..\..\Matrix.h"

class CsmFormat {
private:
	Uint32 m_index = 0;
	char* m_data = 0;
public:
	bool save(std::string p_fileName, std::vector<Matrix*>* p_matrixList);
	bool load(std::string p_fileName, std::vector<Matrix*>* p_matrixList);
	bool load1(std::vector<Matrix*>* p_matrixList);
	bool load2(std::vector<Matrix*>* p_matrixList);
	bool load3(std::vector<Matrix*>* p_matrixList);
	bool load4(std::vector<Matrix*>* p_matrixList);
};
