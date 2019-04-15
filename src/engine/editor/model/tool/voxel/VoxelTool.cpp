#include "engine\editor\model\tool\voxel\VoxelTool.h"
#include <set>

glm::ivec3* VoxelTool::m_selectedVoxel, *VoxelTool::m_selectedVoxelOffset;
Sint8* VoxelTool::m_selectedSide;
Color* VoxelTool::m_color;

VoxelTool::VoxelTool() {
	m_toolType = ToolType::VOXEL;
}
VoxelTool::~VoxelTool() {}

void VoxelTool::init(glm::ivec3* p_voxel, glm::ivec3* p_voxelOffset, Sint8* p_selectedSide, Color* p_color) {
	m_selectedVoxel = p_voxel;
	m_selectedVoxelOffset = p_voxelOffset;
	m_selectedSide = p_selectedSide;
	m_color = p_color;
}

void VoxelTool::enable() {}
void VoxelTool::disable() {}

void VoxelTool::inputTool() {}
void VoxelTool::updateTool() {}
void VoxelTool::renderTool() {}

void VoxelTool::input() {
	if (m_editMatrix->getId() == -1) return;
	inputTool();
}

void VoxelTool::update() {
	if (m_editMatrix->getId() == -1) return;
	updateTool();
}

void VoxelTool::render() {
	if ((!GMouse::mouseDown(GLFW_MOUSE_BUTTON_LEFT) && GMouse::mouseDown(GLFW_MOUSE_BUTTON_RIGHT)) || m_editMatrix->getId() == -1) return;
	renderTool();
}
