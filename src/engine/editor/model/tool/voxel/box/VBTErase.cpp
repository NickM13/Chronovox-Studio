#include "engine\editor\model\tool\voxel\box\VBTErase.h"

VBTErase::VBTErase()
	: VoxelBoxTool() {
	m_parent = "Box";
	m_toolName = "Box Erase Tool";
	m_toolDesc = "Erase a Box of Voxels";
	m_toolIcon = MTexture::getTexture("gui\\icon\\tool\\BoxErase.png");
	m_boxReplace = false;
	m_keyBind = GKey::KeyBind(GLFW_KEY_B);
}

void VBTErase::inputTool() {
	if (GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT)) {
		if (m_editMatrix->getMatrix()->containsPoint(*m_selectedVoxel)) {
			m_boxStart = *m_selectedVoxel;
			m_boxing = true;
		}
		else if (m_editMatrix->getMatrix()->containsPoint(*m_selectedVoxelOffset)) {
			m_boxStart = *m_selectedVoxelOffset;
			m_boxing = true;
		}
	}
	if (m_boxing) {
		if (GMouse::mouseDown(GLFW_MOUSE_BUTTON_LEFT)) {
			if (m_editMatrix->getMatrix()->containsPoint(*m_selectedVoxel)) {
				box(*m_selectedVoxel, Voxel(0, 0));
			}
			else if (m_editMatrix->getMatrix()->containsPoint(*m_selectedVoxelOffset)) {
				box(*m_selectedVoxelOffset, Voxel(0, 0));
			}
		}
		else {
			m_editMatrix->saveChanges();
			m_boxing = false;
		}
	}
}
void VBTErase::updateTool() {

}
void VBTErase::renderTool() {
	renderBoxMesh(false, true);
}
