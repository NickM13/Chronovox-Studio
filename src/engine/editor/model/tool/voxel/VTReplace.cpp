#include "engine\editor\model\tool\voxel\VTReplace.h"

VTReplace::VTReplace()
	: VoxelTool() {
	m_toolName = "VTReplace";
	m_toolDesc = "Replace Voxel - R";
	m_toolIcon = MTexture::getTexture("gui\\icon\\tool\\VoxelReplace.png");
	m_boxReplace = true;
}

void VTReplace::inputSingle() {
	if(GMouse::mouseDown(GLFW_MOUSE_BUTTON_LEFT)) {
		m_editMatrix->getMatrix()->setVoxel(*m_selectedVoxel, Voxel(1, MColor::getInstance().getUnitID(*m_color)));
	}
}
void VTReplace::updateSingle() {

}
void VTReplace::renderSingle() {
	renderSingleMesh(false);
}

void VTReplace::inputBox() {
	if(GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT) && m_editMatrix->getMatrix()->containsPoint(*m_selectedVoxel)) {
		m_boxStart = *m_selectedVoxel;
		m_boxing = true;
	}
	if(GMouse::mouseDown(GLFW_MOUSE_BUTTON_LEFT)) {
		box(*m_selectedVoxel, Voxel(1, MColor::getInstance().getUnitID(*m_color)));
	}
	if(m_boxing && GMouse::mouseReleased(GLFW_MOUSE_BUTTON_LEFT)) {
		m_editMatrix->saveChanges();
		m_boxing = false;
	}
}
void VTReplace::updateBox() {

}
void VTReplace::renderBox() {
	renderBoxMesh(false, false);
}

void VTReplace::inputFill() {
	if(GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT))
		m_fillArea->use(Voxel(1, MColor::getInstance().getUnitID(*m_color)));
}
void VTReplace::updateFill() {
	m_fillArea->create(true);
}
void VTReplace::renderFill() {
	if(!m_editMatrix->getMatrix()->containsPoint(*m_selectedVoxel)) return;
	renderFillMesh();
}
