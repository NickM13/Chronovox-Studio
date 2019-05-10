#include "engine\editor\model\tool\voxel\fill\VFTAdd.h"

VFTAdd::VFTAdd()
	: VoxelFillTool() {
	m_parent = "Fill";
	m_toolName = "Fill Append Tool";
	m_toolDesc = "Fill Area with Voxels";
	m_toolIcon = MTexture::getTexture("gui\\icon\\toolbar\\FillAdd.png");
	m_keyBind = GKey::KeyBind(GLFW_KEY_F);
}

void VFTAdd::inputTool() {
	if (GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT))
		m_fillArea->use(Voxel(1, MColor::getInstance().getUnitID(*m_color)));
}
void VFTAdd::updateTool() {
	m_fillArea->create(false);
}
void VFTAdd::renderTool() {
	if (!m_editMatrix->getMatrix()->containsPoint(*m_selectedVoxelOffset)) return;
	renderFillMesh();
}
