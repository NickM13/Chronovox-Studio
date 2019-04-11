#pragma once
#include "MatrixTool.h"

class MTResize : public MatrixTool {
private:
	void inputTool();
	void updateTool();
	void renderTool();
public:
	MTResize();
};
