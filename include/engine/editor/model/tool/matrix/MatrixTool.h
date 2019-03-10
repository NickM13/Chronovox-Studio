#pragma once
#include "..\Tool.h"
#include "..\..\EditMatrix.h"
#include "engine\editor\camera\Camera.h"

class MatrixTool : public Tool {
protected:
	static std::vector<Matrix*>* m_matrices;
	static glm::vec3* m_scalePos, m_startPos;
	static Sint8* m_selectedScale;
	bool m_scaling;

	virtual void inputSelect();
	virtual void updateSelect();
	virtual void renderSelect();

	virtual void inputMove();
	virtual void updateMove();
	virtual void renderMove();

	virtual void inputResize();
	virtual void updateResize();
	virtual void renderResize();
public:
	MatrixTool();
	~MatrixTool();

	static void init(std::vector<Matrix*>* p_matrices, glm::vec3* p_scalePos, Sint8* p_selectedScale);

	virtual void enable();
	virtual void disable();

	void input();
	void update();
	void render();
};
