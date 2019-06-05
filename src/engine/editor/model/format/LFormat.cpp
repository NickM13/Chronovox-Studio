#include "engine\editor\model\format\LFormat.h"

std::thread LFormat::m_thread;
std::string LFormat::m_fileName = "";
std::vector<Matrix*>* LFormat::m_matrixList = 0;
LFormat::ImportType LFormat::m_formatType = {};
LFormat::ExportType LFormat::m_exportType = {};

Sint32 LFormat::strToNum(std::string str) {
	Sint32 num = 0;
	for (Sint8 i = 0; i < Sint8(str.length()); i++)
		num += static_cast<Uint8>(str[i]) << (i * 8);
	return num;
}

LFormat::ImportType LFormat::valid(std::string p_fileName) {
	std::ifstream _file;
	Uint32 _length, _index;
	char* _data;
	Sint32 typeInt;
	_file.open(std::string(p_fileName).c_str(), std::ios::binary);
	if (!_file.good()) {
		Logger::logMissingFile(p_fileName);
		_file.close();
		return ImportType::NONE;
	}
	_index = 0;
	_file.seekg(0, _file.end);
	_length = Uint32(_file.tellg());
	_file.seekg(0, _file.beg);
	_data = new char[_length];
	_file.read(_data, _length);
	typeInt = FileExt::readInt(_data, _index);
	delete[] _data;
	_file.close();

	if (typeInt == strToNum(".CSM"))	return ImportType::CSM;
	if (typeInt == strToNum(".NVM"))	return ImportType::NVM;
	if (typeInt == strToNum(".NVA"))	return ImportType::NVA;
	if (typeInt == strToNum("QBCL"))	return ImportType::QBCL;
	if (typeInt == strToNum("VOX "))	return ImportType::VOX;
	if (typeInt == strToNum("‰PNG"))	return ImportType::PNG;
	if (typeInt == strToNum("QB 2"))	return ImportType::QBT;

	std::string typeStr = p_fileName.substr(p_fileName.find_last_of('.'));
	if (typeStr == ".qb")				return ImportType::QB;

	Logger::logError("File type not supported");
	return ImportType::NONE;
}

bool LFormat::load(std::string p_fileName, std::vector<Matrix*>* p_matrixList, ImportType p_formatType) {
	if (GFormat::getLoadPercent() < 1) {
		Logger::logError("An error has occurred, try again later");
		return false;
	}
	GFormat::setLoadPercent(0.f);

	m_fileName = p_fileName;
	m_matrixList = p_matrixList;
	m_formatType = p_formatType;

	m_thread = std::thread([p_fileName, p_matrixList, p_formatType]() {
		glfwMakeContextCurrent(GScreen::getGLFWWindow());
		switch (p_formatType) {
		case ImportType::CSM:	CsmFormat::load(p_fileName, p_matrixList); break;
		case ImportType::NVM:	CsmFormat::load(p_fileName, p_matrixList); break;
		case ImportType::QBCL:	break;
		case ImportType::QB:	QbFormat::load(p_fileName, p_matrixList); break;
		//case ImportType::VOX:	VoxFormat::load(p_fileName, p_matrixList); break;
		default: break;
		}
		GFormat::setLoadPercent(1.f);
		});
	m_thread.detach();
	return true;
}
bool LFormat::save(std::string p_fileName, std::vector<Matrix*>* p_matrixList) {
	if (GFormat::getLoadPercent() < 1) {
		Logger::logError("An error has occurred, try again later");
		return false;
	}
	GFormat::setLoadPercent(0.f);

	m_fileName = p_fileName;
	m_matrixList = p_matrixList;

	m_thread = std::thread([&]() {
		glfwMakeContextCurrent(GScreen::getGLFWWindow());
		CsmFormat::save(m_fileName, m_matrixList);
		GFormat::setLoadPercent(1.f);
		});
	m_thread.detach();
	return true;
}
bool LFormat::exportFile(std::string p_fileName, std::vector<Matrix*>* p_matrixList, ExportType p_exportType) {
	if (GFormat::getLoadPercent() < 1) {
		Logger::logError("An error has occurred, try again later");
		return false;
	}
	GFormat::setLoadPercent(0.f);

	m_fileName = p_fileName;
	m_matrixList = p_matrixList;
	m_exportType = p_exportType;

	m_thread = std::thread([&]() {
		glfwMakeContextCurrent(GScreen::getGLFWWindow());
		switch (m_exportType) {
		case ExportType::OBJ: ObjFormat::save(m_fileName, m_matrixList); break;
		case ExportType::PLY: PlyFormat::save(m_fileName, m_matrixList); break;
		case ExportType::STL: StlFormat::save(m_fileName, m_matrixList); break;
		default: break;
		}
		GFormat::setLoadPercent(1.f);
		});
	m_thread.detach();
	return true;
}
