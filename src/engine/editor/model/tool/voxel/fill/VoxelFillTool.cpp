#include "engine\editor\model\tool\voxel\fill\VoxelFillTool.h"

void VoxelFillTool::FillArea::create(bool p_inset) {
	if (p_inset) insert();
	else        append();
}
void VoxelFillTool::FillArea::destroy() {
	m_fillMatrix = -1;
	m_fillVoxels.clear();
}
void VoxelFillTool::FillArea::vectorAdd(std::vector<glm::ivec3> & list1, std::vector<glm::ivec3> list2, glm::ivec3 element) {
	if (std::find(list1.begin(), list1.end(), element) == list1.end()
		&& std::find(list2.begin(), list2.end(), element) == list2.end())
		list1.push_back(element);
}
bool VoxelFillTool::FillArea::append() {
	Matrix* matrix = 0;
	if (m_editMatrix->getId() == -1
		|| !(matrix = m_editMatrix->getMatrix())->containsPoint(*m_selectedVoxelOffset)) {
		m_fillVoxels.clear();
		return false;
	} else if (std::find(m_fillVoxels.begin(), m_fillVoxels.end(), *m_selectedVoxelOffset) != m_fillVoxels.end()
		&& *m_selectedSide == m_fillSide
		&& m_editMatrix->getId() == m_fillMatrix) return false;

	m_fillVoxels.clear();
	glm::ivec3 _size = matrix->getSize();
	Voxel baseVoxel = matrix->getVoxel(*m_selectedVoxelOffset);
	std::vector<glm::ivec3> check;
	check.push_back(*m_selectedVoxelOffset);

	switch (*m_selectedSide) {
	case FACE_SOUTH:
		while (check.size() > 0) {
			if (check[0].y > 0 && baseVoxel == matrix->getVoxel(check[0] + glm::ivec3(0, -1, 0)) && (check[0].x == _size.x - 1 || matrix->getVoxel(check[0] + glm::ivec3(1, -1, 0)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + glm::ivec3(0, -1, 0));
			if (check[0].y < _size.y - 1 && baseVoxel == matrix->getVoxel(check[0] + glm::ivec3(0, 1, 0)) && (check[0].x == _size.x - 1 || matrix->getVoxel(check[0] + glm::ivec3(1, 1, 0)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + glm::ivec3(0, 1, 0));
			if (check[0].z > 0 && baseVoxel == matrix->getVoxel(check[0] + glm::ivec3(0, 0, -1)) && (check[0].x == _size.x - 1 || matrix->getVoxel(check[0] + glm::ivec3(1, 0, -1)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + glm::ivec3(0, 0, -1));
			if (check[0].z < _size.z - 1 && baseVoxel == matrix->getVoxel(check[0] + glm::ivec3(0, 0, 1)) && (check[0].x == _size.x - 1 || matrix->getVoxel(check[0] + glm::ivec3(1, 0, 1)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + glm::ivec3(0, 0, 1));
			m_fillVoxels.push_back(check[0]);
			check.erase(check.begin());
		}
		break;
	case FACE_NORTH:
		while (check.size() > 0) {
			if (check[0].y > 0 && baseVoxel == matrix->getVoxel(check[0] + glm::ivec3(0, -1, 0)) && (check[0].x == 0 || matrix->getVoxel(check[0] + glm::ivec3(-1, -1, 0)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + glm::ivec3(0, -1, 0));
			if (check[0].y < _size.y - 1 && baseVoxel == matrix->getVoxel(check[0] + glm::ivec3(0, 1, 0)) && (check[0].x == 0 || matrix->getVoxel(check[0] + glm::ivec3(-1, 1, 0)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + glm::ivec3(0, 1, 0));
			if (check[0].z > 0 && baseVoxel == matrix->getVoxel(check[0] + glm::ivec3(0, 0, -1)) && (check[0].x == 0 || matrix->getVoxel(check[0] + glm::ivec3(-1, 0, -1)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + glm::ivec3(0, 0, -1));
			if (check[0].z < _size.z - 1 && baseVoxel == matrix->getVoxel(check[0] + glm::ivec3(0, 0, 1)) && (check[0].x == 0 || matrix->getVoxel(check[0] + glm::ivec3(-1, 0, 1)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + glm::ivec3(0, 0, 1));
			m_fillVoxels.push_back(check[0]);
			check.erase(check.begin());
		}
		break;
	case FACE_BOTTOM:
		while (check.size() > 0) {
			if (check[0].x > 0 && baseVoxel == matrix->getVoxel(check[0] + glm::ivec3(-1, 0, 0)) && (check[0].y == _size.y - 1 || matrix->getVoxel(check[0] + glm::ivec3(-1, 1, 0)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + glm::ivec3(-1, 0, 0));
			if (check[0].x < _size.x - 1 && baseVoxel == matrix->getVoxel(check[0] + glm::ivec3(1, 0, 0)) && (check[0].y == _size.y - 1 || matrix->getVoxel(check[0] + glm::ivec3(1, 1, 0)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + glm::ivec3(1, 0, 0));
			if (check[0].z > 0 && baseVoxel == matrix->getVoxel(check[0] + glm::ivec3(0, 0, -1)) && (check[0].y == _size.y - 1 || matrix->getVoxel(check[0] + glm::ivec3(0, 1, -1)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + glm::ivec3(0, 0, -1));
			if (check[0].z < _size.z - 1 && baseVoxel == matrix->getVoxel(check[0] + glm::ivec3(0, 0, 1)) && (check[0].y == _size.y - 1 || matrix->getVoxel(check[0] + glm::ivec3(0, 1, 1)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + glm::ivec3(0, 0, 1));
			m_fillVoxels.push_back(check[0]);
			check.erase(check.begin());
		}
		break;
	case FACE_TOP:
		while (check.size() > 0) {
			if (check[0].x > 0 && baseVoxel == matrix->getVoxel(check[0] + glm::ivec3(-1, 0, 0)) && (check[0].y == 0 || matrix->getVoxel(check[0] + glm::ivec3(-1, -1, 0)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + glm::ivec3(-1, 0, 0));
			if (check[0].x < _size.x - 1 && baseVoxel == matrix->getVoxel(check[0] + glm::ivec3(1, 0, 0)) && (check[0].y == 0 || matrix->getVoxel(check[0] + glm::ivec3(1, -1, 0)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + glm::ivec3(1, 0, 0));
			if (check[0].z > 0 && baseVoxel == matrix->getVoxel(check[0] + glm::ivec3(0, 0, -1)) && (check[0].y == 0 || matrix->getVoxel(check[0] + glm::ivec3(0, -1, -1)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + glm::ivec3(0, 0, -1));
			if (check[0].z < _size.z - 1 && baseVoxel == matrix->getVoxel(check[0] + glm::ivec3(0, 0, 1)) && (check[0].y == 0 || matrix->getVoxel(check[0] + glm::ivec3(0, -1, 1)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + glm::ivec3(0, 0, 1));
			m_fillVoxels.push_back(check[0]);
			check.erase(check.begin());
		}
		break;
	case FACE_WEST:
		while (check.size() > 0) {
			if (check[0].x > 0 && baseVoxel == matrix->getVoxel(check[0] + glm::ivec3(-1, 0, 0)) && (check[0].z == _size.z - 1 || matrix->getVoxel(check[0] + glm::ivec3(-1, 0, 1)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + glm::ivec3(-1, 0, 0));
			if (check[0].x < _size.x - 1 && baseVoxel == matrix->getVoxel(check[0] + glm::ivec3(1, 0, 0)) && (check[0].z == _size.z - 1 || matrix->getVoxel(check[0] + glm::ivec3(1, 0, 1)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + glm::ivec3(1, 0, 0));
			if (check[0].y > 0 && baseVoxel == matrix->getVoxel(check[0] + glm::ivec3(0, -1, 0)) && (check[0].z == _size.z - 1 || matrix->getVoxel(check[0] + glm::ivec3(0, -1, 1)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + glm::ivec3(0, -1, 0));
			if (check[0].y < _size.y - 1 && baseVoxel == matrix->getVoxel(check[0] + glm::ivec3(0, 1, 0)) && (check[0].z == _size.z - 1 || matrix->getVoxel(check[0] + glm::ivec3(0, 1, 1)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + glm::ivec3(0, 1, 0));
			m_fillVoxels.push_back(check[0]);
			check.erase(check.begin());
		}
		break;
	case FACE_EAST:
		while (check.size() > 0) {
			if (check[0].x > 0 && baseVoxel == matrix->getVoxel(check[0] + glm::ivec3(-1, 0, 0)) && (check[0].z == 0 || matrix->getVoxel(check[0] + glm::ivec3(-1, 0, -1)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + glm::ivec3(-1, 0, 0));
			if (check[0].x < _size.x - 1 && baseVoxel == matrix->getVoxel(check[0] + glm::ivec3(1, 0, 0)) && (check[0].z == 0 || matrix->getVoxel(check[0] + glm::ivec3(1, 0, -1)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + glm::ivec3(1, 0, 0));
			if (check[0].y > 0 && baseVoxel == matrix->getVoxel(check[0] + glm::ivec3(0, -1, 0)) && (check[0].z == 0 || matrix->getVoxel(check[0] + glm::ivec3(0, -1, -1)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + glm::ivec3(0, -1, 0));
			if (check[0].y < _size.y - 1 && baseVoxel == matrix->getVoxel(check[0] + glm::ivec3(0, 1, 0)) && (check[0].z == 0 || matrix->getVoxel(check[0] + glm::ivec3(0, 1, -1)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + glm::ivec3(0, 1, 0));
			m_fillVoxels.push_back(check[0]);
			check.erase(check.begin());
		}
		break;
	}

	m_fillMatrix = matrix->getId();
	m_fillStart = *m_selectedVoxelOffset;
	m_fillSide = *m_selectedSide;

	createMesh();

	return true;
}
bool VoxelFillTool::FillArea::insert() {
	Matrix* matrix;
	if (m_editMatrix->getId() == -1
		|| !(matrix = m_editMatrix->getMatrix())->containsPoint(*m_selectedVoxel)) {
		m_fillVoxels.clear();
		return false;
	} else if ((m_fillStart == *m_selectedVoxel || std::find(m_fillVoxels.begin(), m_fillVoxels.end(), *m_selectedVoxel) != m_fillVoxels.end())
		&& m_editMatrix->getId() == m_fillMatrix) return false;
	m_fillVoxels.clear();
	glm::ivec3 _size = matrix->getSize();
	Voxel baseVoxel = matrix->getVoxel(*m_selectedVoxel);
	if (baseVoxel.interactionType == 0) return false;
	std::vector<glm::ivec3> check;
	check.push_back(*m_selectedVoxel);
	m_fillVoxels.clear();

	GLfloat startTime = glfwGetTime();

	while (check.size() > 0) {
		if (check[0].x > 0 && baseVoxel == matrix->getVoxel(check[0] + glm::ivec3(-1, 0, 0)))		  vectorAdd(check, m_fillVoxels, check[0] + glm::ivec3(-1, 0, 0));
		if (check[0].x < _size.x - 1 && baseVoxel == matrix->getVoxel(check[0] + glm::ivec3(1, 0, 0))) vectorAdd(check, m_fillVoxels, check[0] + glm::ivec3(1, 0, 0));
		if (check[0].y > 0 && baseVoxel == matrix->getVoxel(check[0] + glm::ivec3(0, -1, 0)))		  vectorAdd(check, m_fillVoxels, check[0] + glm::ivec3(0, -1, 0));
		if (check[0].y < _size.y - 1 && baseVoxel == matrix->getVoxel(check[0] + glm::ivec3(0, 1, 0))) vectorAdd(check, m_fillVoxels, check[0] + glm::ivec3(0, 1, 0));
		if (check[0].z > 0 && baseVoxel == matrix->getVoxel(check[0] + glm::ivec3(0, 0, -1)))		  vectorAdd(check, m_fillVoxels, check[0] + glm::ivec3(0, 0, -1));
		if (check[0].z < _size.z - 1 && baseVoxel == matrix->getVoxel(check[0] + glm::ivec3(0, 0, 1))) vectorAdd(check, m_fillVoxels, check[0] + glm::ivec3(0, 0, 1));
		m_fillVoxels.push_back(check[0]);
		check.erase(check.begin());
	}

	m_fillMatrix = matrix->getId();
	m_fillStart = *m_selectedVoxel;

	createMesh();

	return true;
}
void increment(std::map<VoxelFillTool::Coord, Sint8> & p_outline, glm::ivec3 p_coord, Sint8 p_amt) {
	if (p_outline.find(VoxelFillTool::Coord(p_coord + glm::ivec3(0, 0, 0))) == p_outline.end()) p_outline.insert({ VoxelFillTool::Coord(p_coord + glm::ivec3(0, 0, 0)), p_amt });
	else p_outline.at(VoxelFillTool::Coord(p_coord + glm::ivec3(0, 0, 0))) += p_amt;
}
void VoxelFillTool::FillArea::createMesh() {
	m_fillMesh.clear();
	std::map<Coord, Sint8> outlineX, outlineY, outlineZ;

	for (glm::ivec3 vox : m_fillVoxels) {
		// Coordinates are out of order so concurrent lines are easier to find
		increment(outlineX, glm::ivec3(0 + vox.y, 0 + vox.z, 0 + vox.x), 1);
		increment(outlineX, glm::ivec3(1 + vox.y, 0 + vox.z, 0 + vox.x), 2);
		increment(outlineX, glm::ivec3(1 + vox.y, 1 + vox.z, 0 + vox.x), 4);
		increment(outlineX, glm::ivec3(0 + vox.y, 1 + vox.z, 0 + vox.x), 8);

		increment(outlineY, glm::ivec3(0 + vox.z, 0 + vox.x, 0 + vox.y), 1);
		increment(outlineY, glm::ivec3(1 + vox.z, 0 + vox.x, 0 + vox.y), 2);
		increment(outlineY, glm::ivec3(1 + vox.z, 1 + vox.x, 0 + vox.y), 4);
		increment(outlineY, glm::ivec3(0 + vox.z, 1 + vox.x, 0 + vox.y), 8);

		increment(outlineZ, glm::ivec3(0 + vox.x, 0 + vox.y, 0 + vox.z), 1);
		increment(outlineZ, glm::ivec3(1 + vox.x, 0 + vox.y, 0 + vox.z), 2);
		increment(outlineZ, glm::ivec3(1 + vox.x, 1 + vox.y, 0 + vox.z), 4);
		increment(outlineZ, glm::ivec3(0 + vox.x, 1 + vox.y, 0 + vox.z), 8);
	}

	// Create fill mesh
	// X
	std::pair<Coord, Sint8> lastPos = std::pair<Coord, Sint8>(Coord(glm::ivec3(-1)), 0);
	for (std::pair<Coord, Sint8> c : outlineX) {
		if (c.first.vec == lastPos.first.vec && c.second == lastPos.second) {
			m_fillMesh.back().x++;
		} else {
			switch (c.second) {
			case 1:
				m_fillMesh.push_back(glm::vec3(c.first.vec.z, c.first.vec.x, c.first.vec.y));
				m_fillMesh.push_back(glm::vec3(c.first.vec.z + 1.0f, c.first.vec.x, c.first.vec.y));
				break;
			case 2:
				m_fillMesh.push_back(glm::vec3(c.first.vec.z, c.first.vec.x, c.first.vec.y));
				m_fillMesh.push_back(glm::vec3(c.first.vec.z + 1.0f, c.first.vec.x, c.first.vec.y));
				break;
			case 4:
				m_fillMesh.push_back(glm::vec3(c.first.vec.z, c.first.vec.x, c.first.vec.y));
				m_fillMesh.push_back(glm::vec3(c.first.vec.z + 1.0f, c.first.vec.x, c.first.vec.y));
				break;
			case 8:
				m_fillMesh.push_back(glm::vec3(c.first.vec.z, c.first.vec.x, c.first.vec.y));
				m_fillMesh.push_back(glm::vec3(c.first.vec.z + 1.0f, c.first.vec.x, c.first.vec.y));
				break;
			default: break;
			}
		}
		if (c.second == 1 || c.second == 2 || c.second == 4 || c.second == 8)
			lastPos = std::pair<Coord, Sint8>(glm::vec3(c.first.vec.x, c.first.vec.y, c.first.vec.z + 1), c.second);
		else
			lastPos = std::pair<Coord, Sint8>(glm::vec3(-1), 0);
	}
	// Y
	lastPos = std::pair<Coord, Sint8>(Coord(glm::ivec3(-1)), 0);
	for (std::pair<Coord, Sint8> c : outlineY) {
		if (c.first.vec == lastPos.first.vec && c.second == lastPos.second) {
			m_fillMesh.back().y++;
		} else {
			switch (c.second) {
			case 1:
				m_fillMesh.push_back(glm::vec3(c.first.vec.y, c.first.vec.z, c.first.vec.x));
				m_fillMesh.push_back(glm::vec3(c.first.vec.y, c.first.vec.z + 1.0f, c.first.vec.x));
				break;
			case 2:
				m_fillMesh.push_back(glm::vec3(c.first.vec.y, c.first.vec.z, c.first.vec.x));
				m_fillMesh.push_back(glm::vec3(c.first.vec.y, c.first.vec.z + 1.0f, c.first.vec.x));
				break;
			case 4:
				m_fillMesh.push_back(glm::vec3(c.first.vec.y, c.first.vec.z, c.first.vec.x));
				m_fillMesh.push_back(glm::vec3(c.first.vec.y, c.first.vec.z + 1.0f, c.first.vec.x));
				break;
			case 8:
				m_fillMesh.push_back(glm::vec3(c.first.vec.y, c.first.vec.z, c.first.vec.x));
				m_fillMesh.push_back(glm::vec3(c.first.vec.y, c.first.vec.z + 1.0f, c.first.vec.x));
				break;
			default: break;
			}
		}
		if (c.second == 1 || c.second == 2 || c.second == 4 || c.second == 8)
			lastPos = std::pair<Coord, Sint8>(glm::vec3(c.first.vec.x, c.first.vec.y, c.first.vec.z + 1), c.second);
		else
			lastPos = std::pair<Coord, Sint8>(glm::vec3(-1), 0);
	}
	// Z
	lastPos = std::pair<Coord, Sint8>(Coord(glm::ivec3(-1)), 0);
	for (std::pair<Coord, Sint8> c : outlineZ) {
		if (c.first.vec == lastPos.first.vec && c.second == lastPos.second) {
			m_fillMesh.back().z++;
		} else {
			switch (c.second) {
			case 1:
				m_fillMesh.push_back(glm::vec3(c.first.vec.x, c.first.vec.y, c.first.vec.z));
				m_fillMesh.push_back(glm::vec3(c.first.vec.x, c.first.vec.y, c.first.vec.z + 1.0f));
				break;
			case 2:
				m_fillMesh.push_back(glm::vec3(c.first.vec.x, c.first.vec.y, c.first.vec.z));
				m_fillMesh.push_back(glm::vec3(c.first.vec.x, c.first.vec.y, c.first.vec.z + 1.0f));
				break;
			case 4:
				m_fillMesh.push_back(glm::vec3(c.first.vec.x, c.first.vec.y, c.first.vec.z));
				m_fillMesh.push_back(glm::vec3(c.first.vec.x, c.first.vec.y, c.first.vec.z + 1.0f));
				break;
			case 8:
				m_fillMesh.push_back(glm::vec3(c.first.vec.x, c.first.vec.y, c.first.vec.z));
				m_fillMesh.push_back(glm::vec3(c.first.vec.x, c.first.vec.y, c.first.vec.z + 1.0f));
				break;
			default: break;
			}
		}
		if (c.second == 1 || c.second == 2 || c.second == 4 || c.second == 8)
			lastPos = std::pair<Coord, Sint8>(glm::vec3(c.first.vec.x, c.first.vec.y, c.first.vec.z + 1), c.second);
		else
			lastPos = std::pair<Coord, Sint8>(glm::vec3(-1), 0);
	}
}
void VoxelFillTool::FillArea::use(Voxel p_fill) {
	Matrix* matrix = m_editMatrix->getMatrix();
	for (glm::ivec3 v : m_fillVoxels) {
		matrix->setVoxel(v, p_fill);
	}
	m_fillVoxels.clear();
}

VoxelFillTool::VoxelFillTool() {
	m_fillArea = new FillArea();
	m_parent = "Fill";
}
VoxelFillTool::~VoxelFillTool() {
	delete m_fillArea;
}

void VoxelFillTool::enable() {}
void VoxelFillTool::disable() {}

void VoxelFillTool::inputTool() {}
void VoxelFillTool::updateTool() {}
void VoxelFillTool::renderTool() {}

void VoxelFillTool::renderFillMesh() {
	Matrix* matrix = m_editMatrix->getMatrix();
	std::vector<glm::vec3> coordList = m_fillArea->getMesh();
	Shader::pushMatrixModel();
	Shader::translate(matrix->getPos());
	Shader::applyModel();
	for (Sint32 i = 0; i < (Sint32)coordList.size(); i += 2) {
		MMesh::renderLine(coordList[i], coordList[i + 1]);
	}
	Shader::popMatrixModel();
}
