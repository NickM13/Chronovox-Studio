#include "engine\editor\model\format\NvmFormat.h"
// .nvm = Nick's Voxel Model

Uint32 NvmFormat::m_index = 0;
char* NvmFormat::m_data = 0;

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
