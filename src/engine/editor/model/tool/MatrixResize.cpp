#include "engine\editor\model\tool\MatrixResize.h"

EditMatrix *MatrixResize::m_matrix = 0;
Vector3<GLfloat> MatrixResize::m_startPos;
Sint8 MatrixResize::m_selectedScale = 0;


void MatrixResize::start(EditMatrix* p_matrix, Vector3<GLfloat> p_pos, Sint8 p_selectedScale)
{
	m_matrix = p_matrix;
	m_startPos = p_pos;
	m_selectedScale = p_selectedScale;
}
void MatrixResize::update(Vector3<GLfloat> p_pos, Vector3<GLfloat> p_camDir, Vector3<GLfloat> p_mouseDir)
{
	if(m_matrix)
	{
		Vector3<GLfloat> p_camDir;
		GLfloat dist, denom, p, m_dragDifference;
		denom = p_camDir.dot(p_mouseDir);
		switch(m_selectedScale)
		{
		case FACE_SOUTH:
			p_camDir = m_startPos - p_pos;
			p_camDir.x = 0;
			p_camDir = p_camDir.getNormal();
			denom = p_camDir.dot(p_mouseDir);
			if(abs(denom) > 0.0001f)
			{
				dist = (m_startPos - p_pos).dot(p_camDir) / denom;
				if(dist >= 0.0001f)
				{
					m_dragDifference = (p_pos + p_mouseDir * dist).x - m_startPos.x;
					if(abs(m_dragDifference) >= 1 && m_matrix->getMatrix()->getSize().x - m_dragDifference > 0)
					{
						p = m_matrix->getMatrix()->getPos().x;
						if(m_dragDifference > 0) m_matrix->getMatrix()->shiftVoxels(Vector3<Sint32>(-m_dragDifference, 0, 0));
						m_matrix->getMatrix()->setSize(m_matrix->getMatrix()->getSize() - Vector3<Sint32>(m_dragDifference, 0, 0));
						m_matrix->getMatrix()->addPosition(Vector3<Sint32>(m_dragDifference, 0, 0));
						if(m_dragDifference < 0) m_matrix->getMatrix()->shiftVoxels(Vector3<Sint32>(-m_dragDifference, 0, 0));
						m_startPos.x += m_matrix->getMatrix()->getPos().x - p;
					}
				}
			}
			break;
		case FACE_NORTH:
			p_camDir = m_startPos - p_pos;
			p_camDir.x = 0;
			p_camDir = p_camDir.getNormal();
			denom = p_camDir.dot(p_mouseDir);
			if(abs(denom) > 0.0001f)
			{
				dist = (m_startPos - p_pos).dot(p_camDir) / denom;
				if(dist >= 0.0001f)
				{
					m_dragDifference = (p_pos + p_mouseDir * dist).x - m_startPos.x;
					if(abs(m_dragDifference) >= 1 && m_matrix->getMatrix()->getSize().x + m_dragDifference > 0)
					{
						p = m_matrix->getMatrix()->getSize().x;
						m_matrix->getMatrix()->setSize(m_matrix->getMatrix()->getSize() + Vector3<Sint32>(m_dragDifference, 0, 0));
						m_startPos.x += m_matrix->getMatrix()->getSize().x - p;
					}
				}
			}
			break;
		case FACE_BOTTOM:
			p_camDir = m_startPos - p_pos;
			p_camDir.y = 0;
			p_camDir = p_camDir.getNormal();
			denom = p_camDir.dot(p_mouseDir);
			if(abs(denom) > 0.0001f)
			{
				dist = (m_startPos - p_pos).dot(p_camDir) / denom;
				if(dist >= 0.0001f)
				{
					m_dragDifference = (p_pos + p_mouseDir * dist).y - m_startPos.y;
					if(abs(m_dragDifference) >= 1 && m_matrix->getMatrix()->getSize().y - m_dragDifference > 0)
					{
						p = m_matrix->getMatrix()->getPos().y;
						if(m_dragDifference > 0) m_matrix->getMatrix()->shiftVoxels(Vector3<Sint32>(0, -m_dragDifference, 0));
						m_matrix->getMatrix()->setSize(m_matrix->getMatrix()->getSize() - Vector3<Sint32>(0, m_dragDifference, 0));
						m_matrix->getMatrix()->addPosition(Vector3<Sint32>(0, m_dragDifference, 0));
						if(m_dragDifference < 0) m_matrix->getMatrix()->shiftVoxels(Vector3<Sint32>(0, -m_dragDifference, 0));
						m_startPos.y += m_matrix->getMatrix()->getPos().y - p;
					}
				}
			}
			break;
		case FACE_TOP:
			p_camDir = m_startPos - p_pos;
			p_camDir.y = 0;
			p_camDir = p_camDir.getNormal();
			denom = p_camDir.dot(p_mouseDir);
			if(abs(denom) > 0.0001f)
			{
				dist = (m_startPos - p_pos).dot(p_camDir) / denom;
				if(dist >= 0.0001f)
				{
					m_dragDifference = (p_pos + p_mouseDir * dist).y - m_startPos.y;
					if(abs(m_dragDifference) >= 1 && m_matrix->getMatrix()->getSize().y + m_dragDifference > 0)
					{
						p = m_matrix->getMatrix()->getSize().y;
						m_matrix->getMatrix()->setSize(m_matrix->getMatrix()->getSize() + Vector3<Sint32>(0, m_dragDifference, 0));
						m_startPos.y += m_matrix->getMatrix()->getSize().y - p;
					}
				}
			}
			break;
		case FACE_WEST:
			p_camDir = (m_startPos - p_pos);
			p_camDir.z = 0;
			p_camDir = p_camDir.getNormal();
			denom = p_camDir.dot(p_mouseDir);
			if(abs(denom) > 0.0001f)
			{
				dist = (m_startPos - p_pos).dot(p_camDir) / denom;
				if(dist >= 0.0001f)
				{
					m_dragDifference = (p_pos + p_mouseDir * dist).z - m_startPos.z;
					if(abs(m_dragDifference) >= 1 && m_matrix->getMatrix()->getSize().z - m_dragDifference > 0)
					{
						p = m_matrix->getMatrix()->getPos().z;
						if(m_dragDifference > 0) m_matrix->getMatrix()->shiftVoxels(Vector3<Sint32>(0, 0, -m_dragDifference));
						m_matrix->getMatrix()->setSize(m_matrix->getMatrix()->getSize() - Vector3<Sint32>(0, 0, m_dragDifference));
						m_matrix->getMatrix()->addPosition(Vector3<Sint32>(0, 0, m_dragDifference));
						if(m_dragDifference < 0) m_matrix->getMatrix()->shiftVoxels(Vector3<Sint32>(0, 0, -m_dragDifference));
						m_startPos.z += m_matrix->getMatrix()->getPos().z - p;
					}
				}
			}
			break;
		case FACE_EAST:
			p_camDir = (m_startPos - p_pos);
			p_camDir.z = 0;
			p_camDir = p_camDir.getNormal();
			denom = p_camDir.dot(p_mouseDir);
			if(abs(denom) > 0.0001f)
			{
				dist = (m_startPos - p_pos).dot(p_camDir) / denom;
				if(dist >= 0.0001f)
				{
					m_dragDifference = (p_pos + p_mouseDir * dist).z - m_startPos.z;
					if(abs(m_dragDifference) >= 1 && m_matrix->getMatrix()->getSize().z + m_dragDifference > 0)
					{
						p = m_matrix->getMatrix()->getSize().z;
						m_matrix->getMatrix()->setSize(m_matrix->getMatrix()->getSize() + Vector3<Sint32>(0, 0, m_dragDifference));
						m_startPos.z += m_matrix->getMatrix()->getSize().z - p;
					}
				}
			}
			break;
		}
	}
}
void MatrixResize::end()
{
	if(m_matrix)
	{
		m_matrix->reset();
		m_matrix = 0;
	}
}
