#pragma once
#include "..\EditMatrix.h"

class MatrixResize
{
public:
	static void start(EditMatrix* p_matrix, Vector3<GLfloat> p_pos, Sint8 p_selectedScale);
	static void update(Vector3<GLfloat> p_pos, Vector3<GLfloat> p_camDir, Vector3<GLfloat> p_mouseDir);
	static void end();
private:
	static EditMatrix* m_matrix;
	static Vector3<GLfloat> m_startPos;
	static Sint8 m_selectedScale;
};
