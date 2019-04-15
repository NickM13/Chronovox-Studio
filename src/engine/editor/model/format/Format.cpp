#include "engine\editor\model\format\Format.h"

Sint32 Format::strToNum(std::string str)
{
	Sint32 num = 0;
	for (Sint8 i = 0; i < Sint8(str.length()); i++)
		num += str[i] << (i * 8);
	return num;
}

Format::FormatType Format::valid(std::string p_fileName) {
	std::ifstream _file;
	Uint32 _length, _index;
	char* _data;
	Sint32 typeInt;
	_file.open(std::string(p_fileName).c_str(), std::ios::binary);
	if (!_file.good())
	{
		Logger::logMissingFile(p_fileName);
		_file.close();
		return FormatType::NONE;
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

	std::string typeStr = p_fileName.substr(p_fileName.find_last_of('.'));
	if (typeStr == ".nvm") return FormatType::NVM;
	else if (typeStr == ".nva") return FormatType::NVA;
	else if (typeStr == ".qbcl") return FormatType::QBCL;
	else if (typeStr == ".qb") return FormatType::QB;
	else if (typeStr == ".vox") return FormatType::VOX;
	else if (typeInt == strToNum(".NVM")) return FormatType::NVM;
	else if (typeInt == strToNum(".NVA")) return FormatType::NVA;
	else if (typeInt == strToNum("QBCL")) return FormatType::QBCL;
	else if (typeInt == strToNum("VOX ")) return FormatType::VOX;
	Logger::logError("File type not supported");
	return FormatType::NONE;
}

bool Format::load(std::string p_fileName, std::vector<Matrix*>& p_matrixList, FormatType p_formatType) {
	switch (p_formatType) {
	case FormatType::NVM: NvmFormat::load(p_fileName, p_matrixList); break;
	case FormatType::QBCL: break;
	case FormatType::QB: QbFormat::load(p_fileName, p_matrixList); break;
	case FormatType::VOX: break;
	default: return false;
	}
	return true;
}
bool Format::save(std::string p_fileName, std::vector<Matrix*>& p_matrixList) {
	return NvmFormat::save(p_fileName, p_matrixList);
}
bool Format::exportFile(std::string p_fileName, std::vector<Matrix*>& p_matrixList, ExportType p_exportType) {
	return NvmFormat::save(p_fileName, p_matrixList);
}
