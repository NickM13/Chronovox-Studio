#include "engine\editor\model\format\QbFormat.h"

bool QbFormat::load(std::string p_fileName, std::vector<Matrix*>& p_matrixList)
{
	std::cout << "loading QB" << std::endl;
	std::ifstream _file;
	Uint32 _length, _index;
	char* _data;
	_file.open(std::string(p_fileName).c_str(), std::ios::binary);
	{
		if(!_file.good())
		{
			std::cerr << "Error: File \"" << p_fileName << "\" not found." << std::endl;
			_file.close();
			return false;
		}

		_index = 0;

		_file.seekg(0, _file.end);
		_length = Uint32(_file.tellg());
		_file.seekg(0, _file.beg);
		_data = new char[_length];
		_file.read(_data, _length);

		Uint32 version, colorFormat, zAxisOriented, compressed, visibilityMaskEncoded, numMatrices;

		version = FileExt::readInt(_data, _index);
		colorFormat = FileExt::readInt(_data, _index);
		zAxisOriented = FileExt::readInt(_data, _index);
		compressed = FileExt::readInt(_data, _index);
		visibilityMaskEncoded = FileExt::readInt(_data, _index);
		numMatrices = FileExt::readInt(_data, _index);


		Matrix* m;
		std::string name;
		glm::ivec3 pos;
		glm::ivec3 size;
		Sint32 matrixIndex;
		Uint8 r, g, b, a;
		Uint32 data, count;
		Voxel voxel;
		const Uint32 CODEFLAG = 2;
		const Uint32 NEXTSLICEFLAG = 6;
		Sint32 x, y, z;

		for(Uint32 i = 0; i < numMatrices; i++) {
			name = FileExt::readString(_data, _index);
			size.x = FileExt::readInt(_data, _index);
			size.y = FileExt::readInt(_data, _index);
			size.z = FileExt::readInt(_data, _index);
			pos.x = FileExt::readInt(_data, _index);
			pos.y = FileExt::readInt(_data, _index);
			pos.z = FileExt::readInt(_data, _index);
			m = new Matrix(i, name, "", pos, size);
			if(!compressed) {
				for(z = 0; z < size.z; z++) {
					for(y = 0; y < size.y; y++) {
						for(x = 0; x < size.x; x++) {
							data = FileExt::readInt(_data, _index);
							if(colorFormat == 0) {
								a = (data & 0xFF000000) >> 24;
								b = (data & 0x00FF0000) >> 16;
								g = (data & 0x0000FF00) >> 8;
								r = (data & 0x000000FF);
							}
							else {
								r = (data & 0xFF000000) >> 24;
								g = (data & 0x00FF0000) >> 16;
								b = (data & 0x0000FF00) >> 8;
								a = (data & 0x000000FF);
							}
							if(a != 0)
								m->setVoxel(glm::ivec3(x, y, z), Voxel(1, MColor::getInstance().getUnitID(Color(r / 255.f, g / 255.f, b / 255.f))));
						}
					}
				}
			}
			else {
				z = 0;
				while(z < size.z) {
					matrixIndex = 0;
					while(true) {
						data = FileExt::readInt(_data, _index);
						if(data == NEXTSLICEFLAG) {
							break;
						}
						else if(data == CODEFLAG) {
							count = FileExt::readInt(_data, _index);
							data = FileExt::readInt(_data, _index);
							if(data == 0) {
								voxel = Voxel(0, 0);
							}
							else {
								if(colorFormat == 0) {
									a = (data & 0xFF000000) >> 24;
									b = (data & 0x00FF0000) >> 16;
									g = (data & 0x0000FF00) >> 8;
									r = (data & 0x000000FF);
								}
								else {
									r = (data & 0xFF000000) >> 24;
									g = (data & 0x00FF0000) >> 16;
									b = (data & 0x0000FF00) >> 8;
									a = (data & 0x000000FF);
								}
								voxel = Voxel(1, MColor::getInstance().getUnitID(Color(r / 255.f, g / 255.f, b / 255.f)));
							}
							for(Uint32 j = 0; j < count; j++) {
								x = matrixIndex % size.x;
								y = matrixIndex / size.x;
								matrixIndex++;
								m->setVoxel(glm::ivec3(x, y, z), voxel);
							}
						}
						else {
							if(data == 0) {
								voxel = Voxel(0, 0);
							}
							else {
								if(colorFormat == 0) {
									a = (data & 0xFF000000) >> 24;
									b = (data & 0x00FF0000) >> 16;
									g = (data & 0x0000FF00) >> 8;
									r = (data & 0x000000FF);
								}
								else {
									r = (data & 0xFF000000) >> 24;
									g = (data & 0x00FF0000) >> 16;
									b = (data & 0x0000FF00) >> 8;
									a = (data & 0x000000FF);
								}
								voxel = Voxel(1, MColor::getInstance().getUnitID(Color(r / 255.f, g / 255.f, b / 255.f)));
							}
							m->setVoxel(glm::ivec3(matrixIndex % size.x, matrixIndex / size.x, z), voxel);
							matrixIndex++;
						}
					}
					z++;
				}
			}
			p_matrixList.push_back(m);
		}

		delete[] _data;
	}
	_file.close();
	return true;
}
