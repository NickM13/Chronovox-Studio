#include "engine\editor\model\tool\voxel\single\VSTAdd.h"

VSTAdd::VSTAdd()
	: VoxelSingleTool() {
	m_parent = "Add";
	m_toolName = "Voxel Append Tool";
	m_toolDesc = "Add a Single Voxel (T)";
	m_toolIcon = MTexture::getTexture("gui\\icon\\tool\\SingleAdd.png");
	m_keyBind = GKey::KeyBind(GLFW_KEY_T);
}

void VSTAdd::inputTool() {
	if (GMouse::mouseDown(GLFW_MOUSE_BUTTON_LEFT)) {
		m_editMatrix->getMatrix()->setVoxel(*m_selectedVoxelOffset, Voxel(1, MColor::getInstance().getUnitID(*m_color)));
	}
}
void VSTAdd::updateTool() {

}
void VSTAdd::renderTool() {
	renderSingleMesh(true);
}
