#include "engine\editor\model\format\import\QbclFormat.h"

bool QbclFormat::load(std::string p_fileName, std::vector<Matrix*>& p_matrixList) {
	// Better luck next time
	return false;
	//std::ifstream _file;
	//Uint32 _length, _index;
	//char* _data;
	//_file.open(std::string(p_fileName).c_str(), std::ios::binary);
	//{
	//	if(!_file.good()) {
	//		Logger::logMissingFile(p_fileName);
	//		_file.close();
	//		return false;
	//	}

	//	_index = 0;

	//	_file.seekg(0, _file.end);
	//	_length = Uint32(_file.tellg());
	//	_file.seekg(0, _file.beg);
	//	_data = new char[_length];
	//	_file.read(_data, _length);

	//	Sint32 magic = FileExt::readInt(_data, _index);
	//	Sint32 version = FileExt::readInt(_data, _index);
	//	Sint32 thumbWidth = FileExt::readInt(_data, _index);
	//	Sint32 thumbHeight = FileExt::readInt(_data, _index);
	//	char* thumbBytes = FileExt::readBytes(_data, _index, thumbWidth * thumbHeight * 4);

	//	std::string title = FileExt::readString(_data, _index);
	//	std::string description = FileExt::readString(_data, _index);
	//	std::string keywords= FileExt::readString(_data, _index);
	//	std::string creator = FileExt::readString(_data, _index);
	//	std::string company = FileExt::readString(_data, _index);
	//	std::string website = FileExt::readString(_data, _index);
	//	std::string copyright = FileExt::readString(_data, _index);
	//	// Skip Guid
	//	_index += 16;

	//	// Not sure what these are for
	//	FileExt::readInt(_data, _index);
	//	FileExt::readInt(_data, _index);

	//	std::string modelType = FileExt::readString(_data, _index);
	//	// f0
	//	_index += 3;

	//	Sint32 x = FileExt::readInt(_data, _index);
	//	Sint32 y = FileExt::readInt(_data, _index);
	//	Sint32 z = FileExt::readInt(_data, _index);

	//	// r
	//	_index += 6 * 4;
	//	Sint32 n = FileExt::readInt(_data, _index); // Num of nodes
	//	Sint32 u = FileExt::readInt(_data, _index);
	//	Sint32 v = FileExt::readInt(_data, _index);

	//	std::string node1 = FileExt::readString(_data, _index);
	//	// f2
	//	_index += 3;

	//	Sint32 mx = FileExt::readInt(_data, _index);
	//	Sint32 my = FileExt::readInt(_data, _index);
	//	Sint32 mz = FileExt::readInt(_data, _index);


	//	while(_index < _length - 1) {

	//	}

	//	delete thumbBytes;
	//	delete[] _data;
	//}
	//_file.close();
	//Logger::logLoadedFile(p_fileName);
	//return true;
}
