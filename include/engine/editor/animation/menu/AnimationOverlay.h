#pragma once
#include "engine\editor\animation\Animation.h"
#include "engine\editor\Editor.h"
#include "engine\gfx\LGui.h"
#include "KeyframeTimeline.h"

class AnimationOverlay {
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
	~AnimationOverlay() { terminate(); }
};
