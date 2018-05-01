#include "engine\editor\model\tool\matrix\MatrixTool.h"

std::vector<Matrix*>* MatrixTool::m_matrices;
glm::vec3* MatrixTool::m_scalePos, MatrixTool::m_startPos;
Sint8* MatrixTool::m_selectedScale;

MatrixTool::MatrixTool() {
	m_toolType = ToolType::MATRIX;
}
MatrixTool::~MatrixTool() {

}

void MatrixTool::init(std::vector<Matrix*>* p_matrices, glm::vec3* p_scalePos, Sint8* p_selectedScale) {
	m_matrices = p_matrices;
	m_scalePos = p_scalePos;
	m_selectedScale = p_selectedScale;
}

void MatrixTool::enable() {
	m_scaling = false;
}
void MatrixTool::disable() {
	m_editMatrix->reset();
}

void MatrixTool::inputSelect() {}
void MatrixTool::updateSelect() {}
void MatrixTool::renderSelect() {}

void MatrixTool::inputMove() {}
void MatrixTool::updateMove() {}
void MatrixTool::renderMove() {}

void MatrixTool::inputResize() {}
void MatrixTool::updateResize() {}
void MatrixTool::renderResize() {}

void MatrixTool::input() {
	if(m_editMatrix->getId() == -1) return;
	switch(*m_subTool) {
	case 0: inputSelect(); break;
	case 1: inputMove(); break;
	case 2: inputResize(); break;
	}
}

void MatrixTool::update() {
	if(m_editMatrix->getId() == -1) return;
	switch(*m_subTool) {
	case 0: updateSelect(); break;
	case 1: updateMove(); break;
	case 2: updateResize(); break;
	}
}

void MatrixTool::render() {
	if(m_editMatrix->getId() == -1) return;
	switch(*m_subTool) {
	case 0: renderSelect(); break;
	case 1: renderMove(); break;
	case 2: renderResize(); break;
	}
}
