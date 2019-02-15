#pragma once
#include "engine\editor\model\Model.h"
#include "engine\editor\Editor.h"
#include "engine\gfx\LGui.h"

#include "ColorOverlay.h"

class ModelOverlay {
private:
	static Editor* m_editor;
	static Container* m_container;
	static ColorOverlay* m_colorOverlay;
public:
	static Container* init(Editor* p_editor);
	static void terminate() {
		if(m_container) {
			delete m_container;
			m_container = 0;
		}
	}
	static Container* getContainer() { return m_container; }
	~ModelOverlay() { terminate(); }
};
