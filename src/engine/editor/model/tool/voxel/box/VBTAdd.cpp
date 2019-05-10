#include "engine\editor\model\tool\voxel\box\VBTAdd.h"

VBTAdd::VBTAdd()
	: VoxelBoxTool() {
	m_parent = "Box";
	m_toolName = "Box Append Tool";
	m_toolDesc = "Add a Box of Voxels";
	m_toolIcon = MTexture::getTexture("gui\\icon\\toolbar\\BoxAdd.png");
	m_boxReplace = false;
	m_keyBind = GKey::KeyBind(GLFW_KEY_B);
}

void VBTAdd::inputTool() {
	if (GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT)) {
		m_boxStart = *m_selectedVoxelOffset;
		m_boxing = m_editMatrix->getMatrix()->containsPoint(m_boxStart);
	}
	if (m_boxing) {
		if (GMouse::mouseDown(GLFW_MOUSE_BUTTON_LEFT)) {
			if (m_editMatrix->getMatrix()->containsPoint(*m_selectedVoxelOffset)) {
				box(*m_selectedVoxelOffset, Voxel(1, MColor::getInstance().getUnitID(*m_color)));
			}
		}
		else {
			m_editMatrix->saveChanges();
			m_boxing = false;
		}
	}
}
void VBTAdd::updateTool() {

}
void VBTAdd::renderTool() {
	renderBoxMesh(true, false);
}
