#include "engine\editor\model\tool\matrix\MTMove.h"
#include "engine\gfx\model\MModelObj.h"

MTMove::MTMove()
	: MatrixTool() {
	m_parent = "MatrixTransform";
	m_toolName = "Move Matrix Tool";
	m_toolDesc = "Edit Matrix - V";
	m_toolIcon = MTexture::getTexture("gui\\icon\\tool\\MatrixMove.png");
	m_scaling = false;
	m_keyBind = GKey::KeyBind(GLFW_KEY_V);
}

void MTMove::inputTool() {
	if (GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT) && *m_selectedScale != 0) {
		m_startPos = *m_scalePos;
		m_scaling = true;
	}
	if (m_scaling && !GMouse::mouseDown(GLFW_MOUSE_BUTTON_LEFT)) {
		m_scaling = false;
		m_editMatrix->reset();
	}
}
void MTMove::updateTool() {
	if (m_scaling) {
		glm::vec3 norm;
		GLfloat denom, dist, dragDiff;
		switch (*m_selectedScale) {
		case FACE_SOUTH:
		case FACE_NORTH:
			norm = m_startPos - Camera::getPosition();
			norm.x = 0;
			norm = glm::normalize(norm);
			denom = glm::dot(norm, Camera::getMouseDirection());
			if (abs(denom) > 0.0001f) {
				dist = glm::dot(m_startPos - Camera::getPosition(), norm) / denom;
				if (dist >= 0.0001f) {
					dragDiff = (Camera::getPosition() + Camera::getMouseDirection() * dist).x - m_startPos.x;
					m_editMatrix->getMatrix()->setPosition(m_editMatrix->getInitMatrix()->getPos() + glm::vec3(dragDiff, 0, 0));
				}
			}
			break;
		case FACE_BOTTOM:
		case FACE_TOP:
			norm = m_startPos - Camera::getPosition();
			norm.y = 0;
			norm = glm::normalize(norm);
			denom = glm::dot(norm, Camera::getMouseDirection());
			if (abs(denom) > 0.0001f) {
				dist = glm::dot(m_startPos - Camera::getPosition(), norm) / denom;
				if (dist >= 0.0001f) {
					dragDiff = (Camera::getPosition() + Camera::getMouseDirection() * dist).y - m_startPos.y;
					m_editMatrix->getMatrix()->setPosition(m_editMatrix->getInitMatrix()->getPos() + glm::vec3(0, dragDiff, 0));
				}
			}
			break;
		case FACE_WEST:
		case FACE_EAST:
			norm = (m_startPos - Camera::getPosition());
			norm.z = 0;
			norm = glm::normalize(norm);
			denom = glm::dot(norm, Camera::getMouseDirection());
			if (abs(denom) > 0.0001f) {
				dist = glm::dot(m_startPos - Camera::getPosition(), norm) / denom;
				if (dist >= 0.0001f) {
					dragDiff = (Camera::getPosition() + Camera::getMouseDirection() * dist).z - m_startPos.z;
					m_editMatrix->getMatrix()->setPosition(m_editMatrix->getInitMatrix()->getPos() + glm::vec3(0, 0, dragDiff));
				}
			}
			break;
		}
	}
}
void MTMove::renderTool() {
	glm::vec3 s = glm::vec3((*m_matrices)[m_editMatrix->getId()]->getSize()) / 2.f;
	glm::vec3 _offset = (*m_matrices)[m_editMatrix->getId()]->getPos() + s;

	Shader::pushMatrixModel();
	Shader::translate(glm::vec3(_offset));

	Shader::setColor(((*m_selectedScale == FACE_SOUTH || *m_selectedScale == FACE_NORTH) ? glm::vec4(1.0f, 0.25f, 0.25f, 1.0f) : glm::vec4(0.75f, 0.0f, 0.0f, 1.f)));
	Shader::pushMatrixModel();
	Shader::translate(glm::vec3(s.x, 0, 0));
	Shader::rotate(-90, glm::vec3(0, 0, 1));
	MModelObj::get("Arrow.obj")->renderObj();
	Shader::popMatrixModel();

	Shader::pushMatrixModel();
	Shader::translate(glm::vec3(-s.x, 0, 0));
	Shader::rotate(90, glm::vec3(0, 0, 1));
	MModelObj::get("Arrow.obj")->renderObj();
	Shader::popMatrixModel();

	Shader::setColor(((*m_selectedScale == FACE_BOTTOM || *m_selectedScale == FACE_TOP) ? glm::vec4(0.25f, 1.0f, 0.25f, 1.0f) : glm::vec4(0.0f, 0.75f, 0.0f, 1.f)));
	Shader::pushMatrixModel();
	Shader::translate(glm::vec3(0, s.y, 0));
	//Shader::rotate(0, glm::vec3(0, 0, 1));
	MModelObj::get("Arrow.obj")->renderObj();
	Shader::popMatrixModel();

	Shader::pushMatrixModel();
	Shader::translate(glm::vec3(0, -s.y, 0));
	Shader::rotate(180, glm::vec3(0, 0, 1));
	MModelObj::get("Arrow.obj")->renderObj();
	Shader::popMatrixModel();

	Shader::setColor(((*m_selectedScale == FACE_WEST || *m_selectedScale == FACE_EAST) ? glm::vec4(0.25f, 0.25f, 1.0f, 1.0f) : glm::vec4(0.0f, 0.0f, 0.75f, 1.f)));
	Shader::pushMatrixModel();
	Shader::translate(glm::vec3(0, 0, s.z));
	Shader::rotate(90, glm::vec3(1, 0, 0));
	MModelObj::get("Arrow.obj")->renderObj();
	Shader::popMatrixModel();

	Shader::pushMatrixModel();
	Shader::translate(glm::vec3(0, 0, -s.z));
	Shader::rotate(-90, glm::vec3(1, 0, 0));
	MModelObj::get("Arrow.obj")->renderObj();
	Shader::popMatrixModel();

	Shader::popMatrixModel();
}
