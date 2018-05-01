#include "engine\editor\model\tool\Tool.h"

EditMatrix* Tool::m_editMatrix;
Sint32* Tool::m_subTool;
glm::vec4 Tool::m_colorSelect, Tool::m_colorError;

void Tool::init(EditMatrix* p_editMatrix, Sint32* p_subTool) {
	m_editMatrix = p_editMatrix;
	m_subTool = p_subTool;
	m_colorSelect = glm::vec4(0.5f, 0.7f, 1, 0.75f);
	m_colorError = glm::vec4(1, 0, 0, 0.75f);
}

void Tool::enable() {}
void Tool::disable() {}
