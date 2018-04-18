#include "engine\editor\model\tool\matrix\MTSelect.h"

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
		Vector3<GLfloat> norm;
		GLfloat denom, dist, dragDiff;
		switch(*m_selectedScale) {
		case FACE_SOUTH:
		case FACE_NORTH:
			norm = m_startPos - Camera::getPosition();
			norm.x = 0;
			norm = norm.getNormal();
			denom = norm.dot(Camera::getMouseDirection());
			if(abs(denom) > 0.0001f) {
				dist = (m_startPos - Camera::getPosition()).dot(norm) / denom;
				if(dist >= 0.0001f) {
					dragDiff = (Camera::getPosition() + Camera::getMouseDirection() * dist).x - m_startPos.x;
					m_editMatrix->getMatrix()->setPosition(m_editMatrix->getInitMatrix()->getPos() + Vector3<GLfloat>(dragDiff, 0, 0));
				}
			}
			break;
		case FACE_BOTTOM:
		case FACE_TOP:
			norm = m_startPos - Camera::getPosition();
			norm.y = 0;
			norm = norm.getNormal();
			denom = norm.dot(Camera::getMouseDirection());
			if(abs(denom) > 0.0001f) {
				dist = (m_startPos - Camera::getPosition()).dot(norm) / denom;
				if(dist >= 0.0001f) {
					dragDiff = (Camera::getPosition() + Camera::getMouseDirection() * dist).y - m_startPos.y;
					m_editMatrix->getMatrix()->setPosition(m_editMatrix->getInitMatrix()->getPos() + Vector3<GLfloat>(0, dragDiff, 0));
				}
			}
			break;
		case FACE_WEST:
		case FACE_EAST:
			norm = (m_startPos - Camera::getPosition());
			norm.z = 0;
			norm = norm.getNormal();
			denom = norm.dot(Camera::getMouseDirection());
			if(abs(denom) > 0.0001f) {
				dist = (m_startPos - Camera::getPosition()).dot(norm) / denom;
				if(dist >= 0.0001f) {
					dragDiff = (Camera::getPosition() + Camera::getMouseDirection() * dist).z - m_startPos.z;
					m_editMatrix->getMatrix()->setPosition(m_editMatrix->getInitMatrix()->getPos() + Vector3<GLfloat>(0, 0, dragDiff));
				}
			}
			break;
		}
	}
}
void MTSelect::renderMove() {
	Vector3<GLfloat> s = Vector3<GLfloat>((*m_matrices)[m_editMatrix->getId()]->getSize()) / 2.f;
	Vector3<GLfloat> _offset = (*m_matrices)[m_editMatrix->getId()]->getPos() + s;
	glPushMatrix();
	{
		glTranslatef(_offset.x, _offset.y, _offset.z);

		MMesh::render("Arrow", {-s.x, 0, 0}, {0.1f, 2, 0.1f}, {0, 0, 90}, (*m_selectedScale == FACE_SOUTH ? Color(1, 0.25f, 0.25f) : Color(0.75f, 0, 0)));
		MMesh::render("Arrow", {s.x, 0, 0}, {0.1f, 2, 0.1f}, {0, 0, -90}, (*m_selectedScale == FACE_NORTH ? Color(1, 0.25f, 0.25f) : Color(0.75f, 0, 0)));

		MMesh::render("Arrow", {0, -s.y, 0}, {0.1f, 2, 0.1f}, {180, 0, 0}, (*m_selectedScale == FACE_BOTTOM ? Color(0.25f, 1, 0.25f) : Color(0, 0.75f, 0)));
		MMesh::render("Arrow", {0, s.y, 0}, {0.1f, 2, 0.1f}, {0, 0, 0}, (*m_selectedScale == FACE_TOP ? Color(0.25f, 1, 0.25f) : Color(0, 0.75f, 0)));

		MMesh::render("Arrow", {0, 0, -s.z}, {0.1f, 2, 0.1f}, {-90, 0, 0}, (*m_selectedScale == FACE_WEST ? Color(0.25f, 0.25f, 1) : Color(0, 0, 0.75f)));
		MMesh::render("Arrow", {0, 0, s.z}, {0.1f, 2, 0.1f}, {90, 0, 0}, (*m_selectedScale == FACE_EAST ? Color(0.25f, 0.25f, 1) : Color(0, 0, 0.75f)));
	}
	glPopMatrix();
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
		Vector3<GLfloat> p_camDir;
		GLfloat dist, denom, p, m_dragDifference;
		denom = p_camDir.dot(Camera::getMouseDirection());
		switch(*m_selectedScale) {
		case FACE_SOUTH:
			p_camDir = m_startPos - Camera::getPosition();
			p_camDir.x = 0;
			p_camDir = p_camDir.getNormal();
			denom = p_camDir.dot(Camera::getMouseDirection());
			if(abs(denom) > 0.0001f) {
				dist = (m_startPos - Camera::getPosition()).dot(p_camDir) / denom;
				if(dist >= 0.0001f) {
					m_dragDifference = (Camera::getPosition() + Camera::getMouseDirection() * dist).x - m_startPos.x;
					if(abs(m_dragDifference) >= 1 && m_editMatrix->getMatrix()->getSize().x - m_dragDifference > 0) {
						p = m_editMatrix->getMatrix()->getPos().x;
						if(m_dragDifference > 0) m_editMatrix->getMatrix()->shiftVoxels(Vector3<Sint32>(-m_dragDifference, 0, 0));
						m_editMatrix->getMatrix()->setSize(m_editMatrix->getMatrix()->getSize() - Vector3<Sint32>(m_dragDifference, 0, 0));
						m_editMatrix->getMatrix()->addPosition(Vector3<Sint32>(m_dragDifference, 0, 0));
						if(m_dragDifference < 0) m_editMatrix->getMatrix()->shiftVoxels(Vector3<Sint32>(-m_dragDifference, 0, 0));
						m_startPos.x += m_editMatrix->getMatrix()->getPos().x - p;
					}
				}
			}
			break;
		case FACE_NORTH:
			p_camDir = m_startPos - Camera::getPosition();
			p_camDir.x = 0;
			p_camDir = p_camDir.getNormal();
			denom = p_camDir.dot(Camera::getMouseDirection());
			if(abs(denom) > 0.0001f) {
				dist = (m_startPos - Camera::getPosition()).dot(p_camDir) / denom;
				if(dist >= 0.0001f) {
					m_dragDifference = (Camera::getPosition() + Camera::getMouseDirection() * dist).x - m_startPos.x;
					if(abs(m_dragDifference) >= 1 && m_editMatrix->getMatrix()->getSize().x + m_dragDifference > 0) {
						p = m_editMatrix->getMatrix()->getSize().x;
						m_editMatrix->getMatrix()->setSize(m_editMatrix->getMatrix()->getSize() + Vector3<Sint32>(m_dragDifference, 0, 0));
						m_startPos.x += m_editMatrix->getMatrix()->getSize().x - p;
					}
				}
			}
			break;
		case FACE_BOTTOM:
			p_camDir = m_startPos - Camera::getPosition();
			p_camDir.y = 0;
			p_camDir = p_camDir.getNormal();
			denom = p_camDir.dot(Camera::getMouseDirection());
			if(abs(denom) > 0.0001f) {
				dist = (m_startPos - Camera::getPosition()).dot(p_camDir) / denom;
				if(dist >= 0.0001f) {
					m_dragDifference = (Camera::getPosition() + Camera::getMouseDirection() * dist).y - m_startPos.y;
					if(abs(m_dragDifference) >= 1 && m_editMatrix->getMatrix()->getSize().y - m_dragDifference > 0) {
						p = m_editMatrix->getMatrix()->getPos().y;
						if(m_dragDifference > 0) m_editMatrix->getMatrix()->shiftVoxels(Vector3<Sint32>(0, -m_dragDifference, 0));
						m_editMatrix->getMatrix()->setSize(m_editMatrix->getMatrix()->getSize() - Vector3<Sint32>(0, m_dragDifference, 0));
						m_editMatrix->getMatrix()->addPosition(Vector3<Sint32>(0, m_dragDifference, 0));
						if(m_dragDifference < 0) m_editMatrix->getMatrix()->shiftVoxels(Vector3<Sint32>(0, -m_dragDifference, 0));
						m_startPos.y += m_editMatrix->getMatrix()->getPos().y - p;
					}
				}
			}
			break;
		case FACE_TOP:
			p_camDir = m_startPos - Camera::getPosition();
			p_camDir.y = 0;
			p_camDir = p_camDir.getNormal();
			denom = p_camDir.dot(Camera::getMouseDirection());
			if(abs(denom) > 0.0001f) {
				dist = (m_startPos - Camera::getPosition()).dot(p_camDir) / denom;
				if(dist >= 0.0001f) {
					m_dragDifference = (Camera::getPosition() + Camera::getMouseDirection() * dist).y - m_startPos.y;
					if(abs(m_dragDifference) >= 1 && m_editMatrix->getMatrix()->getSize().y + m_dragDifference > 0) {
						p = m_editMatrix->getMatrix()->getSize().y;
						m_editMatrix->getMatrix()->setSize(m_editMatrix->getMatrix()->getSize() + Vector3<Sint32>(0, m_dragDifference, 0));
						m_startPos.y += m_editMatrix->getMatrix()->getSize().y - p;
					}
				}
			}
			break;
		case FACE_WEST:
			p_camDir = (m_startPos - Camera::getPosition());
			p_camDir.z = 0;
			p_camDir = p_camDir.getNormal();
			denom = p_camDir.dot(Camera::getMouseDirection());
			if(abs(denom) > 0.0001f) {
				dist = (m_startPos - Camera::getPosition()).dot(p_camDir) / denom;
				if(dist >= 0.0001f) {
					m_dragDifference = (Camera::getPosition() + Camera::getMouseDirection() * dist).z - m_startPos.z;
					if(abs(m_dragDifference) >= 1 && m_editMatrix->getMatrix()->getSize().z - m_dragDifference > 0) {
						p = m_editMatrix->getMatrix()->getPos().z;
						if(m_dragDifference > 0) m_editMatrix->getMatrix()->shiftVoxels(Vector3<Sint32>(0, 0, -m_dragDifference));
						m_editMatrix->getMatrix()->setSize(m_editMatrix->getMatrix()->getSize() - Vector3<Sint32>(0, 0, m_dragDifference));
						m_editMatrix->getMatrix()->addPosition(Vector3<Sint32>(0, 0, m_dragDifference));
						if(m_dragDifference < 0) m_editMatrix->getMatrix()->shiftVoxels(Vector3<Sint32>(0, 0, -m_dragDifference));
						m_startPos.z += m_editMatrix->getMatrix()->getPos().z - p;
					}
				}
			}
			break;
		case FACE_EAST:
			p_camDir = (m_startPos - Camera::getPosition());
			p_camDir.z = 0;
			p_camDir = p_camDir.getNormal();
			denom = p_camDir.dot(Camera::getMouseDirection());
			if(abs(denom) > 0.0001f) {
				dist = (m_startPos - Camera::getPosition()).dot(p_camDir) / denom;
				if(dist >= 0.0001f) {
					m_dragDifference = (Camera::getPosition() + Camera::getMouseDirection() * dist).z - m_startPos.z;
					if(abs(m_dragDifference) >= 1 && m_editMatrix->getMatrix()->getSize().z + m_dragDifference > 0) {
						p = m_editMatrix->getMatrix()->getSize().z;
						m_editMatrix->getMatrix()->setSize(m_editMatrix->getMatrix()->getSize() + Vector3<Sint32>(0, 0, m_dragDifference));
						m_startPos.z += m_editMatrix->getMatrix()->getSize().z - p;
					}
				}
			}
			break;
		}
	}
}
void MTSelect::renderResize() {
	Vector3<GLfloat> s = Vector3<GLfloat>((*m_matrices)[m_editMatrix->getId()]->getSize()) / 2.f;
	Vector3<GLfloat> _offset = (*m_matrices)[m_editMatrix->getId()]->getPos() + s;
	glPushMatrix();
	{
		glTranslatef(_offset.x, _offset.y, _offset.z);

		MMesh::render("Scale", {-s.x, 0, 0}, {0.1f, 2, 0.1f}, {0, 0, 90}, (*m_selectedScale == FACE_SOUTH ? Color(1, 0.25f, 0.25f) : Color(0.75f, 0, 0)));
		MMesh::render("Scale", {s.x, 0, 0}, {0.1f, 2, 0.1f}, {0, 0, -90}, (*m_selectedScale == FACE_NORTH ? Color(1, 0.25f, 0.25f) : Color(0.75f, 0, 0)));

		MMesh::render("Scale", {0, -s.y, 0}, {0.1f, 2, 0.1f}, {180, 0, 0}, (*m_selectedScale == FACE_BOTTOM ? Color(0.25f, 1, 0.25f) : Color(0, 0.75f, 0)));
		MMesh::render("Scale", {0, s.y, 0}, {0.1f, 2, 0.1f}, {0, 0, 0}, (*m_selectedScale == FACE_TOP ? Color(0.25f, 1, 0.25f) : Color(0, 0.75f, 0)));

		MMesh::render("Scale", {0, 0, -s.z}, {0.1f, 2, 0.1f}, {-90, 0, 0}, (*m_selectedScale == FACE_WEST ? Color(0.25f, 0.25f, 1) : Color(0, 0, 0.75f)));
		MMesh::render("Scale", {0, 0, s.z}, {0.1f, 2, 0.1f}, {90, 0, 0}, (*m_selectedScale == FACE_EAST ? Color(0.25f, 0.25f, 1) : Color(0, 0, 0.75f)));
	}
	glPopMatrix();
}
