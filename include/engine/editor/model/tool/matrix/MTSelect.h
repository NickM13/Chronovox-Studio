#pragma once
#include "MatrixTool.h"

class MTSelect : public MatrixTool {
private:
	struct MoveMatrix {
		Matrix* matrix;
		Vector3<GLfloat> pos;
	};
	std::vector<MoveMatrix> m_moveMatrices;

	void inputSelect();
	void updateSelect();
	void renderSelect();

	void inputMove();
	void updateMove();
	void renderMove();

	void inputResize();
	void updateResize();
	void renderResize();
public:
	MTSelect();
};
