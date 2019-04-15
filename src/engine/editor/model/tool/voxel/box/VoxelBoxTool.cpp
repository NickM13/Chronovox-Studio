#include "engine\editor\model\tool\voxel\box\VoxelBoxTool.h"

VoxelBoxTool::VoxelBoxTool() {
	m_parent = "Box";
}
VoxelBoxTool::~VoxelBoxTool() {

}

void VoxelBoxTool::enable() {}
void VoxelBoxTool::disable() {}

void VoxelBoxTool::inputTool() {}
void VoxelBoxTool::updateTool() {}
void VoxelBoxTool::renderTool() {}

void VoxelBoxTool::box(glm::ivec3 p_pos, Voxel p_voxel) {
	if (!m_boxing && !m_editMatrix->getMatrix()->containsPoint(p_pos)) return;
	glm::ivec3 s = m_editMatrix->getMatrix()->getSize();
	p_pos = glm::ivec3(std::fmaxf(0, std::fminf(p_pos.x, s.x - 1)), std::fmaxf(0, std::fminf(p_pos.y, s.y - 1)), std::fmaxf(0, std::fminf(p_pos.z, s.z - 1)));

	glm::ivec3 start;
	glm::ivec3 end;

	start = glm::ivec3(std::fmaxf(0, std::fminf(m_boxStart.x, m_boxEnd.x)), std::fmaxf(0, std::fminf(m_boxStart.y, m_boxEnd.y)), std::fmaxf(0, std::fminf(m_boxStart.z, m_boxEnd.z)));
	end = glm::ivec3(std::fminf(s.x - 1, std::fmaxf(m_boxStart.x, m_boxEnd.x)), std::fminf(s.y - 1, std::fmaxf(m_boxStart.y, m_boxEnd.y)), std::fminf(s.z - 1, std::fmaxf(m_boxStart.z, m_boxEnd.z)));

	for (Sint32 x = start.x; x <= end.x; x++)
		for (Sint32 y = start.y; y <= end.y; y++)
			for (Sint32 z = start.z; z <= end.z; z++)
				m_editMatrix->getMatrix()->setVoxel({ x, y, z }, m_editMatrix->getInitMatrix()->getVoxel(glm::ivec3(x, y, z)));

	start = glm::ivec3(std::fminf(m_boxStart.x, p_pos.x), std::fminf(m_boxStart.y, p_pos.y), std::fminf(m_boxStart.z, p_pos.z));
	end = glm::ivec3(std::fmaxf(m_boxStart.x, p_pos.x), std::fmaxf(m_boxStart.y, p_pos.y), std::fmaxf(m_boxStart.z, p_pos.z));

	if (m_boxReplace) {
		Voxel match = m_editMatrix->getInitMatrix()->getVoxel(m_boxStart);
		for (Sint32 x = start.x; x <= end.x; x++)
			for (Sint32 y = start.y; y <= end.y; y++)
				for (Sint32 z = start.z; z <= end.z; z++)
					if (match == m_editMatrix->getInitMatrix()->getVoxel({ x, y, z }))
						m_editMatrix->getMatrix()->setVoxel({ x, y, z }, p_voxel);
	} else {
		for (Sint32 x = start.x; x <= end.x; x++)
			for (Sint32 y = start.y; y <= end.y; y++)
				for (Sint32 z = start.z; z <= end.z; z++)
					m_editMatrix->getMatrix()->setVoxel({ x, y, z }, p_voxel);
	}

	m_boxEnd = p_pos;
}

void VoxelBoxTool::renderBoxMesh(bool p_insetVoxel, bool p_insetBox) {
	glm::ivec3 selected;

	glBindTexture(GL_TEXTURE_2D, 0); // TODO Is this necessary?
	GLfloat c;
	glm::vec3 matPos = m_editMatrix->getPos();
	glm::vec3 pos;
	glm::vec3 size;

	if (m_boxing) {
		c = p_insetBox ? -SELECT_CORRECTION : SELECT_CORRECTION;
		pos = matPos + glm::vec3(glm::min(m_boxStart, m_boxEnd)) - glm::vec3(c);
		size = glm::vec3(glm::abs(m_boxStart - m_boxEnd)) + glm::vec3(c * 2 + 1);

		Shader::setColor(m_colorSelect);
		MMesh::renderBoxOutline(pos, size);
	} else {
		if (p_insetVoxel || !m_editMatrix->getMatrix()->containsPoint(*m_selectedVoxel)) {
			selected = *m_selectedVoxelOffset;
			p_insetVoxel = true;
		} else {
			selected = *m_selectedVoxel;
		}

		c = p_insetVoxel ? -SELECT_CORRECTION : SELECT_CORRECTION;
		matPos = m_editMatrix->getPos();
		pos = matPos + glm::vec3(selected) - glm::vec3(c);
		size = glm::vec3(1.f + c * 2);

		Shader::setColor(m_editMatrix->getMatrix()->containsPoint(selected) ? m_colorSelect : m_colorError);
		MMesh::renderBoxOutline(pos, size);
	}
}
