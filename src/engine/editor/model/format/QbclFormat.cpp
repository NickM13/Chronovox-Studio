#include "engine\editor\model\format\QbclFormat.h"

bool QbclFormat::load(std::string p_fileName, std::vector<Matrix*>& p_matrixList) {
	std::ifstream _file;
	Uint32 _length, _index;
	char* _data;
	_file.open(std::string(p_fileName).c_str(), std::ios::binary);
	{
		if(!_file.good()) {
			Logger::logMissingFile(p_fileName);
			_file.close();
			return false;
		}

		_index = 0;

		_file.seekg(0, _file.end);
		_length = Uint32(_file.tellg());
		_file.seekg(0, _file.beg);
		_data = new char[_length];
		_file.read(_data, _length);

		Sint32 version, chunkId, chunkSize, chunkChildren, chunkIndex;

		version = FileExt::readInt(_data, _index);

		while(_index < _length - 1) {
			chunkId = FileExt::readInt(_data, _index);
			chunkSize = FileExt::readInt(_data, _index);
			chunkChildren = FileExt::readInt(_data, _index);
			chunkIndex = 0;
			while(chunkIndex < chunkSize) {

			}
		}

		delete[] _data;
	}
	_file.close();
	Logger::logLoadedFile(p_fileName);
	return true;
}
