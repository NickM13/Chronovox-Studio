#pragma once
#include "engine\editor\model\Model.h"
#include "engine\gfx\LGui.h"

#include "ColorOverlay.h"

class ModelOverlay {
private:
	static Model* m_model;
	static Container* m_container;
	static ColorOverlay* m_colorOverlay;
public:
	static Container* init(Model* p_model);
	static void terminate() {
		if(m_container) {
			delete m_container;
			m_container = 0;
		}
	}
	static Container* getContainer() { return m_container; }
	~ModelOverlay() { terminate(); }
};
