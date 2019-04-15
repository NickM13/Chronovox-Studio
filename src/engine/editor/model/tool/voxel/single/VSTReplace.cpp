#include "engine\editor\model\tool\voxel\single\VSTReplace.h"

VSTReplace::VSTReplace()
	: VoxelSingleTool() {
	m_parent = "Replace";
	m_toolName = "Voxel Replace Tool";
	m_toolDesc = "Replace a Single Voxel - R";
	m_toolIcon = MTexture::getTexture("gui\\icon\\tool\\SingleReplace.png");
	m_keyBind = GKey::KeyBind(GLFW_KEY_R);
}

void VSTReplace::inputTool() {
	if (GMouse::mouseDown(GLFW_MOUSE_BUTTON_LEFT)) {
		m_editMatrix->getMatrix()->setVoxel(*m_selectedVoxel, Voxel(1, MColor::getInstance().getUnitID(*m_color)));
	}
}
void VSTReplace::updateTool() {

}
void VSTReplace::renderTool() {
	renderSingleMesh(false);
}
