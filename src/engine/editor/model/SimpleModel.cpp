#include "engine\editor\model\SimpleModel.h"

#include <direct.h>
#include <shlobj.h>

SimpleModel::SimpleModel() {

}
SimpleModel::~SimpleModel() {
	for (Matrix *m : m_matrices) {
		delete m;
	}
	m_matrices.clear();
}
void SimpleModel::init() {

}

void SimpleModel::setVoxel(Uint16 p_matrix, glm::ivec3 p_pos, Voxel p_voxel) {
	m_matrices[p_matrix]->setVoxel(p_pos, p_voxel);
}
Voxel SimpleModel::getVoxel(Uint16 p_matrix, glm::ivec3 p_pos) {
	return m_matrices[p_matrix]->getVoxel(p_pos);
}
Uint16 SimpleModel::getVoxelId(Uint16 p_matrix, glm::ivec3 p_pos) {
	return m_matrices[p_matrix]->getVoxelId(p_pos);
}

void SimpleModel::input(Sint8 p_guiFlags) {

}
void SimpleModel::update(GLfloat p_deltaUpdate) {
	for (Matrix* m : m_matrices) {
		m->update();
	}
}
void SimpleModel::render() {
	for (Matrix* m : m_matrices) {
		m->renderMatrix();
	}
}
void SimpleModel::renderShadow() {
	for (Matrix* m : m_matrices) {
		m->renderShadow();
	}
}

std::vector<std::string> SimpleModel::getMatrixNames() {
	std::vector<std::string> names;
	for (Uint16 i = 0; i < m_matrices.size(); i++) {
		names.push_back("(" + Util::numToStringInt(i) + ") " + m_matrices[i]->getName());
	}
	return names;
}
Matrix* SimpleModel::getMatrix(Sint16 id) {
	if (id >= 0 && id < (Sint16)m_matrices.size()) {
		return m_matrices[id];
	}
	return 0;
}
Matrix* SimpleModel::getMatrix(std::string p_name) {
	for (Matrix* m : m_matrices) {
		if (m->getName() == p_name) {
			return m;
		}
	}
	return 0;
}

void SimpleModel::open() {
	char documents[MAX_PATH];
	HRESULT res = SHGetFolderPath(NULL, CSIDL_MYDOCUMENTS, NULL, SHGFP_TYPE_CURRENT, documents);

	strcat_s(documents, "\\Voxel Models");
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
	ofn.lpstrTitle = "Open Model (View Only)";
	if (!GetOpenFileName(&ofn)) return;
	loadOpen(filename);
}

bool SimpleModel::loadOpen(std::string p_fileName) {
	Format::FormatType _formatType = Format::valid(p_fileName);
	if (_formatType != Format::FormatType::NONE) {
		m_matrices.clear();
		Format::load(p_fileName, m_matrices, _formatType);
		return true;
	}
	return false;
}
