#include "engine\editor\model\tool\voxel\VTErase.h"

VTErase::VTErase()
	: VoxelTool() {
	m_toolName = "VTErase";
	m_toolDesc = "Erase Voxel - E";
	m_toolIcon = MTexture::getTexture("gui\\icon\\tool\\VoxelErase.png");
	m_boxReplace = false;
}

void VTErase::inputSingle() {
	if (GMouse::mouseDown(GLFW_MOUSE_BUTTON_LEFT)) {
		m_editMatrix->getMatrix()->setVoxel(*m_selectedVoxel, Voxel(0, 0));
	}
}
void VTErase::updateSingle() {

}
void VTErase::renderSingle() {
	renderSingleMesh(false);
}

void VTErase::inputBox() {
	if (GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT)) {
		m_boxStart = *m_selectedVoxel;
		m_boxing = true;
	}
	if (m_boxing) {
		if (GMouse::mouseDown(GLFW_MOUSE_BUTTON_LEFT)) {
			if (m_editMatrix->getMatrix()->containsPoint(*m_selectedVoxel)) {
				box(*m_selectedVoxel, Voxel(0, 0));
			}
		}
		else {
			m_editMatrix->saveChanges();
			m_boxing = false;
		}
	}
}
void VTErase::updateBox() {

}
void VTErase::renderBox() {
	renderBoxMesh(false, true);
}

void VTErase::inputFill() {
	if (GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT))
		m_fillArea->use(Voxel(0, 0));
}
void VTErase::updateFill() {
	m_fillArea->create(true);
}
void VTErase::renderFill() {
	if (!m_editMatrix->getMatrix()->containsPoint(*m_selectedVoxel)) return;
	renderFillMesh();
}
