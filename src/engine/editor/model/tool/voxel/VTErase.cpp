#include "engine\editor\model\tool\voxel\VTErase.h"

VTErase::VTErase()
	: VoxelTool() {
	m_toolName = "VTErase";
	m_toolDesc = "Erase Voxel - E";
	m_toolIcon = MTexture::getTexture("gui\\icon\\tool\\VoxelErase.png");
	m_boxReplace = false;
}

void VTErase::inputSingle() {
	if(GMouse::mouseDown(GLFW_MOUSE_BUTTON_LEFT)) {
		m_editMatrix->getMatrix()->setVoxel(*m_selectedVoxel, Voxel(0, 0));
	}
}
void VTErase::updateSingle() {
	
}
void VTErase::renderSingle() {
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

void VTErase::inputBox() {
	if(GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT)) {
		m_boxStart = *m_selectedVoxel;
		m_boxing = true;
	}
	if(GMouse::mouseDown(GLFW_MOUSE_BUTTON_LEFT)) {
		box(*m_selectedVoxel, Voxel(0, 0));
	}
	if(m_boxing && GMouse::mouseReleased(GLFW_MOUSE_BUTTON_LEFT)) {
		m_editMatrix->saveChanges();
		m_boxing = false;
	}
}
void VTErase::updateBox() {

}
void VTErase::renderBox() {
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

void VTErase::inputFill() {
	if(GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT))
		fill(Voxel(0, 0));
}
void VTErase::updateFill() {
	fillInsert();
}
void VTErase::renderFill() {
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
