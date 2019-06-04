#include "engine\editor\model\tool\voxel\VTReplace.h"

VTReplace::VTReplace()
	: VoxelTool() {
	m_parent = "Replace";
	m_toolName = "Voxel Replace Tool";
	m_toolDesc = "Replace Voxels - R";
	m_toolIcon = MTexture::getTexture("gui\\icon\\toolbar\\SingleReplace.png");
	m_boxReplace = true;
	m_keyBind = GKey::KeyBind(GLFW_KEY_R);
}

void VTReplace::inputTool() {
	if (GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT)) {
		if (GKey::modDown(GLFW_MOD_SHIFT)) {
			if (m_editMatrix->getMatrix()->containsPoint(*m_selectedVoxel)) {
				m_boxStart = *m_selectedVoxel;
				m_boxing = true;
				m_voxelMode = VoxelMode::BOX;
			}
		} else {
			m_voxelMode = VoxelMode::SINGLE;
		}
	}
	if (m_voxelMode == VoxelMode::BOX) {
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
	} else if (m_voxelMode == VoxelMode::SINGLE) {
		if (GMouse::mouseDown(GLFW_MOUSE_BUTTON_LEFT)) {
			m_editMatrix->getMatrix()->setVoxel(*m_selectedVoxel, Voxel(1, MColor::getInstance().getUnitID(*m_color)));
		}
	}
}
void VTReplace::updateTool() {

}
void VTReplace::renderTool() {
	if (GMouse::mouseDown(GLFW_MOUSE_BUTTON_LEFT)) {
		switch (m_voxelMode) {
		case VoxelMode::BOX:
			renderBoxMesh(false, false);
			break;
		case VoxelMode::SINGLE:
		default:
			renderSingleMesh(false);
			break;
		}
	} else {
		if (GKey::modDown(GLFW_MOD_SHIFT))
			renderBoxMesh(false, false);
		else
			renderSingleMesh(false);
	}
}
