#pragma once

#include "..\Matrix.h"
#include "CsmFormat.h"
#include "NvmFormat.h"
#include "QbclFormat.h"
#include "QbFormat.h"
#include "VoxFormat.h"

class Format {
private:
	static Sint32 strToNum(std::string str);
public:
	enum class FormatType {
		NONE,
		CSM,
		NVM, // OBSELETE
		NVA, // OBSELETE
		QBCL,
		QB,
		VOX
	};
	enum class ExportType {
		NONE,
		OBJ
	};
	static FormatType valid(std::string p_fileName);
	static bool load(std::string p_fileName, std::vector<Matrix*>& p_matrixList, FormatType p_formatType);
	static bool save(std::string p_fileName, std::vector<Matrix*>& p_matrixList);
	static bool exportFile(std::string p_fileName, std::vector<Matrix*>& p_matrixList, ExportType p_exportType);
};
