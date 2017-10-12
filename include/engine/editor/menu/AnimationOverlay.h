#pragma once

#include "engine\gfx\gui\Component.h"
#include "..\model\Model.h"

class AnimationOverlay : public Container
{
public:
	AnimationOverlay(Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Model* p_model)
		: Container("GUI_ANIMATION", p_pos, p_size, false)
	{
		m_model = p_model;
		addComponent(new Panel("BG", "Keyframe Editor", {}, p_size, PRIMARY, BORDER_ALL));
	}
private:
	Model *m_model;
};
