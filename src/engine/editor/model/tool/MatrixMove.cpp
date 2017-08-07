#include "engine\editor\model\tool\MatrixMove.h"

EditMatrix *MatrixMove::m_matrix = 0;
Vector3<GLfloat> MatrixMove::m_startPos;
Sint8 MatrixMove::m_selectedScale = 0;

void MatrixMove::start(EditMatrix* p_matrix, Vector3<GLfloat> p_pos, Sint8 p_selectedScale)
{
	m_matrix = p_matrix;
	m_startPos = p_pos;
	m_selectedScale = p_selectedScale;
}

void MatrixMove::update(Vector3<GLfloat> p_pos, Vector3<GLfloat> p_dir)
{
	if(m_matrix)
	{
		Vector3<GLfloat> norm;
		GLfloat denom, dist, dragDiff;
		switch(m_selectedScale)
		{
		case FACE_SOUTH:
		case FACE_NORTH:
			norm = m_startPos - p_pos;
			norm.x = 0;
			norm = norm.getNormal();
			denom = norm.dot(p_dir);
			if(abs(denom) > 0.0001f)
			{
				dist = (m_startPos - p_pos).dot(norm) / denom;
				if(dist >= 0.0001f)
				{
					dragDiff = (p_pos + p_dir * dist).x - m_startPos.x;
					m_matrix->getMatrix()->setPosition(m_matrix->getPos() + Vector3<GLfloat>(dragDiff, 0, 0));
				}
			}
			break;
		case FACE_BOTTOM:
		case FACE_TOP:
			norm = m_startPos - p_pos;
			norm.y = 0;
			norm = norm.getNormal();
			denom = norm.dot(p_dir);
			if(abs(denom) > 0.0001f)
			{
				dist = (m_startPos - p_pos).dot(norm) / denom;
				if(dist >= 0.0001f)
				{
					dragDiff = (p_pos + p_dir * dist).y - m_startPos.y;
					m_matrix->getMatrix()->setPosition(m_matrix->getPos() + Vector3<GLfloat>(0, dragDiff, 0));
				}
			}
			break;
		case FACE_WEST:
		case FACE_EAST:
			norm = (m_startPos - p_pos);
			norm.z = 0;
			norm = norm.getNormal();
			denom = norm.dot(p_dir);
			if(abs(denom) > 0.0001f)
			{
				dist = (m_startPos - p_pos).dot(norm) / denom;
				if(dist >= 0.0001f)
				{
					dragDiff = (p_pos + p_dir * dist).z - m_startPos.z;
					m_matrix->getMatrix()->setPosition(m_matrix->getPos() + Vector3<GLfloat>(0, 0, dragDiff));
				}
			}
			break;
		}
	}
}

void MatrixMove::end()
{
	if(m_matrix)
	{
		m_matrix->reset();
		m_matrix = 0;
	}
}
