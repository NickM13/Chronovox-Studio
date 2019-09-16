#include "engine\editor\model\format\export\StlFormat.h"
#include "engine\editor\model\format\GFormat.h"

#include "engine\utils\logger\Logger.h"
#include "engine\utils\Utilities.h"
#include <fstream>

Uint32		StlFormat::m_index = 0;
char*		StlFormat::m_data = 0;

glm::vec3	StlFormat::m_scale;
glm::vec3	StlFormat::m_offset;

void StlFormat::printHeader(std::ofstream& p_file, std::string p_header) {
	p_file << p_header;
	for (Sint32 i = static_cast<Sint32>(p_header.length()); i < 80; i++) {
		p_file.write("\0", 1);
	}
}

void StlFormat::init(glm::vec3 p_scale, glm::vec3 p_offset) {
	m_scale = p_scale;
	m_offset = p_offset;
}

bool StlFormat::save(std::string p_fileName, std::vector<Matrix*>* p_matrixList) {
	std::ofstream _file;
	size_t pos = p_fileName.find_last_of('\\');
	std::string folder = "";
	if (pos != -1) {
		folder = p_fileName.substr(0, pos);
	}
	// Todo: CreateDirectory is a windows api function, cross-platformize!
	CreateDirectory(folder.c_str(), NULL);
	_file.open(std::string(p_fileName), std::ios::binary);
	{
		if (!_file.good()) {
			Logger::logError("Could not create file \"" + p_fileName + "\"");
			return false;
		}
		// Header (80 bytes)
		printHeader(_file, "Exported from " + GScreen::getAppName() + " (" + GScreen::getAppVersion() + ")");
		Sint32 nTriangles = 0;

		for (Matrix* m : *p_matrixList) {
			nTriangles += static_cast<Sint32>(m->getMesh()->getVertices().size() / 2);
		}

		FileExt::writeInt(_file, nTriangles);

		Sint32 iTriangle = 0;
		Sint16 vertexAttrib;
		VoxelMesh* mesh = 0;
		std::vector<glm::vec3>* vertices, * normals;
		std::vector<Color>* colors;
		glm::vec3 matrixPos;
		for (Matrix* m : *p_matrixList) {
			mesh = m->getMeshSimple();
			vertices = &mesh->getVertices();
			normals = &mesh->getNormals();
			colors = &mesh->getColors();
			matrixPos = m->getPos();
			for (size_t i = 0; i < vertices->size() / 4; i++) {
				vertexAttrib = (static_cast<Sint16>(colors->at(i * 4).r * 31)) +
					(static_cast<Sint16>(colors->at(i * 4).g * 31) << 5) +
					(static_cast<Sint16>(colors->at(i * 4).b * 31) << 10) +
					(1 << 15);

				FileExt::writeFloat(_file, normals->at(i * 4).x);
				FileExt::writeFloat(_file, normals->at(i * 4).y);
				FileExt::writeFloat(_file, normals->at(i * 4).z);

				FileExt::writeFloat(_file, (vertices->at(i * 4).x + matrixPos.x + m_offset.x) * m_scale.x);
				FileExt::writeFloat(_file, (vertices->at(i * 4).y + matrixPos.y + m_offset.y) * m_scale.y);
				FileExt::writeFloat(_file, (vertices->at(i * 4).z + matrixPos.z + m_offset.z) * m_scale.z);

				FileExt::writeFloat(_file, (vertices->at(i * 4 + 1).x + matrixPos.x + m_offset.x) * m_scale.x);
				FileExt::writeFloat(_file, (vertices->at(i * 4 + 1).y + matrixPos.y + m_offset.y) * m_scale.y);
				FileExt::writeFloat(_file, (vertices->at(i * 4 + 1).z + matrixPos.z + m_offset.z) * m_scale.z);
				
				FileExt::writeFloat(_file, (vertices->at(i * 4 + 2).x + matrixPos.x + m_offset.x) * m_scale.x);
				FileExt::writeFloat(_file, (vertices->at(i * 4 + 2).y + matrixPos.y + m_offset.y) * m_scale.y);
				FileExt::writeFloat(_file, (vertices->at(i * 4 + 2).z + matrixPos.z + m_offset.z) * m_scale.z);

				FileExt::writeShort(_file, vertexAttrib);

				FileExt::writeFloat(_file, normals->at(i * 4).x);
				FileExt::writeFloat(_file, normals->at(i * 4).y);
				FileExt::writeFloat(_file, normals->at(i * 4).z);

				FileExt::writeFloat(_file, (vertices->at(i * 4 + 2).x + matrixPos.x + m_offset.x) * m_scale.x);
				FileExt::writeFloat(_file, (vertices->at(i * 4 + 2).y + matrixPos.y + m_offset.y) * m_scale.y);
				FileExt::writeFloat(_file, (vertices->at(i * 4 + 2).z + matrixPos.z + m_offset.z) * m_scale.z);

				FileExt::writeFloat(_file, (vertices->at(i * 4 + 3).x + matrixPos.x + m_offset.x) * m_scale.x);
				FileExt::writeFloat(_file, (vertices->at(i * 4 + 3).y + matrixPos.y + m_offset.y) * m_scale.y);
				FileExt::writeFloat(_file, (vertices->at(i * 4 + 3).z + matrixPos.z + m_offset.z) * m_scale.z);

				FileExt::writeFloat(_file, (vertices->at(i * 4).x + matrixPos.x + m_offset.x) * m_scale.x);
				FileExt::writeFloat(_file, (vertices->at(i * 4).y + matrixPos.y + m_offset.y) * m_scale.y);
				FileExt::writeFloat(_file, (vertices->at(i * 4).z + matrixPos.z + m_offset.z) * m_scale.z);

				FileExt::writeShort(_file, vertexAttrib);
			}
			GFormat::setLoadPercent(0.05f + ((GLfloat)(m->getId() + 1) / p_matrixList->size()) * 0.9f);
		}
	}
	_file.close();
	Logger::logSavedFile(p_fileName);
	return true;
}
