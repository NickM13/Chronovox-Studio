#include "engine\editor\animation\menu\KeyframeTimeline.h"
#include "engine\editor\animation\Animation.h"

KeyframeTimeline::KeyframeTimeline(Vector2<Sint32> p_pos, Vector2<Sint32> p_size)
	: Container("TIMELINE_KEYFRAME", p_pos, p_size, []() { return true; }) {
	m_border = ((Sint8)BorderFlag::TOP | (Sint8)BorderFlag::BOTTOM);
	m_maxLayer = 0;
	addComponent(new Panel("RULER_BG", "", { 0, 0 }, { 0, 24 }, (Sint8)BorderFlag::ALL), Anchor::NONE, Anchor::TOP_RIGHT);
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
	for (Sint32 i = 0; i < (Sint32)m_keyframes.size(); i++) {
		vk = m_keyframes[i];
		if (vk.layer == layer) {
			k = vk.keyframe;
			if (k->m_start + k->m_length - 1 >= p_keyframe->m_start) {
				layer++;
				i = 0;
			}
		}
	}
	m_keyframes.push_back({ p_keyframe, layer });
	m_maxLayer = std::fmaxf(m_maxLayer, layer);
	m_keyframeHeight = m_keyframeArea.h / (m_maxLayer + 1); // Frame height
}
void KeyframeTimeline::terminate() {
	m_keyframes.clear();
	m_maxLayer = 0;
}

void KeyframeTimeline::input(Sint8& p_interactFlags) {
	Container::input(p_interactFlags);
	Vector2<Sint32> _mousePos = GMouse::getMousePos() - m_pos;

	if (findComponent("RULER_BG")->isSelected()) {
		m_time = std::fminf(m_length, std::fmaxf(0, m_length * (GLfloat(_mousePos.x) / m_size.x)));
		m_playing = false;
	}

	m_hoveredKeyframe = -1;
	if (m_keyframeArea.checkPoint(_mousePos.x, _mousePos.y) &&
		(p_interactFlags & (Sint8)Component::EventFlag::MOUSEOVER)) {
		_mousePos = _mousePos - Vector2<Sint32>(m_keyframeArea.x, m_keyframeArea.y);
		VKeyframe vk;
		Keyframe* k;
		for (Sint32 i = 0; i < (Sint32)m_keyframes.size(); i++) {
			vk = m_keyframes[i];
			k = vk.keyframe;
			if (Rect(Sint32(k->m_start * m_keyframeArea.w / m_length), vk.layer * m_keyframeHeight,
				Sint32(k->m_length * m_keyframeArea.w / m_length), m_keyframeHeight)
				.checkPoint(_mousePos.x, _mousePos.y)) {
				m_hoveredKeyframe = i;
				break;
			}
		}
		if (GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT)) {
			if (m_selectedKeyframe != -1) callReleaseFunction();
			m_selectedKeyframe = m_hoveredKeyframe;
			if (m_selectedKeyframe != -1) callPressFunction();
		}
	}
}
void KeyframeTimeline::update(GLfloat p_deltaUpdate) {
	Container::update(p_deltaUpdate);
	if (m_playing && m_length > 0)
		m_time = (m_time + Sint32(p_deltaUpdate * 1000)) % m_length;
}
void KeyframeTimeline::render() {
	renderBack();
	renderFill();
	Shader::pushMatrixModel();
	Shader::translate(glm::vec3((GLfloat)m_pos.x, (GLfloat)m_pos.y, 0.f));
	Shader::pushMatrixModel();
	Shader::translate(glm::vec3(m_keyframeArea.x, m_keyframeArea.y, 0.f));
	VKeyframe vk;
	Keyframe* k;
	GLfloat s = 1; // Spacing
	GLfloat b = 1; // Frame border
	GLfloat ks, kl; // Keyframe start and length
	Sint32 l0, l1; // Keyframe layer and layer+1
	for (Sint32 i = 0; i < (Sint32)m_keyframes.size(); i++) {
		vk = m_keyframes[i];
		k = vk.keyframe;
		ks = Sint32(k->m_start * m_keyframeArea.w / m_length);
		kl = ks + Sint32(k->m_length * m_keyframeArea.w / m_length);
		l0 = (vk.layer) * m_keyframeHeight;
		l1 = (vk.layer + 1) * m_keyframeHeight;

		switch (k->m_transformationType) {
		case Keyframe::TRANSLATION: GBuffer::setColor(m_translateColor); break;
		case Keyframe::ROTATION:	GBuffer::setColor(m_rotateColor); break;
		case Keyframe::SCALE:		GBuffer::setColor(m_scaleColor); break;
		}
		GBuffer::addVertexQuad(ks, l0 + s);
		GBuffer::addVertexQuad(ks, l1 - s);
		GBuffer::addVertexQuad(kl, l1 - s);
		GBuffer::addVertexQuad(kl, l0 + s);

		if (m_selectedKeyframe == i)		GBuffer::setColor(Color(1, 1, 1, 0.5f));
		else if (m_hoveredKeyframe == i)	GBuffer::setColor(Color("0xAADDFF40"));
		else								GBuffer::setColor(Color(0, 0, 0, 0.5f));
		GBuffer::addVertexQuad(ks + 6 + b, l0 + s + b);
		GBuffer::addVertexQuad(ks + 6 + b, l1 - s - b);
		GBuffer::addVertexQuad(kl - b, l1 - s - b);
		GBuffer::addVertexQuad(kl - b, l0 + s + b);

	}
	Shader::popMatrixModel();
	Shader::popMatrixModel();

	Container::render();


	Shader::pushMatrixModel();
	{
		Vector2<Sint32> framePos = m_pos + Vector2<Sint32>(8, 0);
		Vector2<Sint32> frameArea = m_size - Vector2<Sint32>(16, 0);
		Shader::translate(glm::vec3((GLfloat)framePos.x, (GLfloat)framePos.y, 0.f));
		// Time ruler
		Sint32 lc = 20; // Ruler line count
		GBuffer::setTexture(0);
		GBuffer::setColor(m_colorThemeMap.at("borderElementUnfocused"));
		for (Sint32 i = 0; i < lc; i++) {
			if (i % 5 == 0) {
				GBuffer::addVertexQuad((GLfloat(i) / lc) * frameArea.x    , 0);
				GBuffer::addVertexQuad((GLfloat(i) / lc) * frameArea.x + 2, 0);
				GBuffer::addVertexQuad((GLfloat(i) / lc) * frameArea.x + 2, 12);
				GBuffer::addVertexQuad((GLfloat(i) / lc) * frameArea.x    , 12);
				Font::print(Util::numToStringFloat((m_length * i / lc) / 1000.f, 1), (GLfloat(i) / lc) * frameArea.x + 4, 8);
				GBuffer::setTexture(0);
			}
			else {
				GBuffer::addVertexQuad((GLfloat(i) / lc) * frameArea.x    , 0);
				GBuffer::addVertexQuad((GLfloat(i) / lc) * frameArea.x + 1, 0);
				GBuffer::addVertexQuad((GLfloat(i) / lc) * frameArea.x + 1, 8);
				GBuffer::addVertexQuad((GLfloat(i) / lc) * frameArea.x    , 8);
			}
		}
		GLfloat tl = m_time * m_keyframeArea.w / m_length; // Time line
		GBuffer::setColor(m_colorThemeMap.at("borderElementUnfocused"));
		GBuffer::addVertexQuad(tl    , 12);
		GBuffer::addVertexQuad(tl + 1, 12);
		GBuffer::addVertexQuad(tl + 1, frameArea.y);
		GBuffer::addVertexQuad(tl    , frameArea.y);
	}
	Shader::popMatrixModel();
}
