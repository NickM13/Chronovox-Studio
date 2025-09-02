#include "engine\editor\map\SimpleMap.h"

#include <direct.h>
#include <shlobj.h>

SimpleMap::SimpleMap() {

}
SimpleMap::~SimpleMap() {
	for (Chunk *m : m_matrices) {
		delete m;
	}
	m_matrices.clear();
}
void SimpleMap::init() {

}

void SimpleMap::setVoxel(Uint16 p_matrix, glm::ivec3 p_pos, Voxel p_voxel) {
	m_matrices[p_matrix]->setVoxel(p_pos, p_voxel);
}
Voxel SimpleMap::getVoxel(Uint16 p_matrix, glm::ivec3 p_pos) {
	return m_matrices[p_matrix]->getVoxel(p_pos);
}
Uint16 SimpleMap::getVoxelId(Uint16 p_matrix, glm::ivec3 p_pos) {
	return m_matrices[p_matrix]->getVoxelId(p_pos);
}

void SimpleMap::input(Sint8 p_guiFlags) {

}
void SimpleMap::update(GLfloat p_deltaUpdate) {
	for (Chunk* m : m_matrices) {
		m->update();
	}
}
void SimpleMap::render() {
	for (Chunk* m : m_matrices) {
		m->renderChunk();
	}
}
void SimpleMap::renderShadow() {
	for (Chunk* m : m_matrices) {
		m->renderShadow();
	}
}

std::vector<std::string> SimpleMap::getChunkNames() {
	std::vector<std::string> names;
	for (Uint16 i = 0; i < m_matrices.size(); i++) {
		names.push_back("(" + Util::numToStringInt(i) + ") " + m_matrices[i]->getName());
	}
	return names;
}
Chunk* SimpleMap::getChunk(Sint16 id) {
	if (id >= 0 && id < (Sint16)m_matrices.size()) {
		return m_matrices[id];
	}
	return 0;
}
Chunk* SimpleMap::getChunk(std::string p_name) {
	for (Chunk* m : m_matrices) {
		if (m->getName() == p_name) {
			return m;
		}
	}
	return 0;
}

void SimpleMap::open() {
	char documents[MAX_PATH];
	HRESULT res = SHGetFolderPath(NULL, CSIDL_MYDOCUMENTS, NULL, SHGFP_TYPE_CURRENT, documents);

	strcat_s(documents, "\\Chronovox Studio");
	_mkdir(documents);

	char filename[MAX_PATH];
	OPENFILENAME ofn;

	ZeroMemory(&filename, sizeof(filename));
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = "NVM (*.nvm)\0*.nvm*\0QBCL (*.qbcl)\0*.qbcl*\0QB (*.qb)\0*.qb*\0VOX (*.vox)\0*.vox*\0Any File\0*.*\0";
	ofn.lpstrFile = filename;
	if (res == S_OK) ofn.lpstrInitialDir = documents;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = "Open Map (View Only)";
	if (!GetOpenFileName(&ofn)) return;
	loadOpen(filename);
}

bool SimpleMap::loadOpen(std::string p_fileName) {
	LFormat::ImportType _formatType = LFormat::valid(p_fileName);
	if (_formatType != LFormat::ImportType::NONE) {
		m_matrices.clear();
		LFormat::load(p_fileName, &m_matrices, _formatType);
		return true;
	}
	return false;
}
