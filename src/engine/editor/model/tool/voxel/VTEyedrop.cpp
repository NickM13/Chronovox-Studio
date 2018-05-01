#include "engine\editor\model\tool\voxel\VTEyedrop.h"

VTEyedrop::VTEyedrop()
	: VoxelTool() {
	m_toolName = "VTEyedrop";
	m_toolDesc = "Eyedropper - K";
	m_toolIcon = MTexture::getTexture("gui\\icon\\tool\\VoxelEyedrop.png");
	m_hasSubmenu = false;
}

void VTEyedrop::inputSingle() {
	if(GMouse::mouseDown(GLFW_MOUSE_BUTTON_LEFT) && m_editMatrix->getMatrix()->containsPoint(*m_selectedVoxel)) {
		*m_color = MColor::getInstance().getUnit(m_editMatrix->getMatrix()->getVoxel(*m_selectedVoxel).color);
	}
}
void VTEyedrop::updateSingle() {

}
void VTEyedrop::renderSingle() {
	renderSingleMesh(false);
}
