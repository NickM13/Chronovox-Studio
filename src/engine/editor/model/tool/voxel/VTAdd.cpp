#include "engine\editor\model\tool\voxel\VTAdd.h"
#include "engine\gfx\shader\Shader.h"

VTAdd::VTAdd()
	: VoxelTool() {
	m_toolName = "VTAdd";
	m_toolDesc = "Add Voxel - T";
	m_toolIcon = MTexture::getTexture("gui\\icon\\tool\\VoxelAdd.png");
	m_boxReplace = false;
}

void VTAdd::inputSingle() {
	if (GMouse::mouseDown(GLFW_MOUSE_BUTTON_LEFT)) {
		m_editMatrix->getMatrix()->setVoxel(*m_selectedVoxelOffset, Voxel(1, MColor::getInstance().getUnitID(*m_color)));
	}
}
void VTAdd::updateSingle() {

}
void VTAdd::renderSingle() {
	renderSingleMesh(true);
}

void VTAdd::inputBox() {
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
void VTAdd::updateBox() {

}
void VTAdd::renderBox() {
	renderBoxMesh(true, false);
}

void VTAdd::inputFill() {
	if (GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT))
		m_fillArea->use(Voxel(1, MColor::getInstance().getUnitID(*m_color)));
}
void VTAdd::updateFill() {
	m_fillArea->create(false);
}
void VTAdd::renderFill() {
	if (!m_editMatrix->getMatrix()->containsPoint(*m_selectedVoxelOffset)) return;
	renderFillMesh();
}
