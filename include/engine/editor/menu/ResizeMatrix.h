#pragma once

#include "engine\gfx\LGui.h"
#include "engine\editor\model\Matrix.h"

class ResizeMatrix {
private:
	static ContainerPanel m_component;
	static Matrix *m_matrix;
public:
	static void init() {

	}

	static void open(Matrix *p_matrix) {
		m_matrix = p_matrix;
	}

	

	static void close() {
		m_matrix = 0;
	}
};
