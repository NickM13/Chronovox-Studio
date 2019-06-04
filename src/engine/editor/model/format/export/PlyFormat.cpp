#include "engine\editor\model\format\export\PlyFormat.h"
#include "engine\editor\model\format\GFormat.h"

#include <map>

Uint32		PlyFormat::m_index = 0;
char*		PlyFormat::m_data = 0;

glm::vec3	PlyFormat::m_scale;
glm::vec3	PlyFormat::m_offset;
PlyFormat::PlyMode PlyFormat::m_plyMode = PlyFormat::PlyMode::BIN_L;

Sint32 PlyFormat::find(std::vector<Vertex>& vertexList, Vertex& vertex) {
	std::vector<Vertex>::iterator it;
	for (it = vertexList.begin(); it != vertexList.end(); it++) {
		if (vertex.x == it->x &&
			vertex.y == it->y &&
			vertex.z == it->z &&
			vertex.r == it->r &&
			vertex.g == it->g &&
			vertex.b == it->b) {
			return it - vertexList.begin();
		}
	}
	return vertexList.size();
}

void PlyFormat::init(glm::vec3 p_scale, glm::vec3 p_offset) {
	m_scale = p_scale;
	m_offset = p_offset;
}

bool PlyFormat::save(std::string p_fileName, std::vector<Matrix*>* p_matrixList) {
	GFormat::setLoadPercent(0.f);
	std::ofstream _file;
	size_t pos = p_fileName.find_last_of('\\');
	std::string folder = "";
	if (pos != -1) {
		folder = p_fileName.substr(0, pos);
	}
	// Todo: CreateDirectory is a windows api function, cross-platformize!
	CreateDirectory(folder.c_str(), NULL);

	std::map<Sint32, VoxelMesh*> meshMap;
	VoxelMesh* mesh;

	_file.open(std::string(p_fileName), std::ios::binary);
	{
		if (!_file.good()) {
			Logger::logError("Could not create file \"" + p_fileName + "\"");
			GFormat::setLoadPercent(1.f);
			return false;
		}
		_file << "ply\n";
		switch (m_plyMode) {
		case PlyMode::ASCII:
			_file << "format ascii 1.0" << std::endl;
			break;
		case PlyMode::BIN_L:
		default:
			_file << "format binary_little_endian 1.0" << std::endl;
			break;
		}
		_file << "comment Created by " + GScreen::getAppName() + " (" + GScreen::getAppVersion() + ") - www.nickvoxel.com, source file: '" << p_fileName.substr(pos + 1) << "'" << std::endl;

		Sint32 nTriangles = 0;

		Vertex vertex;
		std::vector<Vertex> vertexList;
		for (Matrix* m : *p_matrixList) {
			mesh = m->getMeshSimple();
			meshMap.insert({ m->getId(), mesh });

			nTriangles += mesh->getVertices().size() / 2;

			std::vector<glm::vec3>& vertices = mesh->getVertices(), & normals = mesh->getNormals();
			std::vector<Color>& colors = mesh->getColors();

			glm::vec3 matrixPos = m->getPos();

			for (size_t i = 0; i < vertices.size(); i++) {
				vertex = {	static_cast<Sint32>((vertices[i].x + matrixPos.x) * 10),
							static_cast<Sint32>((vertices[i].y + matrixPos.y) * 10),
							static_cast<Sint32>((vertices[i].z + matrixPos.z) * 10),
							static_cast<Uint8>(colors[i].r * 255),
							static_cast<Uint8>(colors[i].g * 255),
							static_cast<Uint8>(colors[i].b * 255) };
				if (find(vertexList, vertex) == vertexList.size()) {
					vertexList.push_back(vertex);
				}
			}
		}

		_file << "element vertex " << vertexList.size() << std::endl;
		_file << "property float x" << std::endl;
		_file << "property float y" << std::endl;
		_file << "property float z" << std::endl;
		_file << "property uchar red" << std::endl;
		_file << "property uchar green" << std::endl;
		_file << "property uchar blue" << std::endl;
		_file << "element face " << nTriangles << std::endl;
		_file << "property list uchar uint vertex_indices" << std::endl;
		_file << "end_header" << std::endl;
		GFormat::setLoadPercent(0.05f);

		for (Vertex v : vertexList) {
			switch (m_plyMode) {
			case PlyMode::ASCII:
				_file <<
					((v.x / 10.f) + m_offset.x) * m_scale.x << " " <<
					((v.y / 10.f) + m_offset.y) * m_scale.y << " " <<
					((v.z / 10.f) + m_offset.z) * m_scale.z << " " <<
					(Sint32)v.r << " " <<
					(Sint32)v.g << " " <<
					(Sint32)v.b << std::endl;
				break;
			case PlyMode::BIN_L:
			default:
				FileExt::writeFloat(_file, ((v.x / 10.f) + m_offset.x) * m_scale.x);
				FileExt::writeFloat(_file, ((v.y / 10.f) + m_offset.y) * m_scale.y);
				FileExt::writeFloat(_file, ((v.z / 10.f) + m_offset.z) * m_scale.z);
				FileExt::writeChar(_file, v.r);
				FileExt::writeChar(_file, v.g);
				FileExt::writeChar(_file, v.b);
				break;
			}
		}

		std::vector<glm::vec3>* vertices, * normals;
		std::vector<Color>* colors;
		for (Matrix* m : *p_matrixList) {
			mesh = meshMap.at(m->getId());
			vertices = &mesh->getVertices();
			normals = &mesh->getNormals();
			colors = &mesh->getColors();

			glm::vec3 matrixPos = m->getPos();

			Sint32 fv[4];

			for (size_t i = 0; i < vertices->size() / 4; i++) {
				for (Sint32 j = 0; j < 4; j++) {
					vertex = {	static_cast<Sint32>((vertices->at(i * 4 + j).x + matrixPos.x) * 10),
								static_cast<Sint32>((vertices->at(i * 4 + j).y + matrixPos.y) * 10),
								static_cast<Sint32>((vertices->at(i * 4 + j).z + matrixPos.z) * 10),
								static_cast<Uint8>(colors->at(i * 4 + j).r * 255),
								static_cast<Uint8>(colors->at(i * 4 + j).g * 255),
								static_cast<Uint8>(colors->at(i * 4 + j).b * 255) };
					fv[j] = find(vertexList, vertex);
				}

				switch (m_plyMode) {
				case PlyMode::ASCII:
					_file << "3 " << fv[0] << " " << fv[1] << " " << fv[2] << std::endl;
					_file << "3 " << fv[2] << " " << fv[3] << " " << fv[0] << std::endl;
					break;
				case PlyMode::BIN_L:
				default:
					FileExt::writeChar(_file, 3);
					FileExt::writeInt(_file, fv[0]);
					FileExt::writeInt(_file, fv[1]);
					FileExt::writeInt(_file, fv[2]);

					FileExt::writeChar(_file, 3);
					FileExt::writeInt(_file, fv[2]);
					FileExt::writeInt(_file, fv[3]);
					FileExt::writeInt(_file, fv[0]);
					break;
				}
			}
			GFormat::setLoadPercent(0.9f * ((GLfloat)(m->getId() + 1) / p_matrixList->size()) + 0.05f);
		}
	}
	_file.close();
	Logger::logSavedFile(p_fileName);
	return true;
}
