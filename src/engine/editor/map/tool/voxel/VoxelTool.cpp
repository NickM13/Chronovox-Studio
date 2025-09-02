#include "engine\editor\model\tool\voxel\VoxelTool.h"
#include <set>

glm::ivec3* VoxelTool::m_selectedVoxel, * VoxelTool::m_selectedVoxelOffset;
Sint8* VoxelTool::m_selectedSide;
Color* VoxelTool::m_color;


void VoxelTool::FillArea::create(bool p_inset) {
	if (p_inset)	insert();
	else			append();
}
void VoxelTool::FillArea::destroy() {
	m_fillMatrix = -1;
	m_fillVoxels.clear();
}
void VoxelTool::FillArea::vectorAdd(std::vector<glm::ivec3>& list1, std::vector<glm::ivec3> list2, glm::ivec3 element) {
	if (std::find(list1.begin(), list1.end(), element) == list1.end()
		&& std::find(list2.begin(), list2.end(), element) == list2.end())
		list1.push_back(element);
}

void VoxelTool::FillArea::checkPointAppend(std::vector<glm::ivec2>& fillVoxels, const glm::ivec2& size, bool** checkMap, bool** validMap, std::queue<glm::ivec2>& checkQueue, const glm::ivec2& coord) {
	if (coord.x < 0 || coord.x >= size.x ||
		coord.y < 0 || coord.y >= size.y) return;
	if (!checkMap[coord.x][coord.y]) {
		checkMap[coord.x][coord.y] = true;
		if (validMap[coord.x][coord.y]) {
			checkQueue.push(coord);
			fillVoxels.push_back(coord);
		}
	}
}
std::vector<glm::ivec2> VoxelTool::FillArea::append(const glm::ivec2& size, bool** validMap, const glm::ivec2& start) {
	std::queue<glm::ivec2> checkQueue;
	std::vector<glm::ivec2> fillList;
	bool** checkMap = new bool*[size.x];
	for (Sint32 i = 0; i < size.x; i++) {
		checkMap[i] = new bool[size.y];
		ZeroMemory(checkMap[i], size.y);
	}

	checkQueue.push(start);
	checkMap[start.x][start.y] = true;
	fillList.push_back(start);

	glm::ivec2 fc;
	while (!checkQueue.empty()) {
		fc = checkQueue.front();
		checkPointAppend(fillList, size, checkMap, validMap, checkQueue, { fc.x - 1, fc.y });
		checkPointAppend(fillList, size, checkMap, validMap, checkQueue, { fc.x + 1, fc.y });
		checkPointAppend(fillList, size, checkMap, validMap, checkQueue, { fc.x, fc.y - 1 });
		checkPointAppend(fillList, size, checkMap, validMap, checkQueue, { fc.x, fc.y + 1 });
		checkQueue.pop();
	}

	for (Sint32 i = 0; i < size.x; i++) {
		delete[] checkMap[i];
	}
	delete[] checkMap;

	return fillList;
}
bool VoxelTool::FillArea::append() {
	Matrix* matrix;
	m_editMatrix->getMatrix()->autoResize(m_selectedVoxel, m_selectedVoxelOffset);
	if (m_editMatrix->getId() == -1
		|| !(matrix = m_editMatrix->getMatrix())->containsPoint(*m_selectedVoxelOffset)) {
		m_fillVoxels.clear();
		return false;
	}
	//if ((m_fillStart == *m_selectedVoxel || std::find(m_fillVoxels.begin(), m_fillVoxels.end(), *m_selectedVoxel) != m_fillVoxels.end())
	//	&& m_editMatrix->getId() == m_fillMatrix) return false;
	if (matrix->getVoxel(*m_selectedVoxelOffset).interactionType != 0) return false;
	m_fillVoxels.clear();
	glm::ivec3 msize = matrix->getSize();
	Uint16*** voxelData = matrix->getVoxelData();
	bool** validMap;
	std::vector<glm::ivec2> fillVoxels;

	glm::ivec2 size, start;
	switch (*m_selectedSide) {
	case FACE_SOUTH:
		size = { msize.y, msize.z };
		start = { m_selectedVoxelOffset->y, m_selectedVoxelOffset->z };
		validMap = new bool* [size.x];
		for (Sint32 i = 0; i < size.x; i++) {
			validMap[i] = new bool[size.y];
			if (m_selectedVoxelOffset->x >= msize.x - 1) {
				for (Sint32 j = 0; j < size.y; j++)
					validMap[i][j] =	voxelData[m_selectedVoxelOffset->x + 1][i + 1][j + 1] == 0;
			} else {
				for (Sint32 j = 0; j < size.y; j++)
					validMap[i][j] = (	voxelData[m_selectedVoxelOffset->x + 1][i + 1][j + 1] == 0 &&
										voxelData[m_selectedVoxelOffset->x + 2][i + 1][j + 1] != 0);
			}
		}
		fillVoxels = append(size, validMap, start);
		for (glm::ivec2 c : fillVoxels) {
			m_fillVoxels.push_back(FillCoord(m_selectedVoxelOffset->x, c.x, c.y));
		}
		break;
	case FACE_NORTH:
		size = { msize.y, msize.z };
		start = { m_selectedVoxelOffset->y, m_selectedVoxelOffset->z };
		validMap = new bool* [size.x];
		for (Sint32 i = 0; i < size.x; i++) {
			validMap[i] = new bool[size.y];
			if (m_selectedVoxelOffset->x <= 0) {
				for (Sint32 j = 0; j < size.y; j++)
					validMap[i][j] =	voxelData[m_selectedVoxelOffset->x + 1][i + 1][j + 1] == 0;
			} else {
				for (Sint32 j = 0; j < size.y; j++)
					validMap[i][j] = (	voxelData[m_selectedVoxelOffset->x + 1][i + 1][j + 1] == 0 &&
										voxelData[m_selectedVoxelOffset->x	  ][i + 1][j + 1] != 0);
			}
		}
		fillVoxels = append(size, validMap, start);
		for (glm::ivec2 c : fillVoxels) {
			m_fillVoxels.push_back(FillCoord(m_selectedVoxelOffset->x, c.x, c.y));
		}
		break;
	case FACE_BOTTOM:
		size = { msize.z, msize.x };
		start = { m_selectedVoxelOffset->z, m_selectedVoxelOffset->x };
		validMap = new bool* [size.x];
		for (Sint32 i = 0; i < size.x; i++) {
			validMap[i] = new bool[size.y];
			if (m_selectedVoxelOffset->y >= msize.y - 1) {
				for (Sint32 j = 0; j < size.y; j++)
					validMap[i][j] =	voxelData[j + 1][m_selectedVoxelOffset->y + 1][i + 1] == 0;
			} else {
				for (Sint32 j = 0; j < size.y; j++)
					validMap[i][j] = (	voxelData[j + 1][m_selectedVoxelOffset->y + 1][i + 1] == 0 &&
										voxelData[j + 1][m_selectedVoxelOffset->y + 2][i + 1] != 0);
			}
		}
		fillVoxels = append(size, validMap, start);
		for (glm::ivec2 c : fillVoxels) {
			m_fillVoxels.push_back(FillCoord(c.y, m_selectedVoxelOffset->y, c.x));
		}
		break;
	case FACE_TOP:
		size = { msize.z, msize.x };
		start = { m_selectedVoxelOffset->z, m_selectedVoxelOffset->x };
		validMap = new bool* [size.x];
		for (Sint32 i = 0; i < size.x; i++) {
			validMap[i] = new bool[size.y];
			if (m_selectedVoxelOffset->y <= 0) {
				for (Sint32 j = 0; j < size.y; j++)
					validMap[i][j] =	voxelData[j + 1][m_selectedVoxelOffset->y + 1][i + 1] == 0;
			} else {
				for (Sint32 j = 0; j < size.y; j++)
					validMap[i][j] = (	voxelData[j + 1][m_selectedVoxelOffset->y + 1][i + 1] == 0 &&
										voxelData[j + 1][m_selectedVoxelOffset->y	 ][i + 1] != 0);
			}
		}
		fillVoxels = append(size, validMap, start);
		for (glm::ivec2 c : fillVoxels) {
			m_fillVoxels.push_back(FillCoord(c.y, m_selectedVoxelOffset->y, c.x));
		}
		break;
	case FACE_WEST:
		size = { msize.x, msize.y };
		start = { m_selectedVoxelOffset->x, m_selectedVoxelOffset->y };
		validMap = new bool* [size.x];
		for (Sint32 i = 0; i < size.x; i++) {
			validMap[i] = new bool[size.y];
			if (m_selectedVoxelOffset->z >= msize.z - 1) {
				for (Sint32 j = 0; j < size.y; j++)
					validMap[i][j] =	voxelData[i + 1][j + 1][m_selectedVoxelOffset->z + 1] == 0;
			} else {
				for (Sint32 j = 0; j < size.y; j++)
					validMap[i][j] = (	voxelData[i + 1][j + 1][m_selectedVoxelOffset->z + 1] == 0 &&
										voxelData[i + 1][j + 1][m_selectedVoxelOffset->z + 2] != 0);
			}
		}
		fillVoxels = append(size, validMap, start);
		for (glm::ivec2 c : fillVoxels) {
			m_fillVoxels.push_back(FillCoord(c.x, c.y, m_selectedVoxelOffset->z));
		}
		break;
	case FACE_EAST:
		size = { msize.x, msize.y };
		start = { m_selectedVoxelOffset->x, m_selectedVoxelOffset->y };
		validMap = new bool* [size.x];
		for (Sint32 i = 0; i < size.x; i++) {
			validMap[i] = new bool[size.y];
			if (m_selectedVoxelOffset->z <= 0) {
				for (Sint32 j = 0; j < size.y; j++)
					validMap[i][j] =	voxelData[i + 1][j + 1][m_selectedVoxelOffset->z + 1] == 0;
			} else {
				for (Sint32 j = 0; j < size.y; j++)
					validMap[i][j] = (	voxelData[i + 1][j + 1][m_selectedVoxelOffset->z + 1] == 0 &&
										voxelData[i + 1][j + 1][m_selectedVoxelOffset->z	] != 0);
			}
		}
		fillVoxels = append(size, validMap, start);
		for (glm::ivec2 c : fillVoxels) {
			m_fillVoxels.push_back(FillCoord(c.x, c.y, m_selectedVoxelOffset->z));
		}
		break;
	default: return false;
	}

	//m_fillVoxels.push_back({ m_selectedVoxel->x, m_selectedVoxel->y, m_selectedVoxel->z });

	for (Sint32 i = 0; i < size.x; i++) {
		delete[] validMap[i];
	}
	delete[] validMap;

	return true;
}

void VoxelTool::FillArea::checkPoint(const glm::ivec3& size, bool*** checkMap, Uint16*** voxelData, std::queue<FillCoord>& checkQueue, Uint16 baseVoxel, const FillCoord& coord) {
	if (coord.x < 0 || coord.x >= size.x ||
		coord.y < 0 || coord.y >= size.y ||
		coord.z < 0 || coord.z >= size.z) return;
	if (!checkMap[coord.x][coord.y][coord.z]) {
		checkMap[coord.x][coord.y][coord.z] = true;
		if (voxelData[coord.x + 1][coord.y + 1][coord.z + 1] == baseVoxel) {
			checkQueue.push(coord);
			m_fillVoxels.push_back(coord);
		}
	}
}
bool VoxelTool::FillArea::insert() {
	Matrix* matrix;
	if (m_editMatrix->getId() == -1
		|| !(matrix = m_editMatrix->getMatrix())->containsPoint(*m_selectedVoxel)) {
		m_fillVoxels.clear();
		return false;
	}
	//if ((m_fillStart == *m_selectedVoxel || std::find(m_fillVoxels.begin(), m_fillVoxels.end(), *m_selectedVoxel) != m_fillVoxels.end())
	//	&& m_editMatrix->getId() == m_fillMatrix) return false;
	if (matrix->getVoxel(*m_selectedVoxel).interactionType == 0) return false;
	m_fillVoxels.clear();
	std::queue<FillCoord> checkQueue;
	glm::ivec3 size = matrix->getSize();
	Uint16*** voxelData = matrix->getVoxelData();
	glm::ivec3 start = *m_selectedVoxel + 1;
	Uint16 baseVoxel = voxelData[start.x][start.y][start.z];
	bool*** checkMap = new bool** [size.x];
	for (Sint32 x = 0; x < size.x; x++) {
		checkMap[x] = new bool* [size.y];
		for (Sint32 y = 0; y < size.y; y++) {
			checkMap[x][y] = new bool[size.z];
			ZeroMemory(checkMap[x][y], size.z);
		}
	}

	checkMap[m_selectedVoxel->x][m_selectedVoxel->y][m_selectedVoxel->z] = true;
	checkQueue.push({ m_selectedVoxel->x, m_selectedVoxel->y, m_selectedVoxel->z });
	m_fillVoxels.push_back({ m_selectedVoxel->x, m_selectedVoxel->y, m_selectedVoxel->z });

	GLfloat startTime = glfwGetTime();

	FillCoord fc(0, 0, 0);
	while (!checkQueue.empty()) {
		fc = checkQueue.front();
		checkPoint(size, checkMap, voxelData, checkQueue, baseVoxel, { fc.x + 1,	fc.y,		fc.z });
		checkPoint(size, checkMap, voxelData, checkQueue, baseVoxel, { fc.x - 1,	fc.y,		fc.z });
		checkPoint(size, checkMap, voxelData, checkQueue, baseVoxel, { fc.x,		fc.y - 1,	fc.z });
		checkPoint(size, checkMap, voxelData, checkQueue, baseVoxel, { fc.x,		fc.y + 1,	fc.z });
		checkPoint(size, checkMap, voxelData, checkQueue, baseVoxel, { fc.x,		fc.y,		fc.z - 1 });
		checkPoint(size, checkMap, voxelData, checkQueue, baseVoxel, { fc.x,		fc.y,		fc.z + 1 });
		checkQueue.pop();
	}

	for (Sint32 x = 0; x < size.x; x++) {
		for (Sint32 y = 0; y < size.y; y++) {
			delete[] checkMap[x][y];
		}
		delete[] checkMap[x];
	}
	delete[] checkMap;

	//Logger::logQuiet("Inserted voxels in %d seconds.", glfwGetTime() - startTime);

	return true;
}
void increment(std::map<VoxelTool::Coord, Sint8>& p_outline, glm::ivec3 p_coord, Sint8 p_amt) {
	if (p_outline.find(VoxelTool::Coord(p_coord + glm::ivec3(0, 0, 0))) == p_outline.end()) p_outline.insert({ VoxelTool::Coord(p_coord + glm::ivec3(0, 0, 0)), p_amt });
	else p_outline.at(VoxelTool::Coord(p_coord + glm::ivec3(0, 0, 0))) += p_amt;
}
void VoxelTool::FillArea::createMesh() {
	//m_fillMesh.clear();
	//std::map<Coord, Sint8> outlineX, outlineY, outlineZ;

	//for (glm::ivec3 vox : m_fillVoxels) {
	//	// Coordinates are out of order so concurrent lines are easier to find
	//	increment(outlineX, glm::ivec3(0 + vox.y, 0 + vox.z, 0 + vox.x), 1);
	//	increment(outlineX, glm::ivec3(1 + vox.y, 0 + vox.z, 0 + vox.x), 2);
	//	increment(outlineX, glm::ivec3(1 + vox.y, 1 + vox.z, 0 + vox.x), 4);
	//	increment(outlineX, glm::ivec3(0 + vox.y, 1 + vox.z, 0 + vox.x), 8);

	//	increment(outlineY, glm::ivec3(0 + vox.z, 0 + vox.x, 0 + vox.y), 1);
	//	increment(outlineY, glm::ivec3(1 + vox.z, 0 + vox.x, 0 + vox.y), 2);
	//	increment(outlineY, glm::ivec3(1 + vox.z, 1 + vox.x, 0 + vox.y), 4);
	//	increment(outlineY, glm::ivec3(0 + vox.z, 1 + vox.x, 0 + vox.y), 8);

	//	increment(outlineZ, glm::ivec3(0 + vox.x, 0 + vox.y, 0 + vox.z), 1);
	//	increment(outlineZ, glm::ivec3(1 + vox.x, 0 + vox.y, 0 + vox.z), 2);
	//	increment(outlineZ, glm::ivec3(1 + vox.x, 1 + vox.y, 0 + vox.z), 4);
	//	increment(outlineZ, glm::ivec3(0 + vox.x, 1 + vox.y, 0 + vox.z), 8);
	//}

	//// Create fill mesh
	//// X
	//std::pair<Coord, Sint8> lastPos = std::pair<Coord, Sint8>(Coord(glm::ivec3(-1)), 0);
	//for (std::pair<Coord, Sint8> c : outlineX) {
	//	if (c.first.vec == lastPos.first.vec && c.second == lastPos.second) {
	//		m_fillMesh.back().x++;
	//	} else {
	//		switch (c.second) {
	//		case 1:
	//			m_fillMesh.push_back(glm::vec3(c.first.vec.z, c.first.vec.x, c.first.vec.y));
	//			m_fillMesh.push_back(glm::vec3(c.first.vec.z + 1.0f, c.first.vec.x, c.first.vec.y));
	//			break;
	//		case 2:
	//			m_fillMesh.push_back(glm::vec3(c.first.vec.z, c.first.vec.x, c.first.vec.y));
	//			m_fillMesh.push_back(glm::vec3(c.first.vec.z + 1.0f, c.first.vec.x, c.first.vec.y));
	//			break;
	//		case 4:
	//			m_fillMesh.push_back(glm::vec3(c.first.vec.z, c.first.vec.x, c.first.vec.y));
	//			m_fillMesh.push_back(glm::vec3(c.first.vec.z + 1.0f, c.first.vec.x, c.first.vec.y));
	//			break;
	//		case 8:
	//			m_fillMesh.push_back(glm::vec3(c.first.vec.z, c.first.vec.x, c.first.vec.y));
	//			m_fillMesh.push_back(glm::vec3(c.first.vec.z + 1.0f, c.first.vec.x, c.first.vec.y));
	//			break;
	//		default: break;
	//		}
	//	}
	//	if (c.second == 1 || c.second == 2 || c.second == 4 || c.second == 8)
	//		lastPos = std::pair<Coord, Sint8>(glm::vec3(c.first.vec.x, c.first.vec.y, c.first.vec.z + 1), c.second);
	//	else
	//		lastPos = std::pair<Coord, Sint8>(glm::vec3(-1), 0);
	//}
	//// Y
	//lastPos = std::pair<Coord, Sint8>(Coord(glm::ivec3(-1)), 0);
	//for (std::pair<Coord, Sint8> c : outlineY) {
	//	if (c.first.vec == lastPos.first.vec && c.second == lastPos.second) {
	//		m_fillMesh.back().y++;
	//	} else {
	//		switch (c.second) {
	//		case 1:
	//			m_fillMesh.push_back(glm::vec3(c.first.vec.y, c.first.vec.z, c.first.vec.x));
	//			m_fillMesh.push_back(glm::vec3(c.first.vec.y, c.first.vec.z + 1.0f, c.first.vec.x));
	//			break;
	//		case 2:
	//			m_fillMesh.push_back(glm::vec3(c.first.vec.y, c.first.vec.z, c.first.vec.x));
	//			m_fillMesh.push_back(glm::vec3(c.first.vec.y, c.first.vec.z + 1.0f, c.first.vec.x));
	//			break;
	//		case 4:
	//			m_fillMesh.push_back(glm::vec3(c.first.vec.y, c.first.vec.z, c.first.vec.x));
	//			m_fillMesh.push_back(glm::vec3(c.first.vec.y, c.first.vec.z + 1.0f, c.first.vec.x));
	//			break;
	//		case 8:
	//			m_fillMesh.push_back(glm::vec3(c.first.vec.y, c.first.vec.z, c.first.vec.x));
	//			m_fillMesh.push_back(glm::vec3(c.first.vec.y, c.first.vec.z + 1.0f, c.first.vec.x));
	//			break;
	//		default: break;
	//		}
	//	}
	//	if (c.second == 1 || c.second == 2 || c.second == 4 || c.second == 8)
	//		lastPos = std::pair<Coord, Sint8>(glm::vec3(c.first.vec.x, c.first.vec.y, c.first.vec.z + 1), c.second);
	//	else
	//		lastPos = std::pair<Coord, Sint8>(glm::vec3(-1), 0);
	//}
	//// Z
	//lastPos = std::pair<Coord, Sint8>(Coord(glm::ivec3(-1)), 0);
	//for (std::pair<Coord, Sint8> c : outlineZ) {
	//	if (c.first.vec == lastPos.first.vec && c.second == lastPos.second) {
	//		m_fillMesh.back().z++;
	//	} else {
	//		switch (c.second) {
	//		case 1:
	//			m_fillMesh.push_back(glm::vec3(c.first.vec.x, c.first.vec.y, c.first.vec.z));
	//			m_fillMesh.push_back(glm::vec3(c.first.vec.x, c.first.vec.y, c.first.vec.z + 1.0f));
	//			break;
	//		case 2:
	//			m_fillMesh.push_back(glm::vec3(c.first.vec.x, c.first.vec.y, c.first.vec.z));
	//			m_fillMesh.push_back(glm::vec3(c.first.vec.x, c.first.vec.y, c.first.vec.z + 1.0f));
	//			break;
	//		case 4:
	//			m_fillMesh.push_back(glm::vec3(c.first.vec.x, c.first.vec.y, c.first.vec.z));
	//			m_fillMesh.push_back(glm::vec3(c.first.vec.x, c.first.vec.y, c.first.vec.z + 1.0f));
	//			break;
	//		case 8:
	//			m_fillMesh.push_back(glm::vec3(c.first.vec.x, c.first.vec.y, c.first.vec.z));
	//			m_fillMesh.push_back(glm::vec3(c.first.vec.x, c.first.vec.y, c.first.vec.z + 1.0f));
	//			break;
	//		default: break;
	//		}
	//	}
	//	if (c.second == 1 || c.second == 2 || c.second == 4 || c.second == 8)
	//		lastPos = std::pair<Coord, Sint8>(glm::vec3(c.first.vec.x, c.first.vec.y, c.first.vec.z + 1), c.second);
	//	else
	//		lastPos = std::pair<Coord, Sint8>(glm::vec3(-1), 0);
	//}
}
void VoxelTool::FillArea::use(Voxel p_fill) {
	Matrix* matrix = m_editMatrix->getMatrix();
	for (FillCoord fc : m_fillVoxels) {
		matrix->setVoxel({ fc.x, fc.y, fc.z }, p_fill);
	}
	m_fillVoxels.clear();
	m_fillMesh.clear();
	m_editMatrix->reset();
}

VoxelTool::VoxelTool() {
	m_fillArea = new FillArea();
	m_toolType = ToolType::VOXEL;
	m_voxelMode = VoxelMode::SINGLE;
}
VoxelTool::~VoxelTool() {
	delete m_fillArea;
}

void VoxelTool::init(glm::ivec3* p_voxel, glm::ivec3* p_voxelOffset, Sint8* p_selectedSide, Color* p_color) {
	m_selectedVoxel = p_voxel;
	m_selectedVoxelOffset = p_voxelOffset;
	m_selectedSide = p_selectedSide;
	m_color = p_color;
}

void VoxelTool::renderSingleMesh(bool p_inset) {
	glm::ivec3 selected;
	if (p_inset) {
		selected = *m_selectedVoxelOffset;
		if (selected == glm::ivec3(-1)) return;
	} else {
		selected = *m_selectedVoxel;
	}
	if (!m_editMatrix->getMatrix()->containsPoint(selected)) return;

	glBindTexture(GL_TEXTURE_2D, 0); // TODO Is this necessary?
	GLfloat c = p_inset ? -SELECT_CORRECTION : SELECT_CORRECTION;
	glm::vec3 matPos = m_editMatrix->getPos();
	glm::vec3 pos = matPos + glm::vec3(selected) - glm::vec3(c);
	glm::vec3 size = glm::vec3(1.f + c * 2);

	Shader::setColor(m_editMatrix->getMatrix()->containsPoint(selected) ? m_colorSelect : m_colorError);
	MMesh::renderBoxSolid(pos, size, glm::vec4(1, 1, 1, 0.25f));
}

void VoxelTool::renderBoxMesh(bool p_insetVoxel, bool p_insetBox) {
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
		if (!m_editMatrix->getMatrix()->containsPoint(selected)) return;

		c = p_insetVoxel ? -SELECT_CORRECTION : SELECT_CORRECTION;
		matPos = m_editMatrix->getPos();
		pos = matPos + glm::vec3(selected) - glm::vec3(c);
		size = glm::vec3(1.f + c * 2);

		Shader::setColor(m_editMatrix->getMatrix()->containsPoint(selected) ? m_colorSelect : m_colorError);
		MMesh::renderBoxOutline(pos, size);
	}
}

void VoxelTool::renderFillMesh() {
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

void VoxelTool::box(glm::ivec3 p_pos, Voxel p_voxel) {
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


void VoxelTool::enable() {}
void VoxelTool::disable() {}

void VoxelTool::inputTool() {}
void VoxelTool::updateTool() {}
void VoxelTool::renderTool() {}

void VoxelTool::input() {
	if (m_editMatrix->getId() == -1) return;
	inputTool();
	if (GMouse::mouseReleased(GLFW_MOUSE_BUTTON_LEFT)) {
		m_voxelMode = VoxelMode::SINGLE;
	}
}

void VoxelTool::update() {
	if (m_editMatrix->getId() == -1) return;
	updateTool();
}

void VoxelTool::render() {
	if ((!GMouse::mouseDown(GLFW_MOUSE_BUTTON_LEFT) && GMouse::mouseDown(GLFW_MOUSE_BUTTON_RIGHT)) || m_editMatrix->getId() == -1) return;
	renderTool();
}
