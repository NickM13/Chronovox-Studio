#include "engine\gfx\mesh\MMesh.h"

std::vector<TMeshObject*> MMesh::m_objectList;

void MMesh::init() {
	m_objectList.push_back(new MArrow());
	m_objectList.push_back(new MCube());
	m_objectList.push_back(new MPlane());
	m_objectList.push_back(new MScale());
	m_objectList.push_back(new MSCube());
}
void MMesh::terminate() {
	for(Uint16 i = 0; i < m_objectList.size(); i++) {
		delete m_objectList[i];
	}
	m_objectList.clear();
}
void MMesh::render(std::string p_objectName, Vector3<GLfloat> p_pos, Vector3<GLfloat> p_size, Vector3<GLfloat> p_rotation, Color p_color) {
	glBindTexture(GL_TEXTURE_2D, 0);
	glColor3f(1, 1, 1);

	TMeshObject* _obj = 0;
	for(Uint16 i = 0; i < m_objectList.size(); i++) {
		if(p_objectName == m_objectList[i]->m_objectName) {
			_obj = m_objectList[i];
			break;
		}
	}
	if(!_obj) return;

	if(p_size.x < 0) {
		p_size.x = -p_size.x;
		p_pos.x -= p_size.x;
	}
	if(p_size.y < 0) {
		p_size.y = -p_size.y;
		p_pos.y -= p_size.y;
	}
	if(p_size.z < 0) {
		p_size.z = -p_size.z;
		p_pos.z -= p_size.z;
	}

	int _lineCount = _obj->m_lineVertices.size() * 2,
		_triCount = _obj->m_triVertices.size() * 3,
		_quadCount = _obj->m_quadVertices.size() * 4;
	Color *_lineColors = new Color[_lineCount],
		*_triColors = new Color[_triCount],
		*_quadColors = new Color[_quadCount];
	for(int i = 0; i < _lineCount; i++)
		_lineColors[i] = p_color * _obj->m_lineShading[(int)floor(i / 2.f)];
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
		if(_lineCount > 0) {
			glColorPointer(4, GL_FLOAT, 0, &_lineColors[0].r);
			glVertexPointer(3, GL_FLOAT, 0, &_obj->m_lineVertices[0].x.x);
			glDrawArrays(GL_LINES, 0, _obj->m_lineVertices.size() * 2);
		}
		if(_triCount > 0) {
			glColorPointer(4, GL_FLOAT, 0, &_triColors[0].r);
			glVertexPointer(3, GL_FLOAT, 0, &_obj->m_triVertices[0].x.x);
			glDrawArrays(GL_TRIANGLES, 0, _obj->m_triVertices.size() * 3);
		}
		if(_quadCount > 0) {
			glColorPointer(4, GL_FLOAT, 0, &_quadColors[0].r);
			glVertexPointer(3, GL_FLOAT, 0, &_obj->m_quadVertices[0].x1.x);
			glDrawArrays(GL_QUADS, 0, _obj->m_quadVertices.size() * 4);
		}
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
	}
	glPopMatrix();

	delete[] _lineColors;
	delete[] _triColors;
	delete[] _quadColors;
}
