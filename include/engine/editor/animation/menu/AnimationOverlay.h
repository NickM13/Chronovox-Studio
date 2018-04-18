#pragma once
#include "engine\editor\animation\Animation.h"
#include "engine\gfx\LGui.h"
#include "KeyframeTimeline.h"

class AnimationOverlay {
private:
	static Animation* m_animation;
	static Container* m_container;
public:
	static Container* init(Animation* p_model);
	static void terminate() {
		if(m_container) {
			delete m_container;
			m_container = 0;
		}
	}
	static Container* getContainer() { return m_container; }
	~AnimationOverlay() { terminate(); }
};
