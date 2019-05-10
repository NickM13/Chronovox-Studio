#include "engine\editor\model\tool\voxel\fill\VFTErase.h"

VFTErase::VFTErase()
	: VoxelFillTool() {
	m_parent = "Fill";
	m_toolName = "Fill Erase Tool";
	m_toolDesc = "Erase Voxel - E";
	m_toolIcon = MTexture::getTexture("gui\\icon\\toolbar\\FillErase.png");
	m_keyBind = GKey::KeyBind(GLFW_KEY_F);
}

void VFTErase::inputTool() {
	if (GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT))
		m_fillArea->use(Voxel(0, 0));
}
void VFTErase::updateTool() {
	m_fillArea->create(true);
}
void VFTErase::renderTool() {
	if (!m_editMatrix->getMatrix()->containsPoint(*m_selectedVoxel)) return;
	renderFillMesh();
}
