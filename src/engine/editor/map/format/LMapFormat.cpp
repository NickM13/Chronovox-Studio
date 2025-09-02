#include "engine\editor\map\format\LMapFormat.h"

#include "engine\utils\logger\Logger.h"
#include "engine\utils\Utilities.h"
#include <fstream>

std::thread LMapFormat::m_thread;
std::string LMapFormat::m_fileName = "";
std::vector<Chunk*>* LMapFormat::m_matrixList = 0;
LMapFormat::ImportType LMapFormat::m_formatType = {};
LMapFormat::ExportType LMapFormat::m_exportType = {};

Sint32 LMapFormat::strToNum(std::string str) {
	Sint32 num = 0;
	for (Sint8 i = 0; i < Sint8(str.length()); i++)
		num += static_cast<Uint8>(str[i]) << (i * 8);
	return num;
}

LMapFormat::ImportType LMapFormat::valid(std::string p_fileName) {
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

	if (typeInt == strToNum(".CSW"))	return ImportType::CSW;

	Logger::logError("File type not supported");
	return ImportType::NONE;
}

bool LMapFormat::load(std::string p_fileName, std::vector<Chunk*>* p_matrixList, ImportType p_formatType) {
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
		case ImportType::CSW:	CswFormat::load(p_fileName, p_matrixList); break;
		default: break;
		}
		GFormat::setLoadPercent(1.f);
		});
	m_thread.detach();
	return true;
}
bool LMapFormat::save(std::string p_fileName, std::vector<Chunk*>* p_matrixList) {
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
bool LMapFormat::exportFile(std::string p_fileName, std::vector<Chunk*>* p_matrixList, ExportType p_exportType) {
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
