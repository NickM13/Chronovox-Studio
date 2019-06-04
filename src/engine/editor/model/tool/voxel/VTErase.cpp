#include "engine\editor\model\tool\voxel\VTErase.h"

VTErase::VTErase()
	: VoxelTool() {
	m_parent = "Erase";
	m_toolName = "Voxel Erase Tool";
	m_toolDesc = "Erase Voxels - E";
	m_toolIcon = MTexture::getTexture("gui\\icon\\toolbar\\SingleErase.png");
	m_boxReplace = false;
	m_keyBind = GKey::KeyBind(GLFW_KEY_E);
}

void VTErase::inputTool() {
	if (GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT)) {
		if (GKey::modDown(GLFW_MOD_SHIFT)) {
			if (m_editMatrix->getMatrix()->containsPoint(*m_selectedVoxel)) {
				m_boxStart = *m_selectedVoxel;
				m_boxing = true;
			} else if (m_editMatrix->getMatrix()->containsPoint(*m_selectedVoxelOffset)) {
				m_boxStart = *m_selectedVoxelOffset;
				m_boxing = true;
			}
			m_voxelMode = VoxelMode::BOX;
		} else {
			m_voxelMode = VoxelMode::SINGLE;
		}
	}
	if (m_voxelMode == VoxelMode::BOX) {
		if (m_boxing) {
			if (GMouse::mouseDown(GLFW_MOUSE_BUTTON_LEFT)) {
				if (m_editMatrix->getMatrix()->containsPoint(*m_selectedVoxel)) {
					box(*m_selectedVoxel, Voxel(0, 0));
				} else if (m_editMatrix->getMatrix()->containsPoint(*m_selectedVoxelOffset)) {
					box(*m_selectedVoxelOffset, Voxel(0, 0));
				}
			} else {
				m_editMatrix->saveChanges();
				m_boxing = false;
			}
		}
	} else if (m_voxelMode == VoxelMode::SINGLE) {
		if (GMouse::mouseDown(GLFW_MOUSE_BUTTON_LEFT)) {
			m_editMatrix->getMatrix()->setVoxel(*m_selectedVoxel, Voxel(0, 0));
		}
	}
}
void VTErase::updateTool() {

}
void VTErase::renderTool() {
	if (GMouse::mouseDown(GLFW_MOUSE_BUTTON_LEFT)) {
		switch (m_voxelMode) {
		case VoxelMode::BOX:
			renderBoxMesh(false, true);
			break;
		case VoxelMode::SINGLE:
		default:
			renderSingleMesh(false);
			break;
		}
	} else {
		if (GKey::modDown(GLFW_MOD_SHIFT))
			renderBoxMesh(false, true);
		else
			renderSingleMesh(false);
	}
}
