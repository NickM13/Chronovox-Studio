#include "engine\editor\model\tool\voxel\single\VSTErase.h"

VSTErase::VSTErase()
	: VoxelSingleTool() {
	m_parent = "Erase";
	m_toolName = "Voxel Erase Tool";
	m_toolDesc = "Erase a Single Voxel - E";
	m_toolIcon = MTexture::getTexture("gui\\icon\\tool\\SingleErase.png");
	m_keyBind = GKey::KeyBind(GLFW_KEY_E);
}

void VSTErase::inputTool() {
	if (GMouse::mouseDown(GLFW_MOUSE_BUTTON_LEFT)) {
		m_editMatrix->getMatrix()->setVoxel(*m_selectedVoxel, Voxel(0, 0));
	}
}
void VSTErase::updateTool() {

}
void VSTErase::renderTool() {
	renderSingleMesh(false);
}
