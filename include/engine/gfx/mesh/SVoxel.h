#pragma once

#include "engine\utils\OpenGL.h"
#include "engine\utils\variable\datatype\Vector3.h"
#include "engine\utils\variable\datatype\Vector4.h"

// Static single voxel rendering
class SVoxel
{
public:
	static void render(Vector3<GLfloat> p_pos, Vector3<GLfloat> p_size, Vector4<GLfloat> p_color)
	{
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

		Vector4<GLfloat> _colors[24];
		for(int i = 0; i < 24; i++)
			_colors[i] = p_color * colors[(int)floor(i / 4.f)];

		glPushMatrix();
		{
			glTranslatef(p_pos.x, p_pos.y, p_pos.z);
			glScalef(p_size.x, p_size.y, p_size.z);
			glEnableClientState(GL_COLOR_ARRAY);
			glEnableClientState(GL_VERTEX_ARRAY);
			glColorPointer(4, GL_FLOAT, 0, &_colors[0].x1);
			glVertexPointer(3, GL_FLOAT, 0, &vertices[0].x);
			glDrawArrays(GL_QUADS, 0, 24);
			glDisableClientState(GL_COLOR_ARRAY);
			glDisableClientState(GL_VERTEX_ARRAY);
		}
		glPopMatrix();
	}

private:
	static Vector3<GLfloat> vertices[24];

	static GLfloat colors[6];
};
