#include "engine\editor\TEMode.h"
#include "engine\editor\menu\EditorOverlay.h"
#include "engine\gfx\shader\Shader.h"

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
	Shader::pushMatrixView();
	Camera::applyTransformation();
	Camera::applyLightDirection();

	// Sky box no longer needed but Im leaving this here just incase I change my mind
	//Camera::renderSkybox();

	renderEditor();
	Camera::renderFocus();
	Shader::popMatrixView();
}
void TEMode::renderShadow() {
	Shader::pushMatrixView();
	renderEditorShadow();
	Shader::popMatrixView();
}

void TEMode::inputEditor(Sint8 p_guiFlags) {}
void TEMode::updateEditor(GLfloat p_updateTime) {}
void TEMode::renderEditor() {}
void TEMode::renderEditorShadow() {}

void TEMode::fileNew() {}
void TEMode::fileOpen() {}
void TEMode::fileSave() {}
void TEMode::fileExit() {}

void TEMode::editUndo() {}
void TEMode::editRedo() {}
