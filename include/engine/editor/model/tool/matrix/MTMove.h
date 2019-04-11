#pragma once
#include "MatrixTool.h"

class MTMove : public MatrixTool {
private:
	struct MoveMatrix {
		Matrix* matrix;
		glm::vec3 pos;
	};
	std::vector<MoveMatrix> m_moveMatrices;

	void inputTool();
	void updateTool();
	void renderTool();
public:
	MTMove();
};
