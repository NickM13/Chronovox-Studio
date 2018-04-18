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
	if(!m_editMatrix->getMatrix()->containsPoint(*m_selectedVoxel)) return;
	glPushMatrix();
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		Vector3<GLfloat> _mPos = m_editMatrix->getPos();
		Vector3<GLfloat> _offset = _mPos + *m_selectedVoxel + 0.5f;
		GLfloat c = 1.025f;

		glTranslatef(_offset.x, _offset.y, _offset.z);

		MMesh::render("SCube", {0, 0, 0}, {c, c, c}, {}, m_colorSelect);
	}
	glPopMatrix();
}
