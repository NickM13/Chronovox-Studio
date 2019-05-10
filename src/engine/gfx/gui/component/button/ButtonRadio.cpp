#include "engine\gfx\gui\component\button\ButtonRadio.h"

CButtonRadio::CButtonRadio(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Vector2<Sint32> p_buttonSize, Vector2<Sint32> p_buttonStep)
	: Component(p_compName, p_title, p_pos, {}) {
	m_buttonSize = p_buttonSize;
	m_buttonStep = p_buttonStep;
	m_selectedHover = -1;
	m_iconWidth = m_buttonSize.x + 20;
	m_texSublist = MTexture::getTexture("gui\\icon\\toolbar\\RadioSublist.png");
}
CButtonRadio::~CButtonRadio() {
	for (RadioBase* b : m_buttonList)
		delete b;
	m_buttonList.clear();
}

Component* CButtonRadio::addButton(Texture * p_tex, std::string p_parent, std::string p_name, std::string p_desc, GKey::KeyBind p_keyBind) {
	RadioSublist* sublist = 0;
	for (RadioSublist* sl : m_buttonList) {
		if (sl->getName() == p_parent) {
			sublist = sl;
		}
	}

	if (!sublist) {
		m_buttonList.push_back(new RadioSublist(p_parent));
		sublist = static_cast<RadioSublist*>(m_buttonList.back());
		m_size = m_buttonSize + m_buttonStep * (m_buttonList.size() - 1);
	}

	RadioButton* _button = new RadioButton(p_tex, p_name, p_desc, p_keyBind);
	sublist->addButton(_button);
	return this;
}

void CButtonRadio::input(Sint8 & p_interactFlags) {
	Vector2<Sint32> _mousePos = GMouse::getMousePos() - m_pos, _mousePos2 = {};
	RadioSublist* rsl = 0;
	Sint32 prevHover = m_selectedHover;
	for (size_t i = 0; i < m_buttonList.size(); i++) {
		rsl = m_buttonList.at(i);
		_mousePos2 = _mousePos - Vector2<Sint32>(m_buttonSize.x, 0);
		if (m_selectedHover == i) {
			if (_mousePos2.x >= 4 && _mousePos2.x <= m_iconWidth + m_buttonSize.x + rsl->getTotalWidth() + 8
				&& _mousePos2.y >= -4 && _mousePos2.y <= static_cast<Sint32>(m_buttonSize.y * rsl->rblist.size() + 8)) {
				for (size_t j = 0; j < rsl->rblist.size(); j++) {
					if (_mousePos2.y >= 0 && _mousePos2.y < m_buttonSize.y) {
						rsl->rblist.at(j)->hovered = true;
						if (GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT)) {
							rsl->selected = j;
							callPressFunction();
						}
					}
					else {
						rsl->rblist.at(j)->hovered = false;
					}
					_mousePos2.y -= m_buttonSize.y;
				}
				p_interactFlags -= static_cast<Sint8>(EventFlag::MOUSEOVER);
			}
			else {
				for (size_t j = 0; j < rsl->rblist.size(); j++) {
					rsl->rblist.at(j)->hovered = false;
				}
			}
		}
		if (p_interactFlags & static_cast<Sint8>(EventFlag::MOUSEOVER)) {
			if (_mousePos.x >= 0 && _mousePos.x < m_buttonSize.x
				&& _mousePos.y >= 0 && _mousePos.y < m_buttonSize.y) {
				m_buttonList.at(i)->hovered = true;
				if (GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT)) {
					m_selectedButton = (Uint16)i;
				} else if (GMouse::mousePressed(GLFW_MOUSE_BUTTON_RIGHT)) {
					m_selectedButton = (Uint16)i;
					if (m_buttonList.at(i)->rblist.size() > 1 && prevHover != i) {
						prevHover = -1;
						m_selectedHover = i;
					}
				}
				p_interactFlags -= static_cast<Sint8>(EventFlag::MOUSEOVER);
			} else {
				m_buttonList.at(i)->hovered = false;
			}
		}
		else {
			m_buttonList.at(i)->hovered = false;
		}
		_mousePos = _mousePos - m_buttonStep;
	}
	if (prevHover != -1 && (GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT) || GMouse::mousePressed(GLFW_MOUSE_BUTTON_RIGHT))) {
		m_selectedHover = -1;
	}
	if (p_interactFlags & static_cast<Sint8>(EventFlag::KEYPRESS)) {
		RadioSublist* sl = 0;
		for (size_t i = 0; i < m_buttonList.size(); i++) {
			sl = m_buttonList.at(i);
			if (GKey::keyPressed(sl->getKeyBind().key, sl->getKeyBind().mods)) {
				if (m_selectedButton == i) {
					/*if (GKey::modDown(GLFW_MOD_SHIFT)) {
						sl->selected++;
						if (sl->selected >= (Sint32)sl->rblist.size()) {
							sl->selected = 0;
						}
					}*/
					m_prevSelectedButton = -1;
				}
				else {
					m_selectedButton = (Uint16)i;
				}
				p_interactFlags -= static_cast<Sint8>(EventFlag::KEYPRESS);
				break;
			}
		}
	}
}
void CButtonRadio::update(GLfloat p_deltaUpdate) {
	if (m_buttonList.size() != 0) {
		if (m_prevSelectedButton != m_selectedButton) {
			callPressFunction();
			m_prevSelectedButton = m_selectedButton;
		}
	}
	RadioSublist* rsl = 0;
	for (size_t i = 0; i < m_buttonList.size(); i++) {
		rsl = m_buttonList.at(i);
		if (m_selectedHover == i) {
			for (size_t j = 0; j < rsl->rblist.size(); j++) {
				if (rsl->rblist.at(j)->hovered) {
					rsl->rblist.at(j)->hoverTimer += p_deltaUpdate * 8.f;
					if (rsl->rblist.at(j)->hoverTimer > 1.f) {
						rsl->rblist.at(j)->hoverTimer = 1.f;
					}
				} else {
					rsl->rblist.at(j)->hoverTimer -= p_deltaUpdate * 8.f;
					if (rsl->rblist.at(j)->hoverTimer < 0.f) {
						rsl->rblist.at(j)->hoverTimer = 0.f;
					}
				}
			}
		}
		if (rsl->hovered) {
			rsl->hoverTimer += p_deltaUpdate * 8.f;
			if (rsl->hoverTimer > 1.f) {
				rsl->hoverTimer = 1.f;
				if (m_selectedHover == -1) {
					GGui::setTooltip(rsl->rblist.at(rsl->selected)->getName() + " (" + rsl->getKeyBindText() + ")", GMouse::getMousePos());
				}
			}
		}
		else {
			rsl->hoverTimer -= p_deltaUpdate * 8.f;
			if (rsl->hoverTimer < 0.f) {
				rsl->hoverTimer = 0.f;
			}
		}
	}
}
void CButtonRadio::render() {
	Shader::pushMatrixModel();
	Shader::translate({ m_pos.x, m_pos.y, 0 });
	RadioSublist* rb = 0;
	for (size_t i = 0; i < m_buttonList.size(); i++) {
		rb = m_buttonList.at(i);
		Shader::pushMatrixModel();
		Shader::translate(glm::vec3(m_buttonStep.x, m_buttonStep.y, 0) * static_cast<GLfloat>(i));

		GBuffer::setTexture(0);
		if (m_selectedButton == i) {
			GBuffer::setColor(m_colorThemeMap.at("actionPressed"));
			GBuffer::addQuadFilled({ 0, 0 }, m_buttonSize);
		}
		if (rb->hoverTimer > 0) {
			GBuffer::setColor(m_colorThemeMap.at("actionHighlight").applyScale(Color(1, 1, 1, rb->hoverTimer)));
			GBuffer::addQuadOutlined({ 0, 0 }, m_buttonSize);
		}

		GBuffer::setColor(Color(1.f, 1.f, 1.f));
		GBuffer::renderTexture(rb->getTexture(), (m_buttonSize - rb->getTexture()->getSize()) / 2, rb->getTexture()->getSize());

		if (rb->rblist.size() > 1) {
			GBuffer::renderTexture(m_texSublist, m_buttonSize - m_texSublist->getSize(), m_texSublist->getSize());
		}

		if (m_selectedHover == i) {
			GBuffer::setScissorActive(false);
			GBuffer::setTexture(0);
			Shader::pushMatrixModel();
			Shader::translate(glm::vec3(m_buttonSize.x + 4, 0, 0));
			GBuffer::renderShadow(Vector2<Sint32>(-4, -4), Vector2<Sint32>(m_iconWidth + m_buttonSize.x + rb->getTotalWidth() + m_buttonSize.x / 2, rb->rblist.size() * m_buttonSize.y + 8));
			GBuffer::setTexture(0);
			GBuffer::setColor(getPrimaryColor());
			GBuffer::addQuadFilled(Vector2<Sint32>(-4, -4), Vector2<Sint32>(m_iconWidth + m_buttonSize.x + rb->getTotalWidth() + m_buttonSize.x / 2, rb->rblist.size() * m_buttonSize.y + 8));
			GBuffer::setColor(m_colorThemeMap.at("borderElementUnfocused"));
			GBuffer::addQuadOutlined(Vector2<Sint32>(-4, -4), Vector2<Sint32>(m_iconWidth + m_buttonSize.x + rb->getTotalWidth() + m_buttonSize.x / 2, rb->rblist.size() * m_buttonSize.y + 8));

			for (size_t j = 0; j < rb->rblist.size(); j++) {
				Shader::pushMatrixModel();
				GBuffer::setTexture(0);
				GBuffer::setColor(m_colorThemeMap.at("actionHovered").applyScale(Color(1, 1, 1, rb->rblist.at(j)->hoverTimer)));
				GBuffer::addQuadFilled(Vector2<Sint32>(0, m_buttonStep.y * j), Vector2<Sint32>(m_iconWidth + m_buttonSize.x + rb->getTotalWidth() + m_buttonSize.x / 2 - 8, m_buttonSize.y));
				Shader::translate(glm::vec3(0, m_buttonStep.y * j, 0));
				if (rb->selected == j) {
					GBuffer::setTexture(0);
					GBuffer::setColor(m_colorThemeMap.at("textLight"));
					GBuffer::addQuadFilled(Vector2<Sint32>(6, m_buttonSize.y / 2 - 2), Vector2<Sint32>(4, 4));
				}
				Shader::translate(glm::vec3(m_buttonSize.x / 2, 0, 0));
				GBuffer::setColor(Color(1.f, 1.f, 1.f));
				GBuffer::renderTexture(rb->rblist.at(j)->getTexture(), (m_buttonSize - rb->rblist.at(j)->getTexture()->getSize()) / 2, rb->rblist.at(j)->getTexture()->getSize());
				Shader::translate(glm::vec3(m_iconWidth - m_buttonSize.x / 2, 0, 0));
				Font::setAlignment(Alignment::ALIGN_LEFT);
				GBuffer::setColor(m_colorThemeMap.at("textLight"));
				Font::print(rb->rblist.at(j)->getName(), 0, m_buttonSize.y / 2);
				Font::setAlignment(Alignment::ALIGN_RIGHT);
				Font::print(rb->rblist.at(j)->getKeyBindText(), rb->getTotalWidth() + 16, m_buttonSize.y / 2);
				Shader::popMatrixModel();
			}

			Shader::popMatrixModel();
			GBuffer::setScissorActive(true);
		}

		Shader::popMatrixModel();
	}
	Shader::popMatrixModel();
}

void CButtonRadio::setSelectedItem(Uint16 p_selectedButton) {
	m_prevSelectedButton = m_selectedButton;
	m_selectedButton = p_selectedButton;
}
std::string CButtonRadio::getSelectedRadio() {
	RadioSublist* rsl = m_buttonList.at(m_selectedButton);
	return rsl->rblist.at(rsl->selected)->getName();
}
