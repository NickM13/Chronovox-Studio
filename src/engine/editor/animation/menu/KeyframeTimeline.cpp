#include "engine\editor\animation\menu\KeyframeTimeline.h"
#include "engine\editor\animation\Animation.h"

KeyframeTimeline::KeyframeTimeline(Vector2<Sint32> p_pos, Vector2<Sint32> p_size)
	: Container("TIMELINE_KEYFRAME", p_pos, p_size, true) {
	m_border = ((Sint8)BorderFlag::TOP | (Sint8)BorderFlag::BOTTOM);
	m_maxLayer = 0;
	addComponent(new Panel("RULER_BG", "", {0, 0}, {0, 24}, Component::Theme::INFO, (Sint8)BorderFlag::ALL), Anchor::NONE, Anchor::TOP_RIGHT);
	m_translateColor = Color(1, 0.5f, 0);
	m_rotateColor = Color(1, 0, 0);
	m_scaleColor = Color(0.5f, 1, 0);
	resize();
	m_selectedKeyframe = m_hoveredKeyframe = -1;
}

void KeyframeTimeline::resize() {
	Container::resize();
	m_keyframeArea = Rect(0, 24, m_size.x, m_size.y - 24);
	m_keyframeHeight = m_keyframeArea.h / (m_maxLayer + 1); // Frame height
}

void KeyframeTimeline::setLength(Sint32 p_length) {
	m_length = p_length;
}
void KeyframeTimeline::setTime(Sint32 p_time) {
	m_time = p_time;
}

// Keyframes should be added in order of their start time
void KeyframeTimeline::addKeyframe(Keyframe* p_keyframe) {
	VKeyframe vk;
	Keyframe* k;
	Sint8 layer = 0;
	for(Sint32 i = 0; i < (Sint32)m_keyframes.size(); i++) {
		vk = m_keyframes[i];
		if(vk.layer == layer) {
			k = vk.keyframe;
			if(k->m_start + k->m_length - 1 >= p_keyframe->m_start) {
				layer++;
				i = 0;
			}
		}
	}
	m_keyframes.push_back({p_keyframe, layer});
	m_maxLayer = max(m_maxLayer, layer);
	m_keyframeHeight = m_keyframeArea.h / (m_maxLayer + 1); // Frame height
}
void KeyframeTimeline::terminate() {
	m_keyframes.clear();
	m_maxLayer = 0;
}

void KeyframeTimeline::input(Sint8& p_interactFlags) {
	Container::input(p_interactFlags);
	Vector2<Sint32> _mousePos = GMouse::getMousePos() - m_pos;

	if(findComponent("RULER_BG")->isSelected()) {
		m_time = min(m_length, max(0, m_length * (GLfloat(_mousePos.x) / m_size.x)));
		m_playing = false;
	}

	m_hoveredKeyframe = -1;
	if(m_keyframeArea.checkPoint(_mousePos.x, _mousePos.y) &&
		(p_interactFlags & (Sint8)Component::EventFlag::MOUSEOVER)) {
		_mousePos = _mousePos - Vector2<Sint32>(m_keyframeArea.x, m_keyframeArea.y);
		VKeyframe vk;
		Keyframe* k;
		for(Sint32 i = 0; i < (Sint32)m_keyframes.size(); i++) {
			vk = m_keyframes[i];
			k = vk.keyframe;
			if(Rect(Sint32(k->m_start * m_keyframeArea.w / m_length), vk.layer * m_keyframeHeight,
				Sint32(k->m_length * m_keyframeArea.w / m_length), m_keyframeHeight)
				.checkPoint(_mousePos.x, _mousePos.y)) {
				m_hoveredKeyframe = i;
				break;
			}
		}
		if(GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT)) {
			if(m_selectedKeyframe != -1) callReleaseFunction();
			m_selectedKeyframe = m_hoveredKeyframe;
			if(m_selectedKeyframe != -1) callPressFunction();
		}
	}
}
void KeyframeTimeline::update(GLfloat p_deltaUpdate) {
	Container::update(p_deltaUpdate);
	if(m_playing && m_length > 0)
		m_time = (m_time + Sint32(p_deltaUpdate * 1000)) % m_length;
}
void KeyframeTimeline::render() {
	renderBack();
	renderFill();
	glPushMatrix();
	{
		glTranslatef(GLfloat(m_pos.x), GLfloat(m_pos.y), 0);
		glPushMatrix();
		{
			glTranslatef(m_keyframeArea.x, m_keyframeArea.y, 0);
			glBegin(GL_QUADS);
			{
				VKeyframe vk;
				Keyframe* k;
				GLfloat s = 1; // Spacing
				GLfloat b = 1; // Frame border
				GLfloat ks, kl; // Keyframe start and length
				Sint32 l0, l1; // Keyframe layer and layer+1
				for(Sint32 i = 0; i < (Sint32)m_keyframes.size(); i++) {
					vk = m_keyframes[i];
					k = vk.keyframe;
					ks = Sint32(k->m_start * m_keyframeArea.w / m_length);
					kl = ks + Sint32(k->m_length * m_keyframeArea.w / m_length);
					l0 = (vk.layer) * m_keyframeHeight;
					l1 = (vk.layer + 1) * m_keyframeHeight;

					switch(k->m_transformationType) {
					case Keyframe::TRANSLATION: m_translateColor.useColor(); break;
					case Keyframe::ROTATION: m_rotateColor.useColor(); break;
					case Keyframe::SCALE: m_scaleColor.useColor(); break;
					}
					glVertex2f(ks, l0 + s);
					glVertex2f(ks, l1 - s);
					glVertex2f(kl, l1 - s);
					glVertex2f(kl, l0 + s);

					if(m_selectedKeyframe == i)
						glColor4f(1, 1, 1, 0.5f);
					else if(m_hoveredKeyframe == i)
						Color(0xAADDFF).useColor(1, 1, 1, 0.3f);
					else
						glColor4f(0, 0, 0, 0.5f);
					glVertex2f(ks + 6 + b, l0 + s + b);
					glVertex2f(ks + 6 + b, l1 - s - b);
					glVertex2f(kl - b, l1 - s - b);
					glVertex2f(kl - b, l0 + s + b);

				}
			}
			glEnd();
		}
		glPopMatrix();
	}
	glPopMatrix();

	Container::render();

	glPushMatrix();
	{
		glTranslatef(GLfloat(m_pos.x), GLfloat(m_pos.y), 0);
		// Time ruler
		Sint32 lc = 20; // Ruler line count
		m_colorTheme.m_border.useColor();
		for(Sint32 i = 0; i < lc; i++) {
			if(i % 5 == 0) {
				glLineWidth(2);
				glBegin(GL_LINES);
				{
					glVertex2f((GLfloat(i) / lc) * m_size.x, 0);
					glVertex2f((GLfloat(i) / lc) * m_size.x, 12);
				}
				glEnd();
				Font::print(Util::numToStringFloat((m_length * i / lc) / 1000.f, 1), (GLfloat(i) / lc) * m_size.x + 4, 8);
			}
			else {
				glLineWidth(1);
				glBegin(GL_LINES);
				{
					glVertex2f((GLfloat(i) / lc) * m_size.x, 0);
					glVertex2f((GLfloat(i) / lc) * m_size.x, 8);
				}
				glEnd();
			}
		}
		GLfloat tl = m_time * m_keyframeArea.w / m_length; // Time line
		glLineWidth(2);
		m_colorTheme.m_borderHighlight.useColor();
		glBegin(GL_LINES);
		{
			glVertex2f(tl, 0);
			glVertex2f(tl, m_size.y);
		}
		glEnd();
		glLineWidth(1);
	}
	glPopMatrix();
}
