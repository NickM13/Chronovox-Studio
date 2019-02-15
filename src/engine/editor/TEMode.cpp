#include "engine\editor\TEMode.h"
#include "engine\editor\menu\EditorOverlay.h"

TEMode::TEMode() {
	Camera::init();
}

bool TEMode::hasChanged() {
	return false;
}
void TEMode::setChanged(bool p_changed) {

}

void TEMode::setPath(std::string p_path) {
	m_directory = p_path.substr(0, p_path.find_last_of('\\') + 1);
	m_name = p_path.substr(p_path.find_last_of('\\') + 1);
}
std::string TEMode::getName() {
	return m_name;
}
std::string TEMode::getDirectory() {
	return m_directory;
}
std::string TEMode::getPath() {
	return m_directory + m_name;
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
bool TEMode::fileSave() { return false; }
bool TEMode::fileSaveAs() { return false; }
void TEMode::fileExit() {}

void TEMode::editUndo() {}
void TEMode::editRedo() {}
