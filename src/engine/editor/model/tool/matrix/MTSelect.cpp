#include "engine\editor\model\tool\matrix\MTSelect.h"
#include "engine\gfx\model\MModelObj.h"

MTSelect::MTSelect()
	: MatrixTool() {
	m_parent = "MatrixSelect";
	m_toolName = "Select Matrix Tool";
	m_toolDesc = "Select Matrix - V";
	m_toolIcon = MTexture::getTexture("gui\\icon\\toolbar\\MatrixSelect.png");
	m_keyBind = GKey::KeyBind(GLFW_KEY_V, 0);
}

void MTSelect::inputTool() {

}
void MTSelect::updateTool() {

}
void MTSelect::renderTool() {

}
