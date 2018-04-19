#include "engine\gfx\gui\field\NumberField.h"

NumberField::NumberField(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Vector2<GLfloat> p_numBounds, NumType p_numType)
	: Component(p_compName, p_title, p_pos, {}, Theme::ACTION) {
	m_sizeInit = p_size;
	m_size = Vector2<Sint32>(p_size.x, Sint32(p_size.y * Font::getSpacingHeight()));
	m_selected = 0;
	m_numValue = 0;
	m_numText = "0";
	m_numBounds = p_numBounds;
	m_numType = p_numType;
}

void NumberField::setValue(GLfloat p_value) {
	m_numText = Util::numToStringFloat(p_value);
}

void NumberField::input(Sint8& p_interactFlags) {
	Vector2<Sint32> _mousePos = GMouse::getMousePos() - m_pos;
	if((p_interactFlags & (Sint8)EventFlag::MOUSEOVER) && _mousePos.x >= 0 && _mousePos.x < m_size.x
		&& _mousePos.y >= 0 && _mousePos.y < m_size.y) {
		addTooltip();
		p_interactFlags -= (Sint8)EventFlag::MOUSEOVER;
		if(GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT))
			m_selected = 1;
		GGui::setCursorType(GGui::CursorType::IBEAM);
	}
	else if(GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT))
		m_selected = 0;
	if((p_interactFlags & (Sint8)EventFlag::KEYPRESS) && m_selected != 0) {
		p_interactFlags -= (Sint8)EventFlag::KEYPRESS;
		std::vector<GKey::KeyEvent> _keyEvents = GKey::getKeyEvents();
		for(Uint16 i = 0; i < _keyEvents.size(); i++) {
			if(_keyEvents[i].action != 0) {
				if(_keyEvents[i].keyCode == GLFW_KEY_ENTER || _keyEvents[i].keyCode == GLFW_KEY_TAB) {
					p_interactFlags += (Sint8)EventFlag::KEYPRESS;
					m_selected = 0;
					callReleaseFunction();
				}
				else if(_keyEvents[i].keyCode == GLFW_KEY_ESCAPE) {
					m_selected = 0;
				}
				else if(_keyEvents[i].keyCode == GLFW_KEY_BACKSPACE) {
					m_numText = m_numText.substr(0, m_numText.length() - 1);
					if(m_numText == "" || m_numText == "-") m_numText = "0";
				}
				else if(_keyEvents[i].keyCode >= GLFW_KEY_0 && _keyEvents[i].keyCode <= GLFW_KEY_9) {
					if((_keyEvents[i].mods & 1) == 0) {
						if(m_numText == "0")
							m_numText = Util::numToStringInt(_keyEvents[i].keyCode - GLFW_KEY_0);
						else if(m_numText.find('.') >= m_numText.length() - 2)
							m_numText.append(Util::numToStringInt(_keyEvents[i].keyCode - GLFW_KEY_0));
					}
				}
				else if(_keyEvents[i].keyCode >= GLFW_KEY_KP_0 && _keyEvents[i].keyCode <= GLFW_KEY_KP_9) {
					if(m_numText == "0")
						m_numText = Util::numToStringInt(_keyEvents[i].keyCode - GLFW_KEY_KP_0);
					else if(m_numText.find('.') >= m_numText.length() - 2)
						m_numText.append(Util::numToStringInt(_keyEvents[i].keyCode - GLFW_KEY_KP_0));
				}
				else if(_keyEvents[i].keyCode == GLFW_KEY_UP) {
					if(m_numType == NumType::INT)
						m_numValue += (_keyEvents[i].mods & 1) ? 5 : 1;
					else
						m_numValue += (_keyEvents[i].mods & 1) ? 0.1f : 1;
					if(m_numValue > m_numBounds.y)
						m_numValue = m_numBounds.y;
					if(m_numValue < m_numBounds.x)
						m_numValue = m_numBounds.x;
					m_numText = Util::numToStringFloat(m_numValue);
					
				}
				else if(_keyEvents[i].keyCode == GLFW_KEY_DOWN) {
					if(m_numType == NumType::INT)
						m_numValue -= (_keyEvents[i].mods & 1) ? 5 : 1;
					else
						m_numValue -= (_keyEvents[i].mods & 1) ? 0.1f : 1;
					if(m_numValue > m_numBounds.y)
						m_numValue = m_numBounds.y;
					if(m_numValue < m_numBounds.x)
						m_numValue = m_numBounds.x;
					m_numText = Util::numToStringFloat(m_numValue);
				}
				else if(_keyEvents[i].keyCode == GLFW_KEY_PERIOD) {
					if(m_numType == NumType::FLOAT &&
						m_numText.find('.') == m_numText.npos) {
						m_numText.append(".");
					}
				}
				else if(_keyEvents[i].keyCode == GLFW_KEY_MINUS) {
					if(m_numBounds.x < 0) {
						m_numText = Util::numToStringFloat(std::stoi(m_numText) * -1);
					}
				}
			}
		}
	}
}

void NumberField::update(GLfloat p_deltaUpdate) {
	if(m_numType == INT)
		m_numValue = std::stoi(m_numText);
	else
		m_numValue = std::stof(m_numText);
	if(m_prevValue != m_numValue) {
		if(m_numValue > m_numBounds.y) {
			m_numValue = m_numBounds.y;
			m_numText = Util::numToStringFloat(m_numValue);
		}
		if(m_numValue < m_numBounds.x) {
			m_numValue = m_numBounds.x;
			m_numText = Util::numToStringFloat(m_numValue);
		}
		callPressFunction();
		m_prevValue = m_numValue;
	}
}

void NumberField::render() {
	glPushMatrix();
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		glTranslatef(GLfloat(m_pos.x), GLfloat(m_pos.y), 0);
		glBegin(GL_QUADS);
		{
			m_colorTheme.m_border.useColor();
			glVertex2f(-1, -1);
			glVertex2f(GLfloat(m_size.x + 1), -1);
			glVertex2f(GLfloat(m_size.x + 1), GLfloat(m_size.y + 1));
			glVertex2f(-1, GLfloat(m_size.y + 1));

			if(m_selected) m_colorTheme.m_select.useColor();
			else m_colorTheme.m_primary.useColor();
			glVertex2f(0, 0);
			glVertex2f(GLfloat(m_size.x), 0);
			glVertex2f(GLfloat(m_size.x), GLfloat(m_size.y));
			glVertex2f(0, GLfloat(m_size.y));
		}
		glEnd();
		m_colorTheme.m_textLight.useColor();
		Font::setAlignment(ALIGN_RIGHT);
		Font::print(m_title, -2, Sint32(0.5f * Font::getSpacingHeight()));
		m_colorTheme.m_text.useColor();
		Font::setAlignment(ALIGN_LEFT);
		Font::print(m_numText, 2, Sint32(0.5f * Font::getSpacingHeight()));
		if(m_selected != 0 && (fmod(glfwGetTime(), 0.5) < 0.25))
			Font::print("|", Font::getMessageWidth(m_numText).x, Sint32(0.5f * Font::getSpacingHeight()));
	}
	glPopMatrix();
}
