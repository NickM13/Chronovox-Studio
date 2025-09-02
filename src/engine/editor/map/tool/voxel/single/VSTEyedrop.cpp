#include "engine\editor\model\tool\voxel\single\VSTEyedrop.h"

VSTEyedrop::VSTEyedrop()
	: VoxelTool() {
	m_parent = "Eyedropper";
	m_toolName = "Eyedropper Tool";
	m_toolDesc = "Eyedropper - K";
	m_toolIcon = MTexture::getTexture("gui\\icon\\toolbar\\VoxelEyedrop.png");
	m_keyBind = GKey::KeyBind(GLFW_KEY_K);
}

void VSTEyedrop::inputTool() {
	if(GMouse::mouseDown(GLFW_MOUSE_BUTTON_LEFT) && m_editMatrix->getMatrix()->containsPoint(*m_selectedVoxel)) {
		*m_color = m_editMatrix->getMatrix()->getVoxel(*m_selectedVoxel).color;
	}
}
void VSTEyedrop::updateTool() {

}
void VSTEyedrop::renderTool() {
	renderSingleMesh(false);
}
