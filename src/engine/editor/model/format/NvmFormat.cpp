#include "engine\editor\model\format\NvmFormat.h"
// .nvm = Nick's Voxel Model

bool NvmFormat::save(std::string p_fileName, std::vector<Matrix*>& p_matrixList)
{
	if(p_fileName.substr(p_fileName.length() - 4, 4) != ".nvm")
		p_fileName.append(".nvm");
	std::ofstream _file;
	_file.open(std::string(p_fileName), std::ios::binary);
	{
		if(!_file.good())
		{
			std::cerr << "Error saving file." << std::endl;
			return false;
		}
		// Header
		_file << ".NVM";
		FileExt::writeInt(_file, 1);
		FileExt::writeInt(_file, 0);
		FileExt::writeInt(_file, p_matrixList.size());

		Matrix* m;
		for(Uint16 i = 0; i < p_matrixList.size(); i++)
		{
			m = p_matrixList[i];
			// Matrix Header
			FileExt::writeString(_file, m->getName());

			FileExt::writeShort(_file, round(m->getPos().x * 2));
			FileExt::writeShort(_file, round(m->getPos().y * 2));
			FileExt::writeShort(_file, round(m->getPos().z * 2));

			FileExt::writeShort(_file, m->getSize().x);
			FileExt::writeShort(_file, m->getSize().y);
			FileExt::writeShort(_file, m->getSize().z);

			Uint8 count = 0;
			Uint16 id = m->getVoxelId({0, 0, 0});
			for(Uint16 x = 0; x < m->getSize().x; x++)
			{
				for(Uint16 y = 0; y < m->getSize().y; y++)
				{
					for(Uint16 z = 0; z < m->getSize().z; z++)
					{
						if(m->getVoxelId({x, y, z}) == id && count < 255)
						{
							count++;
						}
						else
						{
							Color c = MColor::getInstance().getUnit(MVoxel::getInstance().getUnit(id).color);
							FileExt::writeChar(_file, count);
							FileExt::writeChar(_file, Uint8(c.r * 255));
							FileExt::writeChar(_file, Uint8(c.g * 255));
							FileExt::writeChar(_file, Uint8(c.b * 255));
							FileExt::writeChar(_file, MVoxel::getInstance().getUnit(id).interactionType);
							id = m->getVoxelId({x, y, z});
							count = 1;
						}
					}
				}
			}
			Color c = MColor::getInstance().getUnit(MVoxel::getInstance().getUnit(id).color);
			FileExt::writeChar(_file, count);
			FileExt::writeChar(_file, Uint8(c.r * 255));
			FileExt::writeChar(_file, Uint8(c.g * 255));
			FileExt::writeChar(_file, Uint8(c.b * 255));
			FileExt::writeChar(_file, MVoxel::getInstance().getUnit(id).interactionType);
		}
	}
	_file.close();
	return true;
}

bool NvmFormat::load(std::string p_fileName, std::vector<Matrix*>& p_matrixList)
{
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

		Sint32 nvm, version, blank, matrixCount;
		nvm = FileExt::readInt(_data, _index);
		version = FileExt::readInt(_data, _index);
		blank = FileExt::readInt(_data, _index);
		matrixCount = FileExt::readInt(_data, _index);

		Matrix* m;
		std::string name;
		Vector3<Sint16> pos, size;
		Sint32 volume;
		Sint32 matrixIndex;
		Uint8 count, r, g, b, a;
		Voxel vox;
		for(Uint16 i = 0; i < matrixCount; i++)
		{
			// Matrix Header
			name = FileExt::readString(_data, _index);

			pos.x = FileExt::readShort(_data, _index);
			pos.y = FileExt::readShort(_data, _index);
			pos.z = FileExt::readShort(_data, _index);

			size.x = FileExt::readShort(_data, _index);
			size.y = FileExt::readShort(_data, _index);
			size.z = FileExt::readShort(_data, _index);

			m = new Matrix(name, i, Vector3<GLfloat>(pos) / 2, size);

			volume = size.x * size.y * size.z;
			matrixIndex = 0;

			while(matrixIndex < volume)
			{
				count = FileExt::readChar(_data, _index);
				r = FileExt::readChar(_data, _index);
				g = FileExt::readChar(_data, _index);
				b = FileExt::readChar(_data, _index);
				a = FileExt::readChar(_data, _index);
				vox = Voxel(a, MColor::getInstance().getUnitID(Color(r / 255.f, g / 255.f, b / 255.f)));
				for(Sint32 i = matrixIndex; i < matrixIndex + count; i++)
					m->setVoxel(Vector3<Sint32>(fmod(floorf(GLfloat(i) / (size.z * size.y)), size.x), fmod(floorf(GLfloat(i) / (size.z)), size.y), fmod(i, size.z)), vox);

				matrixIndex += count;
			}
			p_matrixList.push_back(m);
		}

		delete[] _data;
	}
	_file.close();
	return true;
}
