#include "engine\editor\model\tool\Box.h"

Matrix *Box::m_matrix = 0, *Box::m_initMatrix = 0;
Voxel Box::m_voxel;
Vector3<Sint32> Box::m_startPos, Box::m_lastPos;
bool Box::m_replace = false;

void Box::start(Matrix* p_matrix, Vector3<Sint32> p_pos, Voxel p_voxel, bool p_replace)
{
	if(p_pos.x >= 0 && p_pos.y >= 0 && p_pos.z >= 0 && p_pos.x < p_matrix->getSize().x && p_pos.y < p_matrix->getSize().y && p_pos.z < p_matrix->getSize().z)
	{
		m_matrix = p_matrix;
		m_initMatrix = new Matrix(*p_matrix);
		m_startPos = p_pos;
		m_lastPos = m_startPos;
		m_voxel = p_voxel;
		m_replace = p_replace;
	}
}
void Box::update(Vector3<Sint32> p_pos)
{
	if(m_matrix != 0 && !(p_pos == Vector3<Sint32>(-1, -1, -1)))
	{
		Vector3<Sint32> s = m_matrix->getSize();
		p_pos = Vector3<Sint32>(max(0, min(p_pos.x, s.x - 1)), max(0, min(p_pos.y, s.y - 1)), max(0, min(p_pos.z, s.z - 1)));

		Vector3<Sint32> start;
		Vector3<Sint32> end;

		start = Vector3<Sint32>(max(0, min(m_startPos.x, m_lastPos.x)), max(0, min(m_startPos.y, m_lastPos.y)), max(0, min(m_startPos.z, m_lastPos.z)));
		end = Vector3<Sint32>(min(s.x - 1, max(m_startPos.x, m_lastPos.x)), min(s.y - 1, max(m_startPos.y, m_lastPos.y)), min(s.z - 1, max(m_startPos.z, m_lastPos.z)));

		for(Sint32 x = start.x; x <= end.x; x++)
			for(Sint32 y = start.y; y <= end.y; y++)
				for(Sint32 z = start.z; z <= end.z; z++)
					m_matrix->setVoxel({x, y, z}, m_initMatrix->getVoxel(Vector3<Sint32>(x, y, z)));



		start = Vector3<Sint32>(min(m_startPos.x, p_pos.x), min(m_startPos.y, p_pos.y), min(m_startPos.z, p_pos.z));
		end = Vector3<Sint32>(max(m_startPos.x, p_pos.x), max(m_startPos.y, p_pos.y), max(m_startPos.z, p_pos.z));

		if(m_replace)
		{
			Voxel match = m_initMatrix->getVoxel(m_startPos);
			for(Sint32 x = start.x; x <= end.x; x++)
				for(Sint32 y = start.y; y <= end.y; y++)
					for(Sint32 z = start.z; z <= end.z; z++)
						if(match == m_initMatrix->getVoxel({x, y, z}))
							m_matrix->setVoxel({x, y, z}, m_voxel);
		}
		else
		{
			for(Sint32 x = start.x; x <= end.x; x++)
				for(Sint32 y = start.y; y <= end.y; y++)
					for(Sint32 z = start.z; z <= end.z; z++)
						m_matrix->setVoxel({x, y, z}, m_voxel);
		}

		m_lastPos = p_pos;
	}
}
void Box::end()
{
	if(m_matrix != 0)
	{
		m_matrix = 0;
		delete m_initMatrix;
	}
}

void Box::render()
{
	GLfloat c = 0.0075f;
	Vector3<GLfloat> s = m_matrix->getSize();
	Vector3<GLfloat> start = Vector3<GLfloat>(max(0, min(m_startPos.x, m_lastPos.x)), max(0, min(m_startPos.y, m_lastPos.y)), max(0, min(m_startPos.z, m_lastPos.z))) - c;
	Vector3<GLfloat> end = Vector3<GLfloat>(min(s.x - 1, max(m_startPos.x, m_lastPos.x)), min(s.y - 1, max(m_startPos.y, m_lastPos.y)), min(s.z - 1, max(m_startPos.z, m_lastPos.z))) + 1 + c;
	glColor4f(0.65f, 0.65f, 1, 1);
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
