#pragma once

#include "engine\gfx\LGui.h"
#include "engine\utils\global\GScreen.h"
#include "engine\editor\model\Model.h"
#include "AnimationOverlay.h"
#include "ColorOverlay.h"
#include "ResizeMatrix.h"

class Overlay
{
private:
	static Model* m_model;
	static Container* m_container;
	static Sint32 *r, *g, *b;
	static Sint32 *hue, *sat, *val;
	static Sint32 *w, *h, *d;
	static Sint32 *x, *y, *z;
	static Uint16 *toolbar, *toolbarMeta;
public:
	static Container* init(Model* p_model);
	static void clear() {
		if(m_container) {
			delete m_container;
			m_container = 0;
		}
	}
	static Container* getContainer() { return m_container; }
	~Overlay() { clear(); }
};
