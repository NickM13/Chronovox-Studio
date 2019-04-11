#include "engine\editor\model\tool\voxel\single\VoxelSingleTool.h"

VoxelSingleTool::VoxelSingleTool() {

}
VoxelSingleTool::~VoxelSingleTool() {

}

void VoxelSingleTool::enable() {}
void VoxelSingleTool::disable() {}

void VoxelSingleTool::inputTool() {}
void VoxelSingleTool::updateTool() {}
void VoxelSingleTool::renderTool() {}

void VoxelSingleTool::renderSingleMesh(bool p_inset) {
	glm::ivec3 selected;
	if (p_inset) {
		selected = *m_selectedVoxelOffset;
		if (selected == glm::ivec3(-1)) return;
	} else {
		selected = *m_selectedVoxel;
		if (!m_editMatrix->getMatrix()->containsPoint(selected)) return;
	}

	glBindTexture(GL_TEXTURE_2D, 0); // TODO Is this necessary?
	GLfloat c = p_inset ? -SELECT_CORRECTION : SELECT_CORRECTION;
	glm::vec3 matPos = m_editMatrix->getPos();
	glm::vec3 pos = matPos + glm::vec3(selected) - glm::vec3(c);
	glm::vec3 size = glm::vec3(1.f + c * 2);

	Shader::setColor(m_editMatrix->getMatrix()->containsPoint(selected) ? m_colorSelect : m_colorError);
	MMesh::renderBoxOutline(pos, size);
}
