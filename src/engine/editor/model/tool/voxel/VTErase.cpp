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
	renderSingleMesh(false);
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
	renderBoxMesh(false, true);
}

void VTErase::inputFill() {
	if(GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT))
		m_fillArea->use(Voxel(0, 0));
}
void VTErase::updateFill() {
	m_fillArea->create(true);
}
void VTErase::renderFill() {
	if(!m_editMatrix->getMatrix()->containsPoint(*m_selectedVoxel)) return;
	glPushMatrix();
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		glm::vec3 _offset = m_editMatrix->getPos() + 0.5f;
		GLfloat c = 1.025f;
		/*
		glTranslatef(_offset.x, _offset.y, _offset.z);
		for(glm::ivec3 voxel : m_fillVoxels) {
			MMesh::render("SCube", voxel, glm::vec3(c), {}, m_colorSelect);
		}
		*/
	}
	glPopMatrix();
}
