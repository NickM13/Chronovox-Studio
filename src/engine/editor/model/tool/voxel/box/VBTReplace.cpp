#include "engine\editor\model\tool\voxel\box\VBTReplace.h"

VBTReplace::VBTReplace()
	: VoxelBoxTool() {
	m_parent = "Box";
	m_toolName = "Box Replace Tool";
	m_toolDesc = "Replace a Box of Voxels";
	m_toolIcon = MTexture::getTexture("gui\\icon\\tool\\BoxReplace.png");
	m_boxReplace = false;
	m_keyBind = GKey::KeyBind(GLFW_KEY_B);
}

void VBTReplace::inputTool() {
	if (GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT) && m_editMatrix->getMatrix()->containsPoint(*m_selectedVoxel)) {
		m_boxStart = *m_selectedVoxel;
		m_boxing = true;
	}
	if (m_boxing) {
		if (GMouse::mouseDown(GLFW_MOUSE_BUTTON_LEFT)) {
			if (m_editMatrix->getMatrix()->containsPoint(*m_selectedVoxel)) {
				box(*m_selectedVoxel, Voxel(1, MColor::getInstance().getUnitID(*m_color)));
			} else if (m_editMatrix->getMatrix()->containsPoint(*m_selectedVoxelOffset)) {
				box(*m_selectedVoxelOffset, Voxel(1, MColor::getInstance().getUnitID(*m_color)));
			}
		}
		if (GMouse::mouseReleased(GLFW_MOUSE_BUTTON_LEFT)) {
			m_editMatrix->saveChanges();
			m_boxing = false;
		}
	}
}
void VBTReplace::updateTool() {

}
void VBTReplace::renderTool() {
	if (!m_editMatrix->getMatrix()->containsPoint(*m_selectedVoxel) && !m_boxing) return;
	renderBoxMesh(false, false);
}
