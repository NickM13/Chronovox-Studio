#include "engine\editor\model\tool\voxel\VoxelTool.h"

Vector3<Sint32>* VoxelTool::m_selectedVoxel, *VoxelTool::m_selectedVoxelOffset;
Sint8* VoxelTool::m_selectedSide;
Color* VoxelTool::m_color;

VoxelTool::VoxelTool() {
	m_toolType = ToolType::VOXEL;
}
VoxelTool::~VoxelTool() {

}

void VoxelTool::init(Vector3<Sint32>* p_voxel, Vector3<Sint32>* p_voxelOffset, Sint8* p_selectedSide, Color* p_color) {
	m_selectedVoxel = p_voxel;
	m_selectedVoxelOffset = p_voxelOffset;
	m_selectedSide = p_selectedSide;
	m_color = p_color;
}

void VoxelTool::enable() {
	m_boxing = false;
	m_fillMatrix = -1;
	m_fillVoxels.clear();
}
void VoxelTool::disable() {

}

void VoxelTool::box(Vector3<Sint32> p_pos, Voxel p_voxel) {
	if(!m_boxing && !m_editMatrix->getMatrix()->containsPoint(p_pos)) return;
	Vector3<Sint32> s = m_editMatrix->getMatrix()->getSize();
	p_pos = Vector3<Sint32>(max(0, min(p_pos.x, s.x - 1)), max(0, min(p_pos.y, s.y - 1)), max(0, min(p_pos.z, s.z - 1)));

	Vector3<Sint32> start;
	Vector3<Sint32> end;

	start = Vector3<Sint32>(max(0, min(m_boxStart.x, m_boxEnd.x)), max(0, min(m_boxStart.y, m_boxEnd.y)), max(0, min(m_boxStart.z, m_boxEnd.z)));
	end = Vector3<Sint32>(min(s.x - 1, max(m_boxStart.x, m_boxEnd.x)), min(s.y - 1, max(m_boxStart.y, m_boxEnd.y)), min(s.z - 1, max(m_boxStart.z, m_boxEnd.z)));

	for(Sint32 x = start.x; x <= end.x; x++)
		for(Sint32 y = start.y; y <= end.y; y++)
			for(Sint32 z = start.z; z <= end.z; z++)
				m_editMatrix->getMatrix()->setVoxel({x, y, z}, m_editMatrix->getInitMatrix()->getVoxel(Vector3<Sint32>(x, y, z)));

	start = Vector3<Sint32>(min(m_boxStart.x, p_pos.x), min(m_boxStart.y, p_pos.y), min(m_boxStart.z, p_pos.z));
	end = Vector3<Sint32>(max(m_boxStart.x, p_pos.x), max(m_boxStart.y, p_pos.y), max(m_boxStart.z, p_pos.z));

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

void VoxelTool::vectorAdd(std::vector<Vector3<Sint32>> &list1, std::vector<Vector3<Sint32>> list2, Vector3<Sint32> element) {
	if(std::find(list1.begin(), list1.end(), element) == list1.end()
		&& std::find(list2.begin(), list2.end(), element) == list2.end())
		list1.push_back(element);
}
void VoxelTool::fillAppend() {
	Matrix* matrix = 0;
	if(m_editMatrix->getId() == -1
		|| !(matrix = m_editMatrix->getMatrix())->containsPoint(*m_selectedVoxelOffset)) {
		m_fillVoxels.clear();
		return;
	}
	else if(std::find(m_fillVoxels.begin(), m_fillVoxels.end(), *m_selectedVoxelOffset) != m_fillVoxels.end()
		&& *m_selectedSide == m_fillSide
		&& m_editMatrix->getId() == m_fillMatrix) return;

	m_fillVoxels.clear();
	Vector3<Sint32> _size = matrix->getSize();
	Voxel baseVoxel = matrix->getVoxel(*m_selectedVoxelOffset);
	std::vector<Vector3<Sint32>> check;
	check.push_back(*m_selectedVoxelOffset);

	switch(*m_selectedSide) {
	case FACE_SOUTH:
		while(check.size() > 0) {
			if(check[0].y > 0 && baseVoxel == matrix->getVoxel(check[0] + Vector3<Sint32>(0, -1, 0)) && (check[0].x == _size.x - 1 || matrix->getVoxel(check[0] + Vector3<Sint32>(1, -1, 0)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + Vector3<Sint32>(0, -1, 0));
			if(check[0].y < _size.y - 1 && baseVoxel == matrix->getVoxel(check[0] + Vector3<Sint32>(0, 1, 0)) && (check[0].x == _size.x - 1 || matrix->getVoxel(check[0] + Vector3<Sint32>(1, 1, 0)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + Vector3<Sint32>(0, 1, 0));
			if(check[0].z > 0 && baseVoxel == matrix->getVoxel(check[0] + Vector3<Sint32>(0, 0, -1)) && (check[0].x == _size.x - 1 || matrix->getVoxel(check[0] + Vector3<Sint32>(1, 0, -1)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + Vector3<Sint32>(0, 0, -1));
			if(check[0].z < _size.z - 1 && baseVoxel == matrix->getVoxel(check[0] + Vector3<Sint32>(0, 0, 1)) && (check[0].x == _size.x - 1 || matrix->getVoxel(check[0] + Vector3<Sint32>(1, 0, 1)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + Vector3<Sint32>(0, 0, 1));
			m_fillVoxels.push_back(check[0]);
			check.erase(check.begin());
		}
		break;
	case FACE_NORTH:
		while(check.size() > 0) {
			if(check[0].y > 0 && baseVoxel == matrix->getVoxel(check[0] + Vector3<Sint32>(0, -1, 0)) && (check[0].x == 0 || matrix->getVoxel(check[0] + Vector3<Sint32>(-1, -1, 0)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + Vector3<Sint32>(0, -1, 0));
			if(check[0].y < _size.y - 1 && baseVoxel == matrix->getVoxel(check[0] + Vector3<Sint32>(0, 1, 0)) && (check[0].x == 0 || matrix->getVoxel(check[0] + Vector3<Sint32>(-1, 1, 0)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + Vector3<Sint32>(0, 1, 0));
			if(check[0].z > 0 && baseVoxel == matrix->getVoxel(check[0] + Vector3<Sint32>(0, 0, -1)) && (check[0].x == 0 || matrix->getVoxel(check[0] + Vector3<Sint32>(-1, 0, -1)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + Vector3<Sint32>(0, 0, -1));
			if(check[0].z < _size.z - 1 && baseVoxel == matrix->getVoxel(check[0] + Vector3<Sint32>(0, 0, 1)) && (check[0].x == 0 || matrix->getVoxel(check[0] + Vector3<Sint32>(-1, 0, 1)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + Vector3<Sint32>(0, 0, 1));
			m_fillVoxels.push_back(check[0]);
			check.erase(check.begin());
		}
		break;
	case FACE_BOTTOM:
		while(check.size() > 0) {
			if(check[0].x > 0 && baseVoxel == matrix->getVoxel(check[0] + Vector3<Sint32>(-1, 0, 0)) && (check[0].y == _size.y - 1 || matrix->getVoxel(check[0] + Vector3<Sint32>(-1, 1, 0)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + Vector3<Sint32>(-1, 0, 0));
			if(check[0].x < _size.x - 1 && baseVoxel == matrix->getVoxel(check[0] + Vector3<Sint32>(1, 0, 0)) && (check[0].y == _size.y - 1 || matrix->getVoxel(check[0] + Vector3<Sint32>(1, 1, 0)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + Vector3<Sint32>(1, 0, 0));
			if(check[0].z > 0 && baseVoxel == matrix->getVoxel(check[0] + Vector3<Sint32>(0, 0, -1)) && (check[0].y == _size.y - 1 || matrix->getVoxel(check[0] + Vector3<Sint32>(0, 1, -1)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + Vector3<Sint32>(0, 0, -1));
			if(check[0].z < _size.z - 1 && baseVoxel == matrix->getVoxel(check[0] + Vector3<Sint32>(0, 0, 1)) && (check[0].y == _size.y - 1 || matrix->getVoxel(check[0] + Vector3<Sint32>(0, 1, 1)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + Vector3<Sint32>(0, 0, 1));
			m_fillVoxels.push_back(check[0]);
			check.erase(check.begin());
		}
		break;
	case FACE_TOP:
		while(check.size() > 0) {
			if(check[0].x > 0 && baseVoxel == matrix->getVoxel(check[0] + Vector3<Sint32>(-1, 0, 0)) && (check[0].y == 0 || matrix->getVoxel(check[0] + Vector3<Sint32>(-1, -1, 0)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + Vector3<Sint32>(-1, 0, 0));
			if(check[0].x < _size.x - 1 && baseVoxel == matrix->getVoxel(check[0] + Vector3<Sint32>(1, 0, 0)) && (check[0].y == 0 || matrix->getVoxel(check[0] + Vector3<Sint32>(1, -1, 0)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + Vector3<Sint32>(1, 0, 0));
			if(check[0].z > 0 && baseVoxel == matrix->getVoxel(check[0] + Vector3<Sint32>(0, 0, -1)) && (check[0].y == 0 || matrix->getVoxel(check[0] + Vector3<Sint32>(0, -1, -1)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + Vector3<Sint32>(0, 0, -1));
			if(check[0].z < _size.z - 1 && baseVoxel == matrix->getVoxel(check[0] + Vector3<Sint32>(0, 0, 1)) && (check[0].y == 0 || matrix->getVoxel(check[0] + Vector3<Sint32>(0, -1, 1)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + Vector3<Sint32>(0, 0, 1));
			m_fillVoxels.push_back(check[0]);
			check.erase(check.begin());
		}
		break;
	case FACE_WEST:
		while(check.size() > 0) {
			if(check[0].x > 0 && baseVoxel == matrix->getVoxel(check[0] + Vector3<Sint32>(-1, 0, 0)) && (check[0].z == _size.z - 1 || matrix->getVoxel(check[0] + Vector3<Sint32>(-1, 0, 1)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + Vector3<Sint32>(-1, 0, 0));
			if(check[0].x < _size.x - 1 && baseVoxel == matrix->getVoxel(check[0] + Vector3<Sint32>(1, 0, 0)) && (check[0].z == _size.z - 1 || matrix->getVoxel(check[0] + Vector3<Sint32>(1, 0, 1)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + Vector3<Sint32>(1, 0, 0));
			if(check[0].y > 0 && baseVoxel == matrix->getVoxel(check[0] + Vector3<Sint32>(0, -1, 0)) && (check[0].z == _size.z - 1 || matrix->getVoxel(check[0] + Vector3<Sint32>(0, -1, 1)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + Vector3<Sint32>(0, -1, 0));
			if(check[0].y < _size.y - 1 && baseVoxel == matrix->getVoxel(check[0] + Vector3<Sint32>(0, 1, 0)) && (check[0].z == _size.z - 1 || matrix->getVoxel(check[0] + Vector3<Sint32>(0, 1, 1)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + Vector3<Sint32>(0, 1, 0));
			m_fillVoxels.push_back(check[0]);
			check.erase(check.begin());
		}
		break;
	case FACE_EAST:
		while(check.size() > 0) {
			if(check[0].x > 0 && baseVoxel == matrix->getVoxel(check[0] + Vector3<Sint32>(-1, 0, 0)) && (check[0].z == 0 || matrix->getVoxel(check[0] + Vector3<Sint32>(-1, 0, -1)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + Vector3<Sint32>(-1, 0, 0));
			if(check[0].x < _size.x - 1 && baseVoxel == matrix->getVoxel(check[0] + Vector3<Sint32>(1, 0, 0)) && (check[0].z == 0 || matrix->getVoxel(check[0] + Vector3<Sint32>(1, 0, -1)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + Vector3<Sint32>(1, 0, 0));
			if(check[0].y > 0 && baseVoxel == matrix->getVoxel(check[0] + Vector3<Sint32>(0, -1, 0)) && (check[0].z == 0 || matrix->getVoxel(check[0] + Vector3<Sint32>(0, -1, -1)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + Vector3<Sint32>(0, -1, 0));
			if(check[0].y < _size.y - 1 && baseVoxel == matrix->getVoxel(check[0] + Vector3<Sint32>(0, 1, 0)) && (check[0].z == 0 || matrix->getVoxel(check[0] + Vector3<Sint32>(0, 1, -1)).interactionType == 1)) vectorAdd(check, m_fillVoxels, check[0] + Vector3<Sint32>(0, 1, 0));
			m_fillVoxels.push_back(check[0]);
			check.erase(check.begin());
		}
		break;
	}
	m_fillMatrix = matrix->getId();
	m_fillStart = *m_selectedVoxelOffset;
	m_fillSide = *m_selectedSide;
}
void VoxelTool::fillInsert() {
	Matrix* matrix;
	if(m_editMatrix->getId() == -1
		|| !(matrix = m_editMatrix->getMatrix())->containsPoint(*m_selectedVoxel)) {
		m_fillVoxels.clear();
		return;
	}
	else if((m_fillStart == *m_selectedVoxel || std::find(m_fillVoxels.begin(), m_fillVoxels.end(), *m_selectedVoxel) != m_fillVoxels.end())
		&& m_editMatrix->getId() == m_fillMatrix) return;
	m_fillVoxels.clear();
	Vector3<Sint32> _size = matrix->getSize();
	Voxel baseVoxel = matrix->getVoxel(*m_selectedVoxel);
	if(baseVoxel.interactionType == 0) return;
	std::vector<Vector3<Sint32>> check;
	check.push_back(*m_selectedVoxel);
	m_fillVoxels.clear();

	GLfloat startTime = glfwGetTime();

	while(check.size() > 0) {
		if(check[0].x > 0 && baseVoxel == matrix->getVoxel(check[0] + Vector3<Sint32>(-1, 0, 0))) vectorAdd(check, m_fillVoxels, check[0] + Vector3<Sint32>(-1, 0, 0));
		if(check[0].x < _size.x - 1 && baseVoxel == matrix->getVoxel(check[0] + Vector3<Sint32>(1, 0, 0))) vectorAdd(check, m_fillVoxels, check[0] + Vector3<Sint32>(1, 0, 0));
		if(check[0].y > 0 && baseVoxel == matrix->getVoxel(check[0] + Vector3<Sint32>(0, -1, 0))) vectorAdd(check, m_fillVoxels, check[0] + Vector3<Sint32>(0, -1, 0));
		if(check[0].y < _size.y - 1 && baseVoxel == matrix->getVoxel(check[0] + Vector3<Sint32>(0, 1, 0))) vectorAdd(check, m_fillVoxels, check[0] + Vector3<Sint32>(0, 1, 0));
		if(check[0].z > 0 && baseVoxel == matrix->getVoxel(check[0] + Vector3<Sint32>(0, 0, -1))) vectorAdd(check, m_fillVoxels, check[0] + Vector3<Sint32>(0, 0, -1));
		if(check[0].z < _size.z - 1 && baseVoxel == matrix->getVoxel(check[0] + Vector3<Sint32>(0, 0, 1))) vectorAdd(check, m_fillVoxels, check[0] + Vector3<Sint32>(0, 0, 1));
		m_fillVoxels.push_back(check[0]);
		check.erase(check.begin());
	}

	m_fillMatrix = matrix->getId();
	m_fillStart = *m_selectedVoxel;
}
void VoxelTool::fill(Voxel p_fill) {
	Matrix* matrix = m_editMatrix->getMatrix();
	for(Vector3<Sint32> v : m_fillVoxels) {
		matrix->setVoxel(v, p_fill);
	}
	m_fillVoxels.clear();
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
