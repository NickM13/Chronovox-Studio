#pragma once

#include "..\Matrix.h"
#include "import\CsmFormat.h"
#include "import\NvmFormat.h"
#include "import\QbclFormat.h"
#include "import\QbFormat.h"
#include "import\VoxFormat.h"

#include "export\ObjFormat.h"
#include "export\PlyFormat.h"
#include "export\StlFormat.h"

#include "GFormat.h"
#include <thread>

class LFormat {
public:
	enum class ImportType {
		NONE,
		CSM,	// Chronovox Studio Model			(*.csm)
		NVM,	// OLD  | Nick's Voxel Model		(*.nvm)
		NVA,	// OLD  | Nick's Voxel Animation	(*.nva)
		QBCL,	// Todo | Qubicle					(*.qbcl)
		QB,		// Qubicle Binary					(*.qb)
		QBT,	// Todo | Qubicle Binary Tree		(*.qbt)
		VOX,	// Todo | MagicaVoxel VOX			(*.vox)
		BINVOX, // Todo | BINVOX					(*.binvox)
		KV6,	// Todo | Voxlap KV6				(*.kv6)
		QEF,	// Todo | QEF						(*.qef)
		RAWVOX, // Todo | RAWVOX					(*.rawvox)
		VOXLAP, // Todo | VoxLap VOX				(*.vox)
		MCS,	// Todo | Minecraft Schematic		(*.schematic)
		PNG		// PNG Image Format					(*.png)
	};
	enum class ExportType {
		NONE,
		STL,
		OBJ,
		PLY,
		FBX,	// Todo
		COLLADA,// Todo
		TDS,	// Todo
		IGES,	// Todo
		STEP,	// Todo
		X3D		// Todo | X3D
	};
private:
	static std::thread m_thread;
	static std::string m_fileName;
	static std::vector<Matrix*>* m_matrixList;
	static ImportType m_formatType;
	static ExportType m_exportType;

	static Sint32 strToNum(std::string str);
public:
	static ImportType valid(std::string p_fileName);
	static bool load(std::string p_fileName, std::vector<Matrix*>* p_matrixList, ImportType p_formatType);
	static bool save(std::string p_fileName, std::vector<Matrix*>* p_matrixList);
	static bool exportFile(std::string p_fileName, std::vector<Matrix*>* p_matrixList, ExportType p_exportType);
};
