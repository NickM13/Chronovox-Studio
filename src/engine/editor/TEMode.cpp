#include "engine\editor\TEMode.h"
#include "engine\editor\menu\EditorOverlay.h"

TEMode::TEMode() {
	Camera::init();
}

void TEMode::input(Sint8 p_guiFlags) {
	Camera::input(p_guiFlags);
	inputEditor(p_guiFlags);
}
void TEMode::update(GLfloat p_deltaUpdate) {
	Camera::update(p_deltaUpdate);
	updateEditor(p_deltaUpdate);
}
void TEMode::render() {
	Camera::renderSkybox();
	glPushMatrix();
	{
		Camera::applyTransformation();
		renderEditor();
		Camera::renderFocus();
	}
	glPopMatrix();
}

void TEMode::inputEditor(Sint8 p_guiFlags) {}
void TEMode::updateEditor(GLfloat p_updateTime) {}
void TEMode::renderEditor() {}

void TEMode::fileNew() {}
void TEMode::fileOpen() {}
void TEMode::fileSave() {}
void TEMode::fileExit() {}

void TEMode::editUndo() {}
void TEMode::editRedo() {}
