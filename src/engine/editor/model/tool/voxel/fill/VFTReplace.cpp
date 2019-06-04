#include "engine\editor\model\tool\voxel\fill\VFTReplace.h"

VFTReplace::VFTReplace()
	: VoxelTool() {
	m_parent = "Fill";
	m_toolName = "Fill Replace Tool";
	m_toolDesc = "Replace Voxel - R";
	m_toolIcon = MTexture::getTexture("gui\\icon\\toolbar\\FillReplace.png");
	m_keyBind = GKey::KeyBind(GLFW_KEY_F);
}

void VFTReplace::inputTool() {
	if (GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT))
		m_fillArea->use(Voxel(1, MColor::getInstance().getUnitID(*m_color)));
}
void VFTReplace::updateTool() {
	if (!GMouse::mouseDown(GLFW_MOUSE_BUTTON_LEFT))
		m_fillArea->create(true);
}
void VFTReplace::renderTool() {
	if (m_editMatrix->getMatrix()->containsPoint(*m_selectedVoxel))
		renderFillMesh();
}
