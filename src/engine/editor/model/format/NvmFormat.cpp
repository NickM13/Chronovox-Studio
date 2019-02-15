#include "engine\editor\model\format\NvmFormat.h"
// .nvm = Nick's Voxel Model

Uint32 NvmFormat::m_index = 0;
char* NvmFormat::m_data = 0;

bool NvmFormat::save(std::string p_fileName, std::vector<Matrix*>& p_matrixList) {
	if (p_fileName.substr(p_fileName.length() - 4, 4) != ".nvm") {
		p_fileName.append(".nvm");
	}
	std::ofstream _file;
	size_t pos = p_fileName.find_last_of('\\');
	std::string folder = "";
	if (pos != -1) {
		folder = p_fileName.substr(0, pos);
	}
	CreateDirectory(folder.c_str(), NULL);
	_file.open(std::string(p_fileName), std::ios::binary);
	{
		if (!_file.good()) {
			Logger::logError("Could not create file \"" + p_fileName + "\"");
			return false;
		}
		// Header
		_file << ".NVM";
		FileExt::writeInt(_file, MODEL_VERSION); // Version
		FileExt::writeInt(_file, 0);
		FileExt::writeInt(_file, p_matrixList.size());

		Matrix* m;
		for (Uint16 i = 0; i < p_matrixList.size(); i++) {
			m = p_matrixList[i];
			// Matrix Header
			FileExt::writeString(_file, m->getName());
			FileExt::writeString(_file, m->getParent());

			FileExt::writeShort(_file, round(m->getPos().x * 2));
			FileExt::writeShort(_file, round(m->getPos().y * 2));
			FileExt::writeShort(_file, round(m->getPos().z * 2));

			FileExt::writeShort(_file, m->getSize().x);
			FileExt::writeShort(_file, m->getSize().y);
			FileExt::writeShort(_file, m->getSize().z);

			Uint8 count = 0;
			Uint16 id = m->getVoxelId({ 0, 0, 0 });
			for (Uint16 x = 0; x < m->getSize().x; x++) {
				for (Uint16 y = 0; y < m->getSize().y; y++) {
					for (Uint16 z = 0; z < m->getSize().z; z++) {
						if (m->getVoxelId({ x, y, z }) == id && count < 255) {
							count++;
						}
						else {
							Color c = MColor::getInstance().getUnit(MVoxel::getInstance().getUnit(id).color);
							FileExt::writeChar(_file, count);
							FileExt::writeChar(_file, Uint8(c.r * 255));
							FileExt::writeChar(_file, Uint8(c.g * 255));
							FileExt::writeChar(_file, Uint8(c.b * 255));
							FileExt::writeChar(_file, MVoxel::getInstance().getUnit(id).interactionType);
							id = m->getVoxelId({ x, y, z });
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
	Logger::logSavedFile(p_fileName);
	return true;
}

bool NvmFormat::load(std::string p_fileName, std::vector<Matrix*>& p_matrixList) {
	std::ifstream _file;
	Uint32 _length;
	bool _success = true;
	_file.open(std::string(p_fileName).c_str(), std::ios::binary);
	{
		if (!_file.good()) {
			Logger::logMissingFile(p_fileName);
			_file.close();
			return false;
		}

		m_index = 0;

		_file.seekg(0, _file.end);
		_length = Uint32(_file.tellg());
		_file.seekg(0, _file.beg);
		m_data = new char[_length];
		_file.read(m_data, _length);

		Sint32 nvm, version, blank, matrixCount;
		nvm = FileExt::readInt(m_data, m_index);
		version = FileExt::readInt(m_data, m_index);
		blank = FileExt::readInt(m_data, m_index);
		matrixCount = FileExt::readInt(m_data, m_index);

		m_index = 0;

		switch (version) {
		case 1:
			_success = load1(p_matrixList);
			break;
		case 2:
			_success = load2(p_matrixList);
			break;
		default:
			_success = false;
			break;
		}

		delete[] m_data;
	}
	_file.close();
	Logger::logLoadedFile(p_fileName);
	return _success;
}

bool NvmFormat::load1(std::vector<Matrix*>& p_matrixList) {
	Sint32 nvm, version, blank, matrixCount;
	nvm = FileExt::readInt(m_data, m_index);
	version = FileExt::readInt(m_data, m_index);
	blank = FileExt::readInt(m_data, m_index);
	matrixCount = FileExt::readInt(m_data, m_index);

	Matrix* m;
	std::string name;
	glm::ivec3 pos, size;
	Sint32 volume;
	Sint32 matrixIndex;
	Uint8 count, r, g, b, a;
	Voxel vox;
	for (Uint16 i = 0; i < matrixCount; i++) {
		// Matrix Header
		name = FileExt::readString(m_data, m_index);

		pos.x = FileExt::readShort(m_data, m_index);
		pos.y = FileExt::readShort(m_data, m_index);
		pos.z = FileExt::readShort(m_data, m_index);

		size.x = FileExt::readShort(m_data, m_index);
		size.y = FileExt::readShort(m_data, m_index);
		size.z = FileExt::readShort(m_data, m_index);

		m = new Matrix(i, name, "", glm::vec3(pos) / glm::vec3(2), size);

		volume = size.x * size.y * size.z;
		matrixIndex = 0;

		while (matrixIndex < volume) {
			count = FileExt::readChar(m_data, m_index);
			r = FileExt::readChar(m_data, m_index);
			g = FileExt::readChar(m_data, m_index);
			b = FileExt::readChar(m_data, m_index);
			a = FileExt::readChar(m_data, m_index);
			vox = Voxel(a, MColor::getInstance().getUnitID(Color(r / 255.f, g / 255.f, b / 255.f)));
			for (Sint32 i = matrixIndex; i < matrixIndex + count; i++)
				m->setVoxel(glm::ivec3(fmod(floorf(GLfloat(i) / (size.z * size.y)), size.x), fmod(floorf(GLfloat(i) / (size.z)), size.y), fmod(i, size.z)), vox);

			matrixIndex += count;
		}
		p_matrixList.push_back(m);
	}
	return true;
}

bool NvmFormat::load2(std::vector<Matrix*>& p_matrixList) {
	Sint32 nvm, version, blank, matrixCount;
	nvm = FileExt::readInt(m_data, m_index);
	version = FileExt::readInt(m_data, m_index);
	blank = FileExt::readInt(m_data, m_index);
	matrixCount = FileExt::readInt(m_data, m_index);

	Matrix* m;
	std::string name, parent;
	glm::ivec3 pos, size;
	Sint32 volume;
	Sint32 matrixIndex;
	Uint8 count, r, g, b, a;
	Voxel vox;
	for (Uint16 i = 0; i < matrixCount; i++) {
		// Matrix Header
		name = FileExt::readString(m_data, m_index);
		parent = FileExt::readString(m_data, m_index);

		pos.x = FileExt::readShort(m_data, m_index);
		pos.y = FileExt::readShort(m_data, m_index);
		pos.z = FileExt::readShort(m_data, m_index);

		size.x = FileExt::readShort(m_data, m_index);
		size.y = FileExt::readShort(m_data, m_index);
		size.z = FileExt::readShort(m_data, m_index);

		m = new Matrix(i, name, parent, glm::vec3(pos) / glm::vec3(2), size);

		volume = size.x * size.y * size.z;
		matrixIndex = 0;

		while (matrixIndex < volume) {
			count = FileExt::readChar(m_data, m_index);
			r = FileExt::readChar(m_data, m_index);
			g = FileExt::readChar(m_data, m_index);
			b = FileExt::readChar(m_data, m_index);
			a = FileExt::readChar(m_data, m_index);
			vox = Voxel(a, MColor::getInstance().getUnitID(Color(r / 255.f, g / 255.f, b / 255.f)));
			for (Sint32 i = matrixIndex; i < matrixIndex + count; i++)
				m->setVoxel(glm::ivec3(fmod(floorf(GLfloat(i) / (size.z * size.y)), size.x), fmod(floorf(GLfloat(i) / (size.z)), size.y), fmod(i, size.z)), vox);

			matrixIndex += count;
		}
		p_matrixList.push_back(m);
	}
	return true;
}
