#include "engine\editor\model\format\VoxFormat.h"

Sint32 VoxFormat::strToNum(std::string str)
{
	Sint32 num = 0;
	for(Sint8 i = 0; i < Sint8(str.length()); i++)
		num += str[i] << (i * 8);
	return num;
}
bool VoxFormat::load(std::string p_fileName, std::vector<Matrix*>& p_matrixList)
{
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

		Sint32 vox, version, chunkId, chunkSize, chunkChildren, chunkIndex;

		vox = FileExt::readInt(_data, _index);
		if(vox != strToNum("VOX ")) {
			Logger::logError("File type not supported \"" + p_fileName + "\"");
			return false;
		}
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
