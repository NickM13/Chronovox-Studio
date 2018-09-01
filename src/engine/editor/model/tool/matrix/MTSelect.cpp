#include "engine\editor\model\tool\matrix\MTSelect.h"
#include "engine\gfx\model\MModelObj.h"

MTSelect::MTSelect()
	: MatrixTool() {
	m_toolName = "MTSelect";
	m_toolDesc = "Edit Matrix - M";
	m_toolIcon = MTexture::getTexture("gui\\icon\\tool\\MatrixSelect.png");
	m_scaling = false;
}

void MTSelect::inputSelect() {}
void MTSelect::updateSelect() {}
void MTSelect::renderSelect() {}

void MTSelect::inputMove() {
	if(GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT) && *m_selectedScale != 0) {
		m_startPos = *m_scalePos;

		m_scaling = true;
	}
	if(m_scaling && !GMouse::mouseDown(GLFW_MOUSE_BUTTON_LEFT)) {
		m_scaling = false;
		m_editMatrix->reset();
	}
}
void MTSelect::updateMove() {
	if(m_scaling) {
		glm::vec3 norm;
		GLfloat denom, dist, dragDiff;
		switch(*m_selectedScale) {
		case FACE_SOUTH:
		case FACE_NORTH:
			norm = m_startPos - Camera::getPosition();
			norm.x = 0;
			norm = glm::normalize(norm);
			denom = glm::dot(norm, Camera::getMouseDirection());
			if(abs(denom) > 0.0001f) {
				dist = glm::dot(m_startPos - Camera::getPosition(), norm) / denom;
				if(dist >= 0.0001f) {
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
			if(abs(denom) > 0.0001f) {
				dist = glm::dot(m_startPos - Camera::getPosition(), norm) / denom;
				if(dist >= 0.0001f) {
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
			if(abs(denom) > 0.0001f) {
				dist = glm::dot(m_startPos - Camera::getPosition(), norm) / denom;
				if(dist >= 0.0001f) {
					dragDiff = (Camera::getPosition() + Camera::getMouseDirection() * dist).z - m_startPos.z;
					m_editMatrix->getMatrix()->setPosition(m_editMatrix->getInitMatrix()->getPos() + glm::vec3(0, 0, dragDiff));
				}
			}
			break;
		}
	}
}
void MTSelect::renderMove() {
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

void MTSelect::inputResize() {
	if(GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT) && *m_selectedScale != 0) {
		m_startPos = *m_scalePos;
		m_scaling = true;
	}
	if(m_scaling && !GMouse::mouseDown(GLFW_MOUSE_BUTTON_LEFT)) {
		m_scaling = false;
		m_editMatrix->reset();
	}
}
void MTSelect::updateResize() {
	if(m_scaling) {
		glm::vec3 p_camDir;
		GLfloat dist, denom, p, m_dragDifference;
		denom = glm::dot(p_camDir, Camera::getMouseDirection());
		switch(*m_selectedScale) {
		case FACE_SOUTH:
			p_camDir = m_startPos - Camera::getPosition();
			p_camDir.x = 0;
			p_camDir = glm::normalize(p_camDir);
			denom = glm::dot(p_camDir, Camera::getMouseDirection());
			if(abs(denom) > 0.0001f) {
				dist = glm::dot(m_startPos - Camera::getPosition(), p_camDir) / denom;
				if(dist >= 0.0001f) {
					m_dragDifference = (Camera::getPosition() + Camera::getMouseDirection() * dist).x - m_startPos.x;
					if(abs(m_dragDifference) >= 1 && m_editMatrix->getMatrix()->getSize().x - m_dragDifference > 0) {
						p = m_editMatrix->getMatrix()->getPos().x;
						if(m_dragDifference > 0) m_editMatrix->getMatrix()->shiftVoxels(glm::ivec3(-m_dragDifference, 0, 0));
						m_editMatrix->getMatrix()->setSize(m_editMatrix->getMatrix()->getSize() - glm::ivec3(m_dragDifference, 0, 0));
						m_editMatrix->getMatrix()->addPosition(glm::ivec3(m_dragDifference, 0, 0));
						if(m_dragDifference < 0) m_editMatrix->getMatrix()->shiftVoxels(glm::ivec3(-m_dragDifference, 0, 0));
						m_startPos.x += m_editMatrix->getMatrix()->getPos().x - p;
					}
				}
			}
			break;
		case FACE_NORTH:
			p_camDir = m_startPos - Camera::getPosition();
			p_camDir.x = 0;
			p_camDir = glm::normalize(p_camDir);
			denom = glm::dot(p_camDir, Camera::getMouseDirection());
			if(abs(denom) > 0.0001f) {
				dist = glm::dot(m_startPos - Camera::getPosition(), p_camDir) / denom;
				if(dist >= 0.0001f) {
					m_dragDifference = (Camera::getPosition() + Camera::getMouseDirection() * dist).x - m_startPos.x;
					if(abs(m_dragDifference) >= 1 && m_editMatrix->getMatrix()->getSize().x + m_dragDifference > 0) {
						p = m_editMatrix->getMatrix()->getSize().x;
						m_editMatrix->getMatrix()->setSize(m_editMatrix->getMatrix()->getSize() + glm::ivec3(m_dragDifference, 0, 0));
						m_startPos.x += m_editMatrix->getMatrix()->getSize().x - p;
					}
				}
			}
			break;
		case FACE_BOTTOM:
			p_camDir = m_startPos - Camera::getPosition();
			p_camDir.y = 0;
			p_camDir = glm::normalize(p_camDir);
			denom = glm::dot(p_camDir, Camera::getMouseDirection());
			if(abs(denom) > 0.0001f) {
				dist = glm::dot(m_startPos - Camera::getPosition(), p_camDir) / denom;
				if(dist >= 0.0001f) {
					m_dragDifference = (Camera::getPosition() + Camera::getMouseDirection() * dist).y - m_startPos.y;
					if(abs(m_dragDifference) >= 1 && m_editMatrix->getMatrix()->getSize().y - m_dragDifference > 0) {
						p = m_editMatrix->getMatrix()->getPos().y;
						if(m_dragDifference > 0) m_editMatrix->getMatrix()->shiftVoxels(glm::ivec3(0, -m_dragDifference, 0));
						m_editMatrix->getMatrix()->setSize(m_editMatrix->getMatrix()->getSize() - glm::ivec3(0, m_dragDifference, 0));
						m_editMatrix->getMatrix()->addPosition(glm::ivec3(0, m_dragDifference, 0));
						if(m_dragDifference < 0) m_editMatrix->getMatrix()->shiftVoxels(glm::ivec3(0, -m_dragDifference, 0));
						m_startPos.y += m_editMatrix->getMatrix()->getPos().y - p;
					}
				}
			}
			break;
		case FACE_TOP:
			p_camDir = m_startPos - Camera::getPosition();
			p_camDir.y = 0;
			p_camDir = glm::normalize(p_camDir);
			denom = glm::dot(p_camDir, Camera::getMouseDirection());
			if(abs(denom) > 0.0001f) {
				dist = glm::dot(m_startPos - Camera::getPosition(), p_camDir) / denom;
				if(dist >= 0.0001f) {
					m_dragDifference = (Camera::getPosition() + Camera::getMouseDirection() * dist).y - m_startPos.y;
					if(abs(m_dragDifference) >= 1 && m_editMatrix->getMatrix()->getSize().y + m_dragDifference > 0) {
						p = m_editMatrix->getMatrix()->getSize().y;
						m_editMatrix->getMatrix()->setSize(m_editMatrix->getMatrix()->getSize() + glm::ivec3(0, m_dragDifference, 0));
						m_startPos.y += m_editMatrix->getMatrix()->getSize().y - p;
					}
				}
			}
			break;
		case FACE_WEST:
			p_camDir = (m_startPos - Camera::getPosition());
			p_camDir.z = 0;
			p_camDir = glm::normalize(p_camDir);
			denom = glm::dot(p_camDir, Camera::getMouseDirection());
			if(abs(denom) > 0.0001f) {
				dist = glm::dot(m_startPos - Camera::getPosition(), p_camDir) / denom;
				if(dist >= 0.0001f) {
					m_dragDifference = (Camera::getPosition() + Camera::getMouseDirection() * dist).z - m_startPos.z;
					if(abs(m_dragDifference) >= 1 && m_editMatrix->getMatrix()->getSize().z - m_dragDifference > 0) {
						p = m_editMatrix->getMatrix()->getPos().z;
						if(m_dragDifference > 0) m_editMatrix->getMatrix()->shiftVoxels(glm::ivec3(0, 0, -m_dragDifference));
						m_editMatrix->getMatrix()->setSize(m_editMatrix->getMatrix()->getSize() - glm::ivec3(0, 0, m_dragDifference));
						m_editMatrix->getMatrix()->addPosition(glm::ivec3(0, 0, m_dragDifference));
						if(m_dragDifference < 0) m_editMatrix->getMatrix()->shiftVoxels(glm::ivec3(0, 0, -m_dragDifference));
						m_startPos.z += m_editMatrix->getMatrix()->getPos().z - p;
					}
				}
			}
			break;
		case FACE_EAST:
			p_camDir = (m_startPos - Camera::getPosition());
			p_camDir.z = 0;
			p_camDir = glm::normalize(p_camDir);
			denom = glm::dot(p_camDir, Camera::getMouseDirection());
			if(abs(denom) > 0.0001f) {
				dist = glm::dot(m_startPos - Camera::getPosition(), p_camDir) / denom;
				if(dist >= 0.0001f) {
					m_dragDifference = (Camera::getPosition() + Camera::getMouseDirection() * dist).z - m_startPos.z;
					if(abs(m_dragDifference) >= 1 && m_editMatrix->getMatrix()->getSize().z + m_dragDifference > 0) {
						p = m_editMatrix->getMatrix()->getSize().z;
						m_editMatrix->getMatrix()->setSize(m_editMatrix->getMatrix()->getSize() + glm::ivec3(0, 0, m_dragDifference));
						m_startPos.z += m_editMatrix->getMatrix()->getSize().z - p;
					}
				}
			}
			break;
		}
	}
}
void MTSelect::renderResize() {
	glm::vec3 s = glm::vec3((*m_matrices)[m_editMatrix->getId()]->getSize()) / 2.f;
	glm::vec3 _offset = (*m_matrices)[m_editMatrix->getId()]->getPos() + s;

	Shader::pushMatrixModel();
	Shader::translate(glm::vec3(_offset));

	Shader::setColor(((*m_selectedScale == FACE_SOUTH || *m_selectedScale == FACE_NORTH) ? glm::vec4(1.0f, 0.25f, 0.25f, 1.0f) : glm::vec4(0.75f, 0.0f, 0.0f, 1.f)));
	Shader::pushMatrixModel();
	Shader::translate(glm::vec3(s.x, 0, 0));
	Shader::rotate(-90, glm::vec3(0, 0, 1));
	MModelObj::get("Scale.obj")->renderObj();
	Shader::popMatrixModel();

	Shader::pushMatrixModel();
	Shader::translate(glm::vec3(-s.x, 0, 0));
	Shader::rotate(90, glm::vec3(0, 0, 1));
	MModelObj::get("Scale.obj")->renderObj();
	Shader::popMatrixModel();

	Shader::setColor(((*m_selectedScale == FACE_BOTTOM || *m_selectedScale == FACE_TOP) ? glm::vec4(0.25f, 1.0f, 0.25f, 1.0f) : glm::vec4(0.0f, 0.75f, 0.0f, 1.f)));
	Shader::pushMatrixModel();
	Shader::translate(glm::vec3(0, s.y, 0));
	//Shader::rotate(0, glm::vec3(0, 0, 1));
	MModelObj::get("Scale.obj")->renderObj();
	Shader::popMatrixModel();

	Shader::pushMatrixModel();
	Shader::translate(glm::vec3(0, -s.y, 0));
	Shader::rotate(180, glm::vec3(0, 0, 1));
	MModelObj::get("Scale.obj")->renderObj();
	Shader::popMatrixModel();

	Shader::setColor(((*m_selectedScale == FACE_WEST || *m_selectedScale == FACE_EAST) ? glm::vec4(0.25f, 0.25f, 1.0f, 1.0f) : glm::vec4(0.0f, 0.0f, 0.75f, 1.f)));
	Shader::pushMatrixModel();
	Shader::translate(glm::vec3(0, 0, s.z));
	Shader::rotate(90, glm::vec3(1, 0, 0));
	MModelObj::get("Scale.obj")->renderObj();
	Shader::popMatrixModel();

	Shader::pushMatrixModel();
	Shader::translate(glm::vec3(0, 0, -s.z));
	Shader::rotate(-90, glm::vec3(1, 0, 0));
	MModelObj::get("Scale.obj")->renderObj();
	Shader::popMatrixModel();

	Shader::popMatrixModel();
}
