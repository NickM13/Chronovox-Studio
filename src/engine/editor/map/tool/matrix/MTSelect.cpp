#include "engine\editor\model\tool\matrix\MTSelect.h"
#include "engine\gfx\model\MModelObj.h"
#include "engine\editor\Editor.h"

MTSelect::MTSelect()
	: MatrixTool() {
	m_parent = "MatrixSelect";
	m_toolName = "Select Matrix Tool";
	m_toolDesc = "Select Matrix - V";
	m_toolIcon = MTexture::getTexture("gui\\icon\\toolbar\\MatrixSelect.png");
	m_keyBind = GKey::KeyBind(GLFW_KEY_V, 0);
}

void MTSelect::inputTool() {
	if (GMouse::mouseDoubleClicked(GLFW_MOUSE_BUTTON_LEFT)) {
		Editor::getModel()->editMatrixProperties();
	}
}
void MTSelect::updateTool() {

}
void MTSelect::renderTool() {

}
