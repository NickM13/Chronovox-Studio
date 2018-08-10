#include "engine\editor\model\tool\voxel\VoxelTool.h"
#include "engine\gfx\shader\Shader.h"
#include <set>

const GLfloat SELECT_CORRECTION = 0.025f;

glm::ivec3* VoxelTool::m_selectedVoxel, *VoxelTool::m_selectedVoxelOffset;
Sint8* VoxelTool::m_selectedSide;
Color* VoxelTool::m_color;

VoxelTool::VoxelTool() {
	m_toolType = ToolType::VOXEL;
	m_fillArea = new FillArea();
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

void VoxelTool::enable() {
	m_boxing = false;
	m_fillArea->destroy();
}
void VoxelTool::disable() {}

void VoxelTool::box(glm::ivec3 p_pos, Voxel p_voxel) {
	if(!m_boxing && !m_editMatrix->getMatrix()->containsPoint(p_pos)) return;
	glm::ivec3 s = m_editMatrix->getMatrix()->getSize();
	p_pos = glm::ivec3(std::fmaxf(0, std::fminf(p_pos.x, s.x - 1)), std::fmaxf(0, std::fminf(p_pos.y, s.y - 1)), std::fmaxf(0, std::fminf(p_pos.z, s.z - 1)));

	glm::ivec3 start;
	glm::ivec3 end;

	start = glm::ivec3(std::fmaxf(0, std::fminf(m_boxStart.x, m_boxEnd.x)), std::fmaxf(0, std::fminf(m_boxStart.y, m_boxEnd.y)), std::fmaxf(0, std::fminf(m_boxStart.z, m_boxEnd.z)));
	end = glm::ivec3(std::fminf(s.x - 1, std::fmaxf(m_boxStart.x, m_boxEnd.x)), std::fminf(s.y - 1, std::fmaxf(m_boxStart.y, m_boxEnd.y)), std::fminf(s.z - 1, std::fmaxf(m_boxStart.z, m_boxEnd.z)));

	for(Sint32 x = start.x; x <= end.x; x++)
		for(Sint32 y = start.y; y <= end.y; y++)
			for(Sint32 z = start.z; z <= end.z; z++)
				m_editMatrix->getMatrix()->setVoxel({x, y, z}, m_editMatrix->getInitMatrix()->getVoxel(glm::ivec3(x, y, z)));

	start = glm::ivec3(std::fminf(m_boxStart.x, p_pos.x), std::fminf(m_boxStart.y, p_pos.y), std::fminf(m_boxStart.z, p_pos.z));
	end = glm::ivec3(std::fmaxf(m_boxStart.x, p_pos.x), std::fmaxf(m_boxStart.y, p_pos.y), std::fmaxf(m_boxStart.z, p_pos.z));

	if(m_boxReplace) {
		Voxel match = m_editMatrix->getInitMatrix()->getVoxel(m_boxStart);
		for(Sint32 x = start.x; x <= end.x; x++)
			for(Sint32 y = start.y; y <= end.y; y++)
				for(Sint32 z = start.z; z <= end.z; z++)
					if(match == m_editMatrix->getInitMatrix()->getVoxel({x, y, z}))
						m_editMatrix->getMatrix()->setVoxel({x, y, z}, p_voxel);
	}
	else {
		for(Sint32 x = start.x; x <= end.x; x++)
			for(Sint32 y = start.y; y <= end.y; y++)
				for(Sint32 z = start.z; z <= end.z; z++)
					m_editMatrix->getMatrix()->setVoxel({x, y, z}, p_voxel);
	}

	m_boxEnd = p_pos;
}

void VoxelTool::FillArea::create(bool p_inset) {
	if(p_inset) insert();
	else        append();
}
void VoxelTool::FillArea::destroy() {
	m_fillMatrix = -1;
	m_fillVoxels.clear();
}
void VoxelTool::FillArea::vectorAdd(std::vector<glm::ivec3> &list1, std::vector<glm::ivec3> list2, glm::ivec3 element) {
	if(std::find(list1.begin(), list1.end(), element) == list1.end()
		&& std::find(list2.begin(), list2.end(), element) == list2.end())
		list1.push_back(element);
}
bool VoxelTool::FillArea::append() {
	Matrix* matrix = 0;
	if(m_editMatrix->getId() == -1
		|| !(matrix = m_editMatrix->getMatrix())->containsPoint(*m_selectedVoxelOffset)) {
		m_fillVoxels.clear();
		return false;
	}
	else if(std::find(m_fillVoxels.begin(), m_fillVoxels.end(), *m_selectedVoxelOffset) != m_fillVoxels.end()
		&& *m_selectedSide == m_fillSide
		&& m_editMatrix->getId() == m_fillMatrix) return false;

	m_fillVoxels.clear();
	glm::ivec3 _size = matrix->getSize();
	Voxel baseVoxel = matrix->getVoxel(*m_selectedVoxelOffset);
	std::vector<glm::ivec3> check;
	check.push_back(*m_selectedVoxelOffset);

	switch(*m_selectedSide) {
	case FACE_SOUTH:
		while(check.size() > 0) {
			if(check[0].y > 0 && baseVoxel == matrix->getVoxel(check[0] + glm::ivec3(0, -1, 0)) && (check[0].x == _size.x - 1 || matrix->getVoxel(check[0] + glm::ivec3(1, -1, 0)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + glm::ivec3(0, -1, 0));
			if(check[0].y < _size.y - 1 && baseVoxel == matrix->getVoxel(check[0] + glm::ivec3(0, 1, 0)) && (check[0].x == _size.x - 1 || matrix->getVoxel(check[0] + glm::ivec3(1, 1, 0)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + glm::ivec3(0, 1, 0));
			if(check[0].z > 0 && baseVoxel == matrix->getVoxel(check[0] + glm::ivec3(0, 0, -1)) && (check[0].x == _size.x - 1 || matrix->getVoxel(check[0] + glm::ivec3(1, 0, -1)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + glm::ivec3(0, 0, -1));
			if(check[0].z < _size.z - 1 && baseVoxel == matrix->getVoxel(check[0] + glm::ivec3(0, 0, 1)) && (check[0].x == _size.x - 1 || matrix->getVoxel(check[0] + glm::ivec3(1, 0, 1)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + glm::ivec3(0, 0, 1));
			m_fillVoxels.push_back(check[0]);
			check.erase(check.begin());
		}
		break;
	case FACE_NORTH:
		while(check.size() > 0) {
			if(check[0].y > 0 && baseVoxel == matrix->getVoxel(check[0] + glm::ivec3(0, -1, 0)) && (check[0].x == 0 || matrix->getVoxel(check[0] + glm::ivec3(-1, -1, 0)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + glm::ivec3(0, -1, 0));
			if(check[0].y < _size.y - 1 && baseVoxel == matrix->getVoxel(check[0] + glm::ivec3(0, 1, 0)) && (check[0].x == 0 || matrix->getVoxel(check[0] + glm::ivec3(-1, 1, 0)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + glm::ivec3(0, 1, 0));
			if(check[0].z > 0 && baseVoxel == matrix->getVoxel(check[0] + glm::ivec3(0, 0, -1)) && (check[0].x == 0 || matrix->getVoxel(check[0] + glm::ivec3(-1, 0, -1)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + glm::ivec3(0, 0, -1));
			if(check[0].z < _size.z - 1 && baseVoxel == matrix->getVoxel(check[0] + glm::ivec3(0, 0, 1)) && (check[0].x == 0 || matrix->getVoxel(check[0] + glm::ivec3(-1, 0, 1)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + glm::ivec3(0, 0, 1));
			m_fillVoxels.push_back(check[0]);
			check.erase(check.begin());
		}
		break;
	case FACE_BOTTOM:
		while(check.size() > 0) {
			if(check[0].x > 0 && baseVoxel == matrix->getVoxel(check[0] + glm::ivec3(-1, 0, 0)) && (check[0].y == _size.y - 1 || matrix->getVoxel(check[0] + glm::ivec3(-1, 1, 0)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + glm::ivec3(-1, 0, 0));
			if(check[0].x < _size.x - 1 && baseVoxel == matrix->getVoxel(check[0] + glm::ivec3(1, 0, 0)) && (check[0].y == _size.y - 1 || matrix->getVoxel(check[0] + glm::ivec3(1, 1, 0)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + glm::ivec3(1, 0, 0));
			if(check[0].z > 0 && baseVoxel == matrix->getVoxel(check[0] + glm::ivec3(0, 0, -1)) && (check[0].y == _size.y - 1 || matrix->getVoxel(check[0] + glm::ivec3(0, 1, -1)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + glm::ivec3(0, 0, -1));
			if(check[0].z < _size.z - 1 && baseVoxel == matrix->getVoxel(check[0] + glm::ivec3(0, 0, 1)) && (check[0].y == _size.y - 1 || matrix->getVoxel(check[0] + glm::ivec3(0, 1, 1)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + glm::ivec3(0, 0, 1));
			m_fillVoxels.push_back(check[0]);
			check.erase(check.begin());
		}
		break;
	case FACE_TOP:
		while(check.size() > 0) {
			if(check[0].x > 0 && baseVoxel == matrix->getVoxel(check[0] + glm::ivec3(-1, 0, 0)) && (check[0].y == 0 || matrix->getVoxel(check[0] + glm::ivec3(-1, -1, 0)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + glm::ivec3(-1, 0, 0));
			if(check[0].x < _size.x - 1 && baseVoxel == matrix->getVoxel(check[0] + glm::ivec3(1, 0, 0)) && (check[0].y == 0 || matrix->getVoxel(check[0] + glm::ivec3(1, -1, 0)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + glm::ivec3(1, 0, 0));
			if(check[0].z > 0 && baseVoxel == matrix->getVoxel(check[0] + glm::ivec3(0, 0, -1)) && (check[0].y == 0 || matrix->getVoxel(check[0] + glm::ivec3(0, -1, -1)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + glm::ivec3(0, 0, -1));
			if(check[0].z < _size.z - 1 && baseVoxel == matrix->getVoxel(check[0] + glm::ivec3(0, 0, 1)) && (check[0].y == 0 || matrix->getVoxel(check[0] + glm::ivec3(0, -1, 1)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + glm::ivec3(0, 0, 1));
			m_fillVoxels.push_back(check[0]);
			check.erase(check.begin());
		}
		break;
	case FACE_WEST:
		while(check.size() > 0) {
			if(check[0].x > 0 && baseVoxel == matrix->getVoxel(check[0] + glm::ivec3(-1, 0, 0)) && (check[0].z == _size.z - 1 || matrix->getVoxel(check[0] + glm::ivec3(-1, 0, 1)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + glm::ivec3(-1, 0, 0));
			if(check[0].x < _size.x - 1 && baseVoxel == matrix->getVoxel(check[0] + glm::ivec3(1, 0, 0)) && (check[0].z == _size.z - 1 || matrix->getVoxel(check[0] + glm::ivec3(1, 0, 1)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + glm::ivec3(1, 0, 0));
			if(check[0].y > 0 && baseVoxel == matrix->getVoxel(check[0] + glm::ivec3(0, -1, 0)) && (check[0].z == _size.z - 1 || matrix->getVoxel(check[0] + glm::ivec3(0, -1, 1)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + glm::ivec3(0, -1, 0));
			if(check[0].y < _size.y - 1 && baseVoxel == matrix->getVoxel(check[0] + glm::ivec3(0, 1, 0)) && (check[0].z == _size.z - 1 || matrix->getVoxel(check[0] + glm::ivec3(0, 1, 1)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + glm::ivec3(0, 1, 0));
			m_fillVoxels.push_back(check[0]);
			check.erase(check.begin());
		}
		break;
	case FACE_EAST:
		while(check.size() > 0) {
			if(check[0].x > 0 && baseVoxel == matrix->getVoxel(check[0] + glm::ivec3(-1, 0, 0)) && (check[0].z == 0 || matrix->getVoxel(check[0] + glm::ivec3(-1, 0, -1)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + glm::ivec3(-1, 0, 0));
			if(check[0].x < _size.x - 1 && baseVoxel == matrix->getVoxel(check[0] + glm::ivec3(1, 0, 0)) && (check[0].z == 0 || matrix->getVoxel(check[0] + glm::ivec3(1, 0, -1)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + glm::ivec3(1, 0, 0));
			if(check[0].y > 0 && baseVoxel == matrix->getVoxel(check[0] + glm::ivec3(0, -1, 0)) && (check[0].z == 0 || matrix->getVoxel(check[0] + glm::ivec3(0, -1, -1)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + glm::ivec3(0, -1, 0));
			if(check[0].y < _size.y - 1 && baseVoxel == matrix->getVoxel(check[0] + glm::ivec3(0, 1, 0)) && (check[0].z == 0 || matrix->getVoxel(check[0] + glm::ivec3(0, 1, -1)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + glm::ivec3(0, 1, 0));
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
bool VoxelTool::FillArea::insert() {
	Matrix* matrix;
	if(m_editMatrix->getId() == -1
		|| !(matrix = m_editMatrix->getMatrix())->containsPoint(*m_selectedVoxel)) {
		m_fillVoxels.clear();
		return false;
	}
	else if((m_fillStart == *m_selectedVoxel || std::find(m_fillVoxels.begin(), m_fillVoxels.end(), *m_selectedVoxel) != m_fillVoxels.end())
		&& m_editMatrix->getId() == m_fillMatrix) return false;
	m_fillVoxels.clear();
	glm::ivec3 _size = matrix->getSize();
	Voxel baseVoxel = matrix->getVoxel(*m_selectedVoxel);
	if(baseVoxel.interactionType == 0) return false;
	std::vector<glm::ivec3> check;
	check.push_back(*m_selectedVoxel);
	m_fillVoxels.clear();

	GLfloat startTime = glfwGetTime();

	while(check.size() > 0) {
		if(check[0].x > 0 && baseVoxel == matrix->getVoxel(check[0] + glm::ivec3(-1, 0, 0)))		  vectorAdd(check, m_fillVoxels, check[0] + glm::ivec3(-1,  0,  0));
		if(check[0].x < _size.x - 1 && baseVoxel == matrix->getVoxel(check[0] + glm::ivec3(1, 0, 0))) vectorAdd(check, m_fillVoxels, check[0] + glm::ivec3( 1,  0,  0));
		if(check[0].y > 0 && baseVoxel == matrix->getVoxel(check[0] + glm::ivec3(0, -1, 0)))		  vectorAdd(check, m_fillVoxels, check[0] + glm::ivec3( 0, -1,  0));
		if(check[0].y < _size.y - 1 && baseVoxel == matrix->getVoxel(check[0] + glm::ivec3(0, 1, 0))) vectorAdd(check, m_fillVoxels, check[0] + glm::ivec3( 0,  1,  0));
		if(check[0].z > 0 && baseVoxel == matrix->getVoxel(check[0] + glm::ivec3(0, 0, -1)))		  vectorAdd(check, m_fillVoxels, check[0] + glm::ivec3( 0,  0, -1));
		if(check[0].z < _size.z - 1 && baseVoxel == matrix->getVoxel(check[0] + glm::ivec3(0, 0, 1))) vectorAdd(check, m_fillVoxels, check[0] + glm::ivec3( 0,  0,  1));
		m_fillVoxels.push_back(check[0]);
		check.erase(check.begin());
	}

	m_fillMatrix = matrix->getId();
	m_fillStart = *m_selectedVoxel;

	createMesh();

	return true;
}
void increment(std::map<VoxelTool::Coord, Sint8>& p_outline, glm::ivec3 p_coord, Sint8 p_amt) {
	if(p_outline.find(VoxelTool::Coord(p_coord + glm::ivec3(0, 0, 0))) == p_outline.end()) p_outline.insert({VoxelTool::Coord(p_coord + glm::ivec3(0, 0, 0)), p_amt});
	else p_outline.at(VoxelTool::Coord(p_coord + glm::ivec3(0, 0, 0))) += p_amt;
}
void VoxelTool::FillArea::createMesh() {
	m_fillMesh.clear();
	std::map<Coord, Sint8> outlineX, outlineY, outlineZ;

	for(glm::ivec3 vox : m_fillVoxels) {
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
	for(std::pair<Coord, Sint8> c : outlineX) {
		if(c.first.vec == lastPos.first.vec && c.second == lastPos.second) {
			m_fillMesh.back().x++;
		}
		else {
			switch(c.second) {
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
		if(c.second == 1 || c.second == 2 || c.second == 4 || c.second == 8)
			lastPos = std::pair<Coord, Sint8>(glm::vec3(c.first.vec.x, c.first.vec.y, c.first.vec.z + 1), c.second);
		else
			lastPos = std::pair<Coord, Sint8>(glm::vec3(-1), 0);
	}
	// Y
	lastPos = std::pair<Coord, Sint8>(Coord(glm::ivec3(-1)), 0);
	for(std::pair<Coord, Sint8> c : outlineY) {
		if(c.first.vec == lastPos.first.vec && c.second == lastPos.second) {
			m_fillMesh.back().y++;
		}
		else {
			switch(c.second) {
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
		if(c.second == 1 || c.second == 2 || c.second == 4 || c.second == 8)
			lastPos = std::pair<Coord, Sint8>(glm::vec3(c.first.vec.x, c.first.vec.y, c.first.vec.z + 1), c.second);
		else
			lastPos = std::pair<Coord, Sint8>(glm::vec3(-1), 0);
	}
	// Z
	lastPos = std::pair<Coord, Sint8>(Coord(glm::ivec3(-1)), 0);
	for(std::pair<Coord, Sint8> c : outlineZ) {
		if(c.first.vec == lastPos.first.vec && c.second == lastPos.second) {
			m_fillMesh.back().z++;
		}
		else {
			switch(c.second) {
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
		if(c.second == 1 || c.second == 2 || c.second == 4 || c.second == 8)
			lastPos = std::pair<Coord, Sint8>(glm::vec3(c.first.vec.x, c.first.vec.y, c.first.vec.z + 1), c.second);
		else
			lastPos = std::pair<Coord, Sint8>(glm::vec3(-1), 0);
	}
}
void VoxelTool::FillArea::use(Voxel p_fill) {
	Matrix* matrix = m_editMatrix->getMatrix();
	for(glm::ivec3 v : m_fillVoxels) {
		matrix->setVoxel(v, p_fill);
	}
	m_fillVoxels.clear();
}

void VoxelTool::renderSingleMesh(bool p_inset) {
	glm::ivec3 selected;
	if(p_inset) {
		selected = *m_selectedVoxelOffset;
		if(selected == glm::ivec3(-1)) return;
	}
	else {
		selected = *m_selectedVoxel;
		if(!m_editMatrix->getMatrix()->containsPoint(selected)) return;
	}

	glBindTexture(GL_TEXTURE_2D, 0); // TODO Is this necessary?
	GLfloat c = p_inset ? -SELECT_CORRECTION : SELECT_CORRECTION;
	glm::vec3 matPos = m_editMatrix->getPos();
	glm::vec3 pos = matPos + glm::vec3(selected) - glm::vec3(c);
	glm::vec3 size = glm::vec3(1.f + c * 2);

	Shader::setColor(m_editMatrix->getMatrix()->containsPoint(selected) ? m_colorSelect : m_colorError);
	MMesh::renderBoxOutline(pos, size);
}
void VoxelTool::renderBoxMesh(bool p_insetVoxel, bool p_insetBox) {
	glm::ivec3 selected;
	if(p_insetVoxel) selected = *m_selectedVoxelOffset;
	else			 selected = *m_selectedVoxel;

	glBindTexture(GL_TEXTURE_2D, 0); // TODO Is this necessary?
	GLfloat c;
	glm::vec3 matPos = m_editMatrix->getPos();
	glm::vec3 pos;
	glm::vec3 size;

	if(m_boxing) {
		c = p_insetBox ? -SELECT_CORRECTION : SELECT_CORRECTION;
		pos = matPos + glm::vec3(glm::min(m_boxStart, m_boxEnd)) - glm::vec3(c);
		size = glm::vec3(glm::abs(m_boxStart - m_boxEnd)) + glm::vec3(c * 2 + 1);

		Shader::setColor(m_colorSelect);
		MMesh::renderBoxOutline(pos, size);
	}
	else {
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
	for(Sint32 i = 0; i < coordList.size(); i += 2) {
		MMesh::renderLine(coordList[i], coordList[i+1]);
	}
	Shader::popMatrixModel();
}

void VoxelTool::inputSingle() {}
void VoxelTool::updateSingle() {}
void VoxelTool::renderSingle() {}

void VoxelTool::inputBox() {}
void VoxelTool::updateBox() {}
void VoxelTool::renderBox() {}

void VoxelTool::inputFill() {}
void VoxelTool::updateFill() {}
void VoxelTool::renderFill() {}

void VoxelTool::input() {
	if(m_editMatrix->getId() == -1) return;
	switch(*m_subTool) {
	case 0: inputSingle(); break;
	case 1: inputBox(); break;
	case 2: inputFill(); break;
	}
}

void VoxelTool::update() {
	if(m_editMatrix->getId() == -1) return;
	switch(*m_subTool) {
	case 0: updateSingle(); break;
	case 1: updateBox(); break;
	case 2: updateFill(); break;
	}
}

void VoxelTool::render() {
	if(GMouse::mouseDown(GLFW_MOUSE_BUTTON_RIGHT) || m_editMatrix->getId() == -1) return;
	switch(*m_subTool) {
	case 0: renderSingle(); break;
	case 1: renderBox(); break;
	case 2: renderFill(); break;
	}
}
