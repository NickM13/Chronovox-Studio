#include "engine\editor\model\tool\voxel\VTAdd.h"

VTAdd::VTAdd() 
	: VoxelTool() {
	m_toolName = "VTAdd";
	m_toolDesc = "Add Voxel - T";
	m_toolIcon = MTexture::getTexture("gui\\icon\\tool\\VoxelAdd.png");
	m_boxReplace = false;
}

void VTAdd::inputSingle() {
	if(GMouse::mouseDown(GLFW_MOUSE_BUTTON_LEFT)) {
		m_editMatrix->getMatrix()->setVoxel(*m_selectedVoxelOffset, Voxel(1, MColor::getInstance().getUnitID(*m_color)));
	}
}
void VTAdd::updateSingle() {
	 
}
void VTAdd::renderSingle() {
	if(*m_selectedVoxelOffset == Vector3<Sint32>(-1, -1, -1)) return;
	glPushMatrix();
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		Vector3<GLfloat> _mPos = m_editMatrix->getPos();
		Vector3<GLfloat> _offset = _mPos + *m_selectedVoxelOffset + 0.5f;
		GLfloat c = 0.925f;

		MMesh::render("SCube", _offset, c, {}, m_editMatrix->getMatrix()->containsPoint(*m_selectedVoxelOffset) ? m_colorSelect : m_colorError);
	}
	glPopMatrix();
}

void VTAdd::inputBox() {
	if(GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT)) {
		m_boxStart = *m_selectedVoxelOffset;
		m_boxing = m_editMatrix->getMatrix()->containsPoint(m_boxStart);
	}
	if(m_boxing) {
		if(GMouse::mouseDown(GLFW_MOUSE_BUTTON_LEFT)) {
			box(*m_selectedVoxelOffset, Voxel(1, MColor::getInstance().getUnitID(*m_color)));
		}
		if(!GMouse::mouseDown(GLFW_MOUSE_BUTTON_LEFT)) {
			m_editMatrix->saveChanges();
			m_boxing = false;
		}
	}
}
void VTAdd::updateBox() {
	 
}
void VTAdd::renderBox() {
	glPushMatrix();
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		Vector3<GLfloat> _mPos = m_editMatrix->getPos();
		Vector3<GLfloat> _offset = _mPos + 0.5f;
		GLfloat c;

		if(m_boxing) {
			c = 1.075f;
			_offset = _offset + Vector3<GLfloat>(m_boxEnd - m_boxStart) / 2 + m_boxStart;
			MMesh::render("SCube", _offset, Vector3<GLfloat>(m_boxStart - m_boxEnd).abs() + c, {}, m_colorSelect);
		}
		else {
			c = 0.925f;
			_offset = _offset + *m_selectedVoxelOffset;
			MMesh::render("SCube", _offset, c, {}, m_editMatrix->getMatrix()->containsPoint(*m_selectedVoxelOffset) ? m_colorSelect : m_colorError);
		}
	}
	glPopMatrix();
}

void VTAdd::inputFill() {
	if(GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT))
		fill(Voxel(1, MColor::getInstance().getUnitID(*m_color)));
}
void VTAdd::updateFill() {
	fillAppend();
}
void VTAdd::renderFill() {
	glPushMatrix();
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		Vector3<GLfloat> _offset = m_editMatrix->getPos() + 0.5f;
		GLfloat c = 0.925f;

		glTranslatef(_offset.x, _offset.y, _offset.z);

		if(m_editMatrix->getMatrix()->containsPoint(*m_selectedVoxelOffset)) {
			for(Vector3<Sint32> voxel : m_fillVoxels) {
				MMesh::render("SCube", voxel, c, {}, m_colorSelect);
			}
		}
		else {
			MMesh::render("SCube", *m_selectedVoxelOffset, c, {}, m_colorError);
		}
	}
	glPopMatrix();
}
