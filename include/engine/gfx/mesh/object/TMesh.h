#pragma once

#include "engine\utils\OpenGL.h"
#include "engine\utils\variable\datatype\Color.h"
#include "engine\utils\variable\datatype\Macros.h"

#include <vector>

#include "TMeshObject.h"
#include "MArrow.h"
#include "MCube.h"
#include "MScale.h"

// Static single voxel rendering
class TMesh
{
public:
	static void init()
	{
		if(!m_initialized)
		{
			m_initialized = true;
			m_objectList.push_back(new MArrow());
			m_objectList.push_back(new MCube());
			m_objectList.push_back(new MScale());
		}
	}
	static void clear()
	{
		for(Uint16 i = 0; i < m_objectList.size(); i++)
		{
			delete m_objectList[i];
		}
		m_objectList.clear();
	}
	static void render(std::string p_objectName, Vector3<GLfloat> p_pos, Vector3<GLfloat> p_size, Vector3<GLfloat> p_rotation, Color p_color)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		glColor3f(1, 1, 1);

		TMeshObject* _obj = 0;
		for(Uint16 i = 0; i < m_objectList.size(); i++)
		{
			if(p_objectName == m_objectList[i]->m_objectName)
			{
				_obj = m_objectList[i];
				break;
			}
		}
		if(!_obj) return;

		if(p_size.x < 0)
		{
			p_size.x = -p_size.x;
			p_pos.x -= p_size.x;
		}
		if(p_size.y < 0)
		{
			p_size.y = -p_size.y;
			p_pos.y -= p_size.y;
		}
		if(p_size.z < 0)
		{
			p_size.z = -p_size.z;
			p_pos.z -= p_size.z;
		}

		int _triCount = _obj->m_triVertices.size() * 3, _quadCount = _obj->m_quadVertices.size() * 4;
		Color *_triColors = new Color[_triCount], *_quadColors = new Color[_quadCount];
		for(int i = 0; i < _triCount; i++)
			_triColors[i] = p_color * _obj->m_triShading[(int)floor(i / 3.f)];
		for(int i = 0; i < _quadCount; i++)
			_quadColors[i] = p_color * _obj->m_quadShading[(int)floor(i / 4.f)];

		glPushMatrix();
		{
			glTranslatef(p_pos.x, p_pos.y, p_pos.z);
			glRotatef(p_rotation.x, 1, 0, 0);
			glRotatef(p_rotation.y, 0, 1, 0);
			glRotatef(p_rotation.z, 0, 0, 1);
			glScalef(p_size.x, p_size.y, p_size.z);
			glEnableClientState(GL_COLOR_ARRAY);
			glEnableClientState(GL_VERTEX_ARRAY);
			if(_triCount > 0)
			{
				glColorPointer(4, GL_FLOAT, 0, &_triColors[0].r);
				glVertexPointer(3, GL_FLOAT, 0, &_obj->m_triVertices[0].x.x);
				glDrawArrays(GL_TRIANGLES, 0, _obj->m_triVertices.size() * 3);
			}
			if(_quadCount > 0)
			{
				glColorPointer(4, GL_FLOAT, 0, &_quadColors[0].r);
				glVertexPointer(3, GL_FLOAT, 0, &_obj->m_quadVertices[0].x1.x);
				glDrawArrays(GL_QUADS, 0, _obj->m_quadVertices.size() * 4);
			}
			glDisableClientState(GL_COLOR_ARRAY);
			glDisableClientState(GL_VERTEX_ARRAY);
		}
		glPopMatrix();

		delete[] _triColors;
		delete[] _quadColors;
	}

protected:
	static std::vector<TMeshObject*> m_objectList;
	static bool m_initialized;
};
