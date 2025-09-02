#include "engine\editor\model\tool\voxel\fill\VFTAdd.h"

VFTAdd::VFTAdd()
	: VoxelTool() {
	m_parent = "Fill";
	m_toolName = "Fill Append Tool";
	m_toolDesc = "Fill Area with Voxels";
	m_toolIcon = MTexture::getTexture("gui\\icon\\toolbar\\FillAdd.png");
	m_keyBind = GKey::KeyBind(GLFW_KEY_F);
}

void VFTAdd::inputTool() {
	if (GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT)) {
		m_fillArea->create(false);
		m_fillArea->use(Voxel(1, *m_color));
	}
}
void VFTAdd::updateTool() {

}
void VFTAdd::renderTool() {

}
