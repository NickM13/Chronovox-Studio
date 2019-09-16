#include "engine\gfx\gui\component\button\Button.h"
#include "engine\gfx\gui\component\container\Container.h"

CButton::CButton(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, RenderStyle p_borderStyle, function p_func)
	: Component(p_compName, p_title, p_pos, p_size) {
	m_selected = 0;
	m_renderStyle = p_borderStyle;
	m_releaseFunction = p_func;
}
CButton::CButton(std::string p_compName, std::string p_title, Texture* p_buttonTex, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, RenderStyle p_borderStyle, function p_func)
	: Component(p_compName, p_title, p_pos, p_size) {
	m_selected = 0;
	m_renderStyle = p_borderStyle;
	m_releaseFunction = p_func;

	m_texture = p_buttonTex;
	m_textureStyle = GBuffer::TextureStyle::CENTERED;
}
void CButton::input(Sint8& p_interactFlags) {
	Vector2<Sint32> _mousePos = GMouse::getMousePos();
	if ((p_interactFlags & (Sint8)EventFlag::MOUSEOVER) &&
		_mousePos.x >= m_pos.x && _mousePos.x < m_pos.x + m_size.x &&
		_mousePos.y >= m_pos.y && _mousePos.y < m_pos.y + m_size.y) {
		setHovered(true);
		GGui::setCursorType(m_hoverCursor);
		addTooltip();
		p_interactFlags -= (Sint8)EventFlag::MOUSEOVER;
	}
	else {
		setHovered(false);
	}

	if (GMouse::mouseDown(GLFW_MOUSE_BUTTON_LEFT)) {
		if (m_hovered || m_selected != 0) {
			if (GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT)) {
				if (m_selected == 0) {
					m_selected = 1;
					if (m_pressFunction) m_pressFunction();
				}
				else m_selected = 2;
			}
		}
		else m_selected = 0;
	}
	else {
		if (m_selected == 1 || m_selected == 2) {
			m_selected = 3;
		}
		else if (m_selected == 3) {
			m_selected = 0;
			if (m_releaseFunction && m_hovered) m_releaseFunction();
		}
	}
}
void CButton::update(GLfloat p_deltaUpdate) {
	Component::update(p_deltaUpdate);
	if (m_stuck) {
		m_selected = 0;
		m_stuck = false;
	}
	else if (m_selected == 3) {
		m_stuck = true;
	}
	else {
		m_stuck = false;
	}
}
void CButton::render() {
	Shader::pushMatrixModel();
	GBuffer::setTexture(0);
	Shader::translate(glm::vec3((GLfloat)m_pos.x, (GLfloat)m_pos.y, 0.f));
	if (m_renderStyle & RenderStyle::FILL) {
		GBuffer::setColor(getElementColor("buttonPrimary"));
		GBuffer::addQuadFilled({}, m_size);
	}
	if (m_highlighting && (isSelected() || m_hoverTimer > 0)) {
		if (isSelected())			GBuffer::setColor(getElementColor("buttonPressed").applyScale(1.f, 1.f, 1.f));
		else if (m_hoverTimer > 0)	GBuffer::setColor(getElementColor("buttonHovered").applyScale(1.f, 1.f, 1.f, m_hoverTimer));
		GBuffer::addQuadFilled({}, m_size);
	}
	if (m_texture) {
		GBuffer::renderTexture(m_texture, {}, m_size, m_textureStyle);
	}
	Shader::popMatrixModel();
	if (m_renderStyle & RenderStyle::BORDER) {
		GBuffer::renderShadow(m_pos - 1, m_size + 2);
		GBuffer::setTexture(0);
		//Component::renderBorder();
	}

	GBuffer::setColor(getElementColor(getElementPos() + "Text1"));
	Font::setAlignment(ALIGN_CENTER);
	Font::print(m_title, m_pos.x + m_size.x / 2, m_pos.y + m_size.y / 2);
}

void CButton::setState(Sint8 p_selected) {
	m_selected = p_selected;
	if (p_selected == 1) callReleaseFunction();
}

// 0 = not selected, 1 = JUST pressed, 2 = is held, 3 = JUST released
Sint8 CButton::isSelected() {
	if (!m_visible) return 0;
	return m_selected;
}
