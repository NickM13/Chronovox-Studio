#pragma once

#include "engine\gfx\LGui.h"
#include "engine\utils\global\GScreen.h"
#include "..\model\menu\ColorOverlay.h"
#include "..\Editor.h"

class EditorOverlay {
private:
	static Editor* m_editor;
	static Container* m_container;
public:
	static Container* init(Editor* p_editor);
	static void terminate() {
		if(m_container) {
			delete m_container;
			m_container = 0;
		}
	}
	static Container* getContainer() { return m_container; }
	~EditorOverlay() { terminate(); }
};
