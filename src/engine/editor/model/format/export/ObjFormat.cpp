#include "engine\editor\model\format\export\ObjFormat.h"
#include "engine\editor\model\format\GFormat.h"
#include "engine\gfx\texture\MTexture.h"
#include "engine\gfx\gui\LGui.h"

#include <set>
#include <list>

Uint32		ObjFormat::m_index = 0;
char* ObjFormat::m_data = 0;

glm::vec3	ObjFormat::m_scale;
glm::vec3	ObjFormat::m_offset;
GLfloat		ObjFormat::m_texelSize;
std::string ObjFormat::m_texSubfolder;

void ObjFormat::init(glm::vec3 p_scale, glm::vec3 p_offset, GLfloat p_texelSize, std::string p_texSubfolder) {
	m_scale = p_scale;
	m_offset = p_offset;
	m_texelSize = p_texelSize;
	m_texSubfolder = p_texSubfolder;
}

bool ObjFormat::save(std::string p_fileName, std::vector<Matrix*>* p_matrixList) {
	std::ofstream _file;
	size_t pos = p_fileName.find_last_of('\\');
	std::string folder = "", filename = "";
	std::string matFolder;
	if (pos != -1) {
		// Todo: Put collection of files into a folder
		// Not clean with Windows file dialog box
		matFolder = folder = p_fileName.substr(0, pos);
		filename = p_fileName.substr(pos + 1, p_fileName.find_last_of('.') - pos - 1); \
	}
	// Todo: CreateDirectory is a windows api function, cross-platformize!
	if (m_texSubfolder != "") {
		matFolder = matFolder + "\\" + m_texSubfolder;
		CreateDirectory(matFolder.c_str(), NULL);
	} else {
		CreateDirectory(folder.c_str(), NULL);
	}
	std::vector<Color> usedColorsSorted;
	std::map<Color, Sint32> usedColorsMap;
	std::map<Sint32, VoxelMesh*> meshMap;
	VoxelMesh* mesh;
	GLfloat invColorSize;

	_file.open(std::string(p_fileName), std::ios::binary);
	{
		if (!_file.good()) {
			Logger::logError("Could not create file \"" + p_fileName + "\"");
			return false;
		}
		_file << "# " + GScreen::getAppName() + " (" + GScreen::getAppVersion() + ") OBJ file: ''" << std::endl;
		_file << "# www.nickvoxel.com/chronovox-studio" << std::endl;

		_file << std::endl;
		_file << "mtllib " << filename << ".mtl" << std::endl;
		_file << std::endl;
		GFormat::setLoadPercent(0.05f);

		std::vector<glm::vec3> vertexList, normalList;
		std::vector<glm::vec3>::iterator it;
		std::vector<glm::vec3>* vertices, * normals;
		std::vector<Color>* colors;
		glm::vec3 matrixPos;

		normalList.push_back(glm::vec3(1, 0, 0));
		normalList.push_back(glm::vec3(-1, 0, 0));
		normalList.push_back(glm::vec3(0, 1, 0));
		normalList.push_back(glm::vec3(0, -1, 0));
		normalList.push_back(glm::vec3(0, 0, 1));
		normalList.push_back(glm::vec3(0, 0, -1));

		for (glm::vec3 n : normalList) {
			_file << "vn " << n.x << " " << n.y << " " << n.z << std::endl;
		}

		_file << std::endl;

		for (Matrix* m : *p_matrixList) {
			mesh = m->getMeshSimple();
			meshMap.insert({ m->getId(), mesh });
			vertices = &mesh->getVertices();
			normals = &mesh->getNormals();
			colors = &mesh->getColors();

			matrixPos = m->getPos();

			for (glm::vec3& v : *vertices) {
				v = (v + matrixPos + m_offset);
				it = std::find(vertexList.begin(), vertexList.end(), v);
				if (it == vertexList.end()) {
					vertexList.push_back(v);
					_file << "v " <<
						v.x * m_scale.x << " " <<
						v.y * m_scale.y << " " <<
						v.z * m_scale.z << std::endl;
				}
			}

			for (Color& c : *colors) {
				if (usedColorsMap.find(c) == usedColorsMap.end()) {
					usedColorsSorted.push_back(c);
					usedColorsMap.insert({ c, static_cast<Sint32>(usedColorsSorted.size()) - 1 });
				}
			}
		}
		invColorSize = 1.f / usedColorsSorted.size();

		_file << std::endl;

		for (size_t i = 0; i < usedColorsSorted.size(); i++) {
			_file << "vt " << (i + 0.5f) * invColorSize << " " << 0.5f << std::endl;
		}

		for (Matrix* m : *p_matrixList) {
			_file << std::endl;
			_file << "o " << m->getName() << std::endl;
			_file << "usemtl " << filename << std::endl;
			_file << "s off" << std::endl;
			_file << std::endl;

			mesh = meshMap.at(m->getId());
			vertices = &mesh->getVertices();
			normals = &mesh->getNormals();
			colors = &mesh->getColors();

			Sint32 fv[4];
			Sint32 fvn;
			Sint32 fvt;

			for (size_t i = 0; i < vertices->size() / 4; i++) {
				fvt = usedColorsMap.at(colors->at(i * 4)) + 1;

				it = std::find(vertexList.begin(), vertexList.end(), vertices->at(i * 4));
				fv[0] = it - vertexList.begin() + 1;
				it = std::find(normalList.begin(), normalList.end(), normals->at(i * 4));
				fvn = it - normalList.begin() + 1;

				it = std::find(vertexList.begin(), vertexList.end(), vertices->at(i * 4 + 1));
				fv[1] = it - vertexList.begin() + 1;

				it = std::find(vertexList.begin(), vertexList.end(), vertices->at(i * 4 + 2));
				fv[2] = it - vertexList.begin() + 1;

				it = std::find(vertexList.begin(), vertexList.end(), vertices->at(i * 4 + 3));
				fv[3] = it - vertexList.begin() + 1;

				_file << "f " << fv[0] << "/" << fvt << "/" << fvn << " " <<
					fv[1] << "/" << fvt << "/" << fvn << " " <<
					fv[2] << "/" << fvt << "/" << fvn << std::endl;
				_file << "f " << fv[2] << "/" << fvt << "/" << fvn << " " <<
					fv[3] << "/" << fvt << "/" << fvn << " " <<
					fv[0] << "/" << fvt << "/" << fvn << std::endl;
			}
			GFormat::setLoadPercent(0.05f + ((GLfloat)(m->getId() + 1) / p_matrixList->size()) * 0.85f);
		}

		for (Matrix* m : *p_matrixList) {
			delete meshMap.at(m->getId());
		}
	}
	_file.close();
	Logger::logSavedFile(p_fileName);

	p_fileName = folder + "\\" + filename + ".mtl";
	_file.open(std::string(p_fileName), std::ios::binary);
	{
		if (!_file.good()) {
			Logger::logError("Could not create file \"" + p_fileName + "\"");
			return false;
		}

		_file << "# Chronovox Studio MTL File: '" << filename << "'" << std::endl;
		_file << "# Material Count: " << std::endl;
		_file << std::endl;
		_file << "newmtl " << filename << std::endl;
		_file << "Ka 0.0 0.0 0.0" << std::endl;
		_file << "Kd 1.0 1.0 1.0" << std::endl;
		_file << "Ks 0.0 0.0 0.0" << std::endl;
		_file << "d 1.0" << std::endl;
		_file << "illum 1" << std::endl;
		_file << "map_Kd " + (m_texSubfolder != "" ? m_texSubfolder + "\\" : "") + filename + ".png" << std::endl;
		GFormat::setLoadPercent(0.95f);
	}
	_file.close();
	Logger::logSavedFile(p_fileName);

	p_fileName = matFolder + "\\" + filename + ".png";
	std::vector<unsigned char> pixels(usedColorsSorted.size() * m_texelSize * m_texelSize * 4);
	for (size_t i = 0; i < usedColorsSorted.size(); i++) {
		Color& c = usedColorsSorted.at(i);
		for (Sint32 x = 0; x < m_texelSize; x++) {
			for (Sint32 y = 0; y < m_texelSize; y++) {
				pixels.at((i * m_texelSize + x + (y * usedColorsSorted.size() * m_texelSize)) * 4 + 0) = c.r * 255;
				pixels.at((i * m_texelSize + x + (y * usedColorsSorted.size() * m_texelSize)) * 4 + 1) = c.g * 255;
				pixels.at((i * m_texelSize + x + (y * usedColorsSorted.size() * m_texelSize)) * 4 + 2) = c.b * 255;
				pixels.at((i * m_texelSize + x + (y * usedColorsSorted.size() * m_texelSize)) * 4 + 3) = c.a * 255;
			}
		}
	}
	MTexture::saveTexturePNG(p_fileName, pixels, usedColorsSorted.size() * m_texelSize, m_texelSize);

	return true;
}
