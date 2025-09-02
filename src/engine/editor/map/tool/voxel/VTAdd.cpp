#include "engine\editor\model\tool\voxel\VTAdd.h"

VTAdd::VTAdd() 
	: VoxelTool() {
	m_parent = "Add";
	m_toolName = "Voxel Append Tool";
	m_toolDesc = "Add Voxels (T)";
	m_toolIcon = MTexture::getTexture("gui\\icon\\toolbar\\SingleAdd.png");
	m_boxReplace = false;
	m_keyBind = GKey::KeyBind(GLFW_KEY_T);
}

void VTAdd::inputTool() {
	if (GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT)) {
		m_editMatrix->getMatrix()->autoResize(m_selectedVoxel, m_selectedVoxelOffset);
		if (GKey::modDown(GLFW_MOD_SHIFT)) {
			m_boxStart = *m_selectedVoxelOffset;
			m_boxing = m_editMatrix->getMatrix()->containsPoint(m_boxStart);
			m_voxelMode = VoxelMode::BOX;
		} else {
			m_voxelMode = VoxelMode::SINGLE;
		}
	}
	if (m_voxelMode == VoxelMode::BOX) {
		if (m_boxing) {
			if (GMouse::mouseDown(GLFW_MOUSE_BUTTON_LEFT)) {
				if (m_editMatrix->getMatrix()->containsPoint(*m_selectedVoxelOffset)) {
					box(*m_selectedVoxelOffset, Voxel(1, *m_color));
				}
			} else {
				m_editMatrix->saveChanges();
				m_boxing = false;
			}
		}
	}
	else if (m_voxelMode == VoxelMode::SINGLE) {
		if (GMouse::mouseDown(GLFW_MOUSE_BUTTON_LEFT)) {
			m_editMatrix->getMatrix()->setVoxel(*m_selectedVoxelOffset, Voxel(1, *m_color));
		}
	}
}
void VTAdd::updateTool() {

}
void VTAdd::renderTool() {
	if (GMouse::mouseDown(GLFW_MOUSE_BUTTON_LEFT)) {
		switch (m_voxelMode) {
		case VoxelMode::BOX:
			renderBoxMesh(true, false);
			break;
		case VoxelMode::SINGLE:
		default:
			renderSingleMesh(true);
			break;
		}
	} else {
		if (GKey::modDown(GLFW_MOD_SHIFT))
			renderBoxMesh(true, false);
		else
			renderSingleMesh(true);
	}
}
