#pragma once
#include "engine\gfx\gui\LGui.h"
#include "..\keyframe\Keyframe.h"

class KeyframeTimeline : public Container {
private:
	bool m_playing, m_holding;
	Sint32 m_length, m_time;
	// Im not sure why I used V
	struct VKeyframe {
		Keyframe* keyframe;
		Sint8 layer;
	};
	std::vector<VKeyframe> m_keyframes;
	Sint8 m_maxLayer;
	Sint32 m_hoveredKeyframe;
	Sint32 m_selectedKeyframe;

	Rect m_keyframeArea;
	GLfloat m_keyframeHeight;

	Color m_translateColor, m_rotateColor, m_scaleColor;
public:
	KeyframeTimeline(Vector2<Sint32> p_pos, Vector2<Sint32> p_size);

	void resize();

	void setPlaying(bool p_playing) { m_playing = p_playing; }
	bool isPlaying() { return m_playing; }
	void setLength(Sint32 p_length);
	void setTime(Sint32 p_time);
	Sint32 getTime() { return m_time; }

	void setHoveredKeyframe(Sint32 i) { m_hoveredKeyframe = i; }
	Sint32 getHoveredKeyframe() { return m_hoveredKeyframe; }
	void setSelectedKeyframe(Sint32 i) { m_selectedKeyframe = i; }
	Sint32 getSelectedKeyframe() { return m_selectedKeyframe; }

	void addKeyframe(Keyframe* p_keyframe);
	void terminate();

	void input(Sint8& p_interactFlags);
	void update(GLfloat p_deltaUpdate);
	void render();
};
