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
	if(!m_editMatrix->getMatrix()->containsPoint(*m_selectedVoxel)) return;
	glPushMatrix();
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		Vector3<GLfloat> _mPos = m_editMatrix->getPos();
		Vector3<GLfloat> _offset = _mPos + *m_selectedVoxel + 0.5f;
		GLfloat c = 1.025f;

		glTranslatef(_offset.x, _offset.y, _offset.z);

		MMesh::render("SCube", {}, c, {}, m_colorSelect);
	}
	glPopMatrix();
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
	if(!m_editMatrix->getMatrix()->containsPoint(*m_selectedVoxel)) return;
	glPushMatrix();
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		Vector3<GLfloat> _mPos = m_editMatrix->getPos();
		Vector3<GLfloat> _offset = _mPos + *m_selectedVoxel + 0.5f;
		GLfloat c = 1.025f;

		MMesh::render("SCube", _offset, c, {}, m_colorSelect);
	}
	glPopMatrix();
}

void VTReplace::inputFill() {
	if(GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT))
		fill(Voxel(1, MColor::getInstance().getUnitID(*m_color)));
}
void VTReplace::updateFill() {
	fillInsert();
}
void VTReplace::renderFill() {
	if(!m_editMatrix->getMatrix()->containsPoint(*m_selectedVoxel)) return;
	glPushMatrix();
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		Vector3<GLfloat> _offset = m_editMatrix->getPos() + 0.5f;
		GLfloat c = 1.025f;

		glTranslatef(_offset.x, _offset.y, _offset.z);
		for(Vector3<Sint32> voxel : m_fillVoxels) {
			MMesh::render("SCube", voxel, c, {}, m_colorSelect);
		}
	}
	glPopMatrix();
}
