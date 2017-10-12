#include "engine\editor\model\tool\Select.h"

Matrix *Select::m_matrix = 0;
Vector3<Sint32> Select::m_startPos, Select::m_lastPos;

void Select::start(Matrix* p_matrix, Vector3<Sint32> p_pos) {
	if(p_pos.x >= 0 && p_pos.y >= 0 && p_pos.z >= 0 && p_pos.x < p_matrix->getSize().x && p_pos.y < p_matrix->getSize().y && p_pos.z < p_matrix->getSize().z) {
		m_matrix = p_matrix;
		m_startPos = m_lastPos = p_pos;
	}
}
void Select::update(Vector3<Sint32> p_pos) {
	if(m_matrix != 0 && !(p_pos == Vector3<Sint32>(-1, -1, -1))) {
		Vector3<Sint32> s = m_matrix->getSize();
		p_pos = Vector3<Sint32>(max(0, min(p_pos.x, s.x - 1)), max(0, min(p_pos.y, s.y - 1)), max(0, min(p_pos.z, s.z - 1)));
		m_lastPos = p_pos;
	}
}
void Select::end() {
	m_matrix = 0;
}

void Select::render() {
	if(!m_matrix) return;
	GLfloat c = 0.0075f;
	Vector3<GLfloat> s = m_matrix->getSize();
	Vector3<GLfloat> start = Vector3<GLfloat>(max(0, min(m_startPos.x, m_lastPos.x)), max(0, min(m_startPos.y, m_lastPos.y)), max(0, min(m_startPos.z, m_lastPos.z))) - c;
	Vector3<GLfloat> end = Vector3<GLfloat>(min(s.x - 1, max(m_startPos.x, m_lastPos.x)), min(s.y - 1, max(m_startPos.y, m_lastPos.y)), min(s.z - 1, max(m_startPos.z, m_lastPos.z))) + 1 + c;
	glColor4f(0.65f, 0.65f, 1, 1);
	glPushMatrix();
	{
		glBegin(GL_LINES);
		{
			glVertex3f(start.x, start.y, start.z);
			glVertex3f(start.x, end.y, start.z);

			glVertex3f(end.x, start.y, start.z);
			glVertex3f(end.x, end.y, start.z);

			glVertex3f(end.x, start.y, end.z);
			glVertex3f(end.x, end.y, end.z);

			glVertex3f(start.x, start.y, end.z);
			glVertex3f(start.x, end.y, end.z);

			glVertex3f(start.x, start.y, start.z);
			glVertex3f(end.x, start.y, start.z);

			glVertex3f(start.x, start.y, end.z);
			glVertex3f(end.x, start.y, end.z);

			glVertex3f(start.x, end.y, start.z);
			glVertex3f(end.x, end.y, start.z);

			glVertex3f(start.x, end.y, end.z);
			glVertex3f(end.x, end.y, end.z);

			glVertex3f(start.x, start.y, start.z);
			glVertex3f(start.x, start.y, end.z);

			glVertex3f(end.x, start.y, start.z);
			glVertex3f(end.x, start.y, end.z);

			glVertex3f(end.x, end.y, start.z);
			glVertex3f(end.x, end.y, end.z);

			glVertex3f(start.x, end.y, start.z);
			glVertex3f(start.x, end.y, end.z);
		}
		glEnd();
	}
	glPopMatrix();
}
