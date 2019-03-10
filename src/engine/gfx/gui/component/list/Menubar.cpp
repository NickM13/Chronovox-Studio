#include "engine\gfx\gui\component\list\Menubar.h"

CMenubar::CMenubar(std::string p_compName, Vector2<Sint32> p_pos, Vector2<Sint32> p_size)
	: Component(p_compName, "", p_pos, p_size) {
	m_panelMain = new Panel("", "", { 0, 0 }, p_size, (Sint8)BorderFlag::NONE);
	m_panelMain->setParent(this);
	m_panelSub = new Panel("", "", { 0, 0 }, { 0, 0 }, 0);
	m_panelSub->setParent(this);
	m_texHasSubmenu = MTexture::getTexture("gui\\icon\\tool\\ArrowRightNoStem.png");
	setPrimaryPos("Top");
}
CMenubar::~CMenubar() {
	delete m_panelMain;
	delete m_panelSub;
}

//Directory splits with '\'
CMenubar::MenuElement* CMenubar::addElement(std::string p_dir, MenuElement* p_element) {
	Uint16 i = 0, j = 0;
	std::vector<std::string> _splitDir;
	if (p_dir != "") {
		while (i < p_dir.length()) {
			if (p_dir[i] == '\\') {
				_splitDir.push_back(p_dir.substr(j, i - j));
				j = i + 1;
			}
			i++;
		}
		if (p_dir[p_dir.length() - 1] != '\\') {
			_splitDir.push_back(p_dir.substr(j));
		}
	}

	Submenu* _submenu = &m_submenu;
	
	for (i = 0; i < _splitDir.size(); i++) {
		_submenu = _submenu->find(_splitDir[i]);
	}
	_submenu->addElement(p_element);

	if (p_element->getFunction() && static_cast<MenuButton*>(p_element)->getKeyBind().key != -1) {
		m_keyBinds.push_back({ static_cast<MenuButton*>(p_element)->getKeyBind(), p_element->getFunction() });
	}
	return p_element;
}
CMenubar::MenuElement* CMenubar::getElement(std::string p_dir) {
	Uint16 i = 0, j = 0;
	std::vector<std::string> _splitDir;
	if (p_dir != "") {
		while (i < p_dir.length()) {
			if (p_dir[i] == '\\') {
				_splitDir.push_back(p_dir.substr(j, i - j));
				j = i + 1;
			}
			i++;
		}
		if (p_dir[p_dir.length() - 1] != '\\') {
			_splitDir.push_back(p_dir.substr(j));
		}
	}

	Submenu* _submenu = &m_submenu;

	for (i = 0; i < _splitDir.size() - 1; i++) {
		_submenu = _submenu->find(_splitDir[i]);
	}

	return _submenu->find(_splitDir.back());
}
void CMenubar::setSize(Vector2<Sint32> p_pos) {
	Component::setSize(p_pos);
	m_panelMain->setSize(p_pos);
}

void CMenubar::input(Sint8& p_interactFlags) {
	Vector2<Sint32> _mousePos = GMouse::getMousePos() - m_pos;
	Uint16 i = 0, j = 0;
	Sint16 w = 0;
	Sint32 _buttonWidth;
	Sint32 _subWidth, _descWidth;
	MenuElement* me = 0;
	if ((p_interactFlags & (Sint8)EventFlag::MOUSEOVER) && GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT)) {
		Submenu* _submenu = &m_submenu;
		std::vector<std::string> _splitDir;
		if (m_selected != "") {
			while (i < m_selected.length()) {
				if (m_selected[i] == '\\') {
					_splitDir.push_back(m_selected.substr(j, i - j));
					j = i + 1;
				}
				i++;
			}
			if (m_selected[m_selected.length() - 1] != '\\') {
				_splitDir.push_back(m_selected.substr(j, i - j));
			}
		}

		for (Uint16 k = 0; k < _splitDir.size(); k++) {
			_submenu = _submenu->find(_splitDir[k]);
		}

		if (_splitDir.size() == 1 && m_currDir == m_selected) {
			m_currDir = "";
		}
		else if (_submenu != 0 && _submenu->getFunction() != 0) {
			m_currDir = "";
			_submenu->callFunction();
			p_interactFlags -= (Sint8)EventFlag::MOUSEOVER;
		}
		else if (_submenu->getType() != ElementType::DIVIDER) {
			m_currDir = m_selected;
		}
	}
	m_selected = "";
	if (p_interactFlags & (Sint8)EventFlag::MOUSEOVER) {
		i = j = 0;
		std::vector<std::string> _splitDir;
		if (m_currDir != "") {
			while (i < m_currDir.length()) {
				if (m_currDir[i] == '\\') {
					_splitDir.push_back(m_currDir.substr(j, i - j));
					j = i + 1;
				}
				i++;
			}
			if (m_currDir[m_currDir.length() - 1] != '\\') {
				_splitDir.push_back(m_currDir.substr(j));
			}
		}

		w = 0;
		Sint32 _subWidthTotal;
		Sint32 _selectHeight = m_slBuffer;
		Sint32 _iconWidth = Font::getSpacingHeight();
		Sint32 _height, _eHeight;
		for (i = 0; i < m_submenu.getVisibleElements().size(); i++) {
			me = m_submenu.getVisibleElements()[i];
			_buttonWidth = Font::getMessageWidth(me->getName()).x + 16;
			if (m_currDir != "" && _splitDir[0] == me->getName()) {
				std::vector<std::string> _path;
				Submenu* _submenu = &m_submenu;
				if (_submenu != 0) {
					_subWidthTotal = m_slBuffer + 1;
					for (j = 0; j < _splitDir.size(); j++) {
						Sint32 _sh = 0;
						_path.push_back(_splitDir[j]);
						if (_submenu->find(_splitDir[j])->getType() != ElementType::SUBMENU) {
							break;
						}
						_submenu = _submenu->find(_splitDir[j]);
						if (j > 0) {
							_subWidth = 0;
						}
						else {
							_subWidth = _buttonWidth;
						}
						_descWidth = 0;
						_height = 0;
						for (Uint16 k = 0; k < _submenu->getVisibleElements().size(); k++) {
							me = _submenu->getVisibleElements()[k];
							_subWidth = std::fmaxf(_subWidth, Font::getMessageWidth(me->getName()).x);
							_descWidth = std::fmaxf(_descWidth, Font::getMessageWidth(me->getDesc()).x);
							_eHeight = me->getHeight();
							if (j < _splitDir.size() - 1 && _splitDir.at(j + 1) == me->getName()) {
								_sh = _height;
							}
							_height += _eHeight;
						}
						_subWidth += _descWidth + 36 + _iconWidth;
						_height = 0;
						for (Uint16 g = 0; g < _submenu->getVisibleElements().size(); g++) {
							me = _submenu->getVisibleElements()[g];
							_eHeight = me->getHeight();
							if (_mousePos.x - w - _subWidthTotal >= 0 && _mousePos.x - w - _subWidthTotal < _subWidth
								&& _mousePos.y - m_size.y - _selectHeight >= _height
								&& _mousePos.y - m_size.y - _selectHeight < _height + _eHeight) {
								m_selected = "";
								for (Uint16 h = 0; h < _path.size(); h++) {
									m_selected += (_path[h] + "\\");
								}
								m_selected += me->getName();
								if (p_interactFlags & (Sint8)EventFlag::MOUSEOVER) {
									p_interactFlags -= (Sint8)EventFlag::MOUSEOVER;
								}
								break;
							}
							_height += _eHeight;
						}
						_selectHeight += _sh;
						_subWidthTotal += _subWidth + m_slBuffer + 1;
					}
				}
			}
			if (_mousePos.x - w >= 0 && _mousePos.x - w < Sint32(_buttonWidth) &&
				_mousePos.y >= 0 && _mousePos.y < m_size.y) {
				m_selected = m_submenu.getVisibleElements()[i]->getName();
				if (p_interactFlags & (Sint8)EventFlag::MOUSEOVER) {
					p_interactFlags -= (Sint8)EventFlag::MOUSEOVER;
				}
				break;
			}
			w += Sint16(_buttonWidth);
		}
	}
	if (m_currDir != "") {
		w = 0;
		for (i = 0; i < m_submenu.getVisibleElements().size(); i++) {
			me = m_submenu.getVisibleElements()[i];
			_buttonWidth = Font::getMessageWidth(me->getName()).x + 16;
			if (_mousePos.x - w >= 0 && _mousePos.x - w < Sint32(_buttonWidth) &&
				_mousePos.y >= 0 && _mousePos.y < m_size.y) {
				m_currDir = me->getName();
				if (p_interactFlags & (Sint8)EventFlag::MOUSEOVER) {
					p_interactFlags -= (Sint8)EventFlag::MOUSEOVER;
				}
				break;
			}
			w += Sint16(_buttonWidth);
		}
	}

	// Keybinds
	for (std::pair<GKey::KeyBind, function> kb : m_keyBinds) {
		if (GKey::keyPressed(kb.first.key, kb.first.mods)) {
			kb.second();
		}
	}
}
void CMenubar::update(GLfloat p_deltaUpdate) {
	std::vector<std::string> _splitDir, _splitSelect;
	if (m_currDir != "") {
		Uint16 i = 0, j = 0;
		while (i < m_currDir.length()) {
			if (m_currDir[i] == '\\') {
				_splitDir.push_back(m_currDir.substr(j, i - j));
				j = i + 1;
			}
			i++;
		}
		if (m_currDir[m_currDir.length() - 1] != '\\') {
			_splitDir.push_back(m_currDir.substr(j));
		}
	}
	else {
		_splitDir.push_back("");
	}

	if (m_selected != "") {
		Uint16 i = 0, j = 0;
		while (i < m_selected.length()) {
			if (m_selected[i] == '\\') {
				_splitSelect.push_back(m_selected.substr(j, i));
				j = i + 1;
			}
			i++;
		}
		if (m_selected[m_selected.length() - 1] != '\\') {
			_splitSelect.push_back(m_selected.substr(j, i));
		}
	}
	else {
		_splitSelect.push_back("");
	}

	std::string _buttonName;
	Sint32 _buttonWidth;
	Sint32 _subWidthTotal = 0;
	Sint32 _selectHeight = 0;
	Sint32 _iconWidth = Font::getSpacingHeight();
	for (Uint16 i = 0; i < m_submenu.getElements().size(); i++) {
		_buttonName = m_submenu.getElements()[i]->getName();
		_buttonWidth = Font::getMessageWidth(_buttonName).x + 16;
		m_submenu.getElements()[i]->update();
		m_submenu.updateElements();

		if (_splitDir[0] == m_submenu.getElements()[i]->getName()) {
			Submenu* _submenu = &m_submenu;
			std::string _subName;
			Sint32 _subWidth, _descWidth;
			_selectHeight = 0;
			for (Uint16 j = 0; j < _splitDir.size(); j++) {
				Sint32 _sh = 0;
				_submenu = _submenu->find(_splitDir[j]);
				_subName = _submenu->getName();
				_submenu->updateElements();
				if (j > 0) {
					_subWidth = 0;
				}
				else {
					_subWidth = _buttonWidth;
				}
				_descWidth = 0;
				for (Uint16 k = 0; k < _submenu->getElements().size(); k++) {
					_subWidth = std::fmaxf(_subWidth, Font::getMessageWidth(_submenu->getElements()[k]->getName()).x);
					_descWidth = std::fmaxf(_descWidth, Font::getMessageWidth(_submenu->getElements()[k]->getDesc()).x);
					_submenu->getElements()[k]->update();
					if (j < _splitDir.size() - 1 && _splitDir.at(j + 1) == _submenu->getElements()[k]->getName()) {
						_sh = k * Font::getSpacingHeight();
					}
				}

				for (Uint16 k = 0; k < _submenu->getElements().size(); k++) {
					if ((Uint16)_splitSelect.size() > j + 1 && _splitSelect[j + 1] == _submenu->getElements()[k]->getName()) {
						_submenu->getElements()[k]->addHoverTimer(GScreen::m_deltaTime * 8);
						if (_submenu->getElements()[k]->isHoverPopped()) {
							if (_submenu->getElements()[k]->getType() == ElementType::SUBMENU) {
								m_currDir = _submenu->getElements()[k]->getPath();
							}
						}
					}
					else {
						_submenu->getElements()[k]->addHoverTimer(-GScreen::m_deltaTime * 8);
					}
				}
				_subWidthTotal += _subWidth + _descWidth + 36 + _iconWidth;
				_selectHeight += _sh;
			}
		}
		if (_splitDir[0] != m_submenu.getElements()[i]->getName()) {
			if (m_selected == m_submenu.getElements()[i]->getName()) {
				m_submenu.getElements()[i]->addHoverTimer(GScreen::m_deltaTime * 8);
			}
			else {
				m_submenu.getElements()[i]->addHoverTimer(-GScreen::m_deltaTime * 8);
			}
		}
	}
}
void CMenubar::render() {
	std::vector<std::string> _splitDir, _splitSelect;
	if (m_currDir != "") {
		Uint16 i = 0, j = 0;
		while (i < m_currDir.length()) {
			if (m_currDir[i] == '\\') {
				_splitDir.push_back(m_currDir.substr(j, i - j));
				j = i + 1;
			}
			i++;
		}
		if (m_currDir[m_currDir.length() - 1] != '\\') {
			_splitDir.push_back(m_currDir.substr(j));
		}
	}
	else {
		_splitDir.push_back("");
	}

	if (m_selected != "") {
		Uint16 i = 0, j = 0;
		while (i < m_selected.length()) {
			if (m_selected[i] == '\\') {
				_splitSelect.push_back(m_selected.substr(j, i));
				j = i + 1;
			}
			i++;
		}
		if (m_selected[m_selected.length() - 1] != '\\') {
			_splitSelect.push_back(m_selected.substr(j, i));
		}
	}
	else {
		_splitSelect.push_back("");
	}

	Shader::pushMatrixModel();

	Shader::translate(glm::vec3((GLfloat)m_pos.x, (GLfloat)m_pos.y, 0.f));
	m_panelMain->render();
	Shader::pushMatrixModel();

	std::string _buttonName;
	Sint32 _buttonWidth;
	Sint32 _subWidthTotal = 1;
	Sint32 _selectHeight = 0;
	Sint32 _iconWidth = Font::getSpacingHeight() + 4;
	Sint32 _height, _eHeight;
	Font::setAlignment(ALIGN_LEFT);
	for (Uint16 i = 0; i < m_submenu.getVisibleElements().size(); i++) {
		_buttonName = m_submenu.getVisibleElements()[i]->getName();
		_buttonWidth = Font::getMessageWidth(_buttonName).x + 16;

		if (_splitDir[0] == m_submenu.getVisibleElements()[i]->getName()) {
			Font::setAlignment(Alignment::ALIGN_LEFT);
			Submenu* _submenu = &m_submenu;
			std::string _subName;
			Sint32 _subWidth, _descWidth;
			_selectHeight = 0;
			for (Uint16 j = 0; j < _splitDir.size(); j++) {
				Sint32 _sh = 0;
				_submenu = _submenu->find(_splitDir[j]);
				_subName = _submenu->getName();
				if (j > 0) {
					_subWidth = 0;
				}
				else {
					_subWidth = _buttonWidth;
				}
				_descWidth = 0;
				_height = 0;
				for (Uint16 k = 0; k < _submenu->getVisibleElements().size(); k++) {
					MenuElement* me = _submenu->getVisibleElements()[k];
					_subWidth = std::fmaxf(_subWidth, Font::getMessageWidth(me->getName()).x);
					_descWidth = std::fmaxf(_descWidth, Font::getMessageWidth(me->getDesc()).x);
					_eHeight = me->getHeight();
					if (j < _splitDir.size() - 1 && _splitDir.at(j + 1) == me->getName()) {
						_sh = _height;
					}
					_height += _eHeight;
				}
				_subWidth += 22;
				_descWidth += 16;

				Shader::pushMatrixModel();

				Shader::translate(glm::vec3((GLfloat)_subWidthTotal, (Sint32)(m_size.y + _selectHeight), 0.f));
				m_panelSub->setState(1);
				m_panelSub->setBorderFlag((Sint8)Component::BorderFlag::ALL);
				m_panelSub->setSize(Vector2<Sint32>(Sint32(_subWidth + _descWidth + _iconWidth), _submenu->getSubHeight()) + (m_slBuffer * 2));
				m_panelSub->render();
				Shader::translate(glm::vec3(m_slBuffer, m_slBuffer, 0));

				GBuffer::setColor(m_colorThemeMap.at("textLight"));
				_height = 0;
				for (Uint16 k = 0; k < _submenu->getVisibleElements().size(); k++) {
					MenuElement* me = _submenu->getVisibleElements()[k];
					_eHeight = me->getHeight();

					GBuffer::setTexture(0);
					if (me->getType() == ElementType::DIVIDER) {
						Shader::pushMatrixModel();
						Shader::translate(glm::vec3(0, _height + _eHeight / 2, 0));

						GBuffer::setColor(m_colorThemeMap.at("borderElementUnfocused"));
						GBuffer::addVertexQuad(1, 0);
						GBuffer::addVertexQuad(_subWidth + _descWidth + _iconWidth - 1, 0);
						GBuffer::addVertexQuad(_subWidth + _descWidth + _iconWidth - 1, 1);
						GBuffer::addVertexQuad(1, 1);

						Shader::popMatrixModel();
					}
					else {
						if ((Uint16)_splitDir.size() > j + 1 && _splitDir[j + 1] == me->getName()
							|| me->getHoverTimer() > 0) {
							GBuffer::setColor(m_colorThemeMap.at("actionHover").applyScale(Color(1, 1, 1, me->getHoverTimer() / 2.f)));
							GBuffer::addVertexQuad(0, _height);
							GBuffer::addVertexQuad((_subWidth + _descWidth + _iconWidth), _height);
							GBuffer::addVertexQuad((_subWidth + _descWidth + _iconWidth), _height + _eHeight);
							GBuffer::addVertexQuad(0, _height + _eHeight);
						}
						if (me->getType() == ElementType::CHECKBOX) {
							GLfloat cbs = 3;
							GLfloat cbh = me->getHeight() - cbs;
							GBuffer::setColor(m_colorThemeMap.at("textLight"));
							// Top
							GBuffer::addVertexQuad(cbs, cbs + _height);
							GBuffer::addVertexQuad(cbh, cbs + _height);
							GBuffer::addVertexQuad(cbh, cbs + 1 + _height);
							GBuffer::addVertexQuad(cbs, cbs + 1 + _height);
							// Right
							GBuffer::addVertexQuad(cbh, cbs + _height);
							GBuffer::addVertexQuad(cbh - 1, cbs + _height);
							GBuffer::addVertexQuad(cbh - 1, cbh + _height);
							GBuffer::addVertexQuad(cbh, cbh + _height);
							// Bottom
							GBuffer::addVertexQuad(cbs, cbh + _height);
							GBuffer::addVertexQuad(cbh, cbh + _height);
							GBuffer::addVertexQuad(cbh, cbh - 1 + _height);
							GBuffer::addVertexQuad(cbs, cbh - 1 + _height);
							// Left
							GBuffer::addVertexQuad(cbs, cbs + _height);
							GBuffer::addVertexQuad(cbs + 1, cbs + _height);
							GBuffer::addVertexQuad(cbs + 1, cbh + _height);
							GBuffer::addVertexQuad(cbs, cbh + _height);

							if (static_cast<MenuCheckbox*>(me)->isChecked()) {
								GBuffer::addVertexQuad(cbs + 3, cbs + 3 + _height);
								GBuffer::addVertexQuad(cbh - 3, cbs + 3 + _height);
								GBuffer::addVertexQuad(cbh - 3, cbh - 3 + _height);
								GBuffer::addVertexQuad(cbs + 3, cbh - 3 + _height);
							}
						}
						GBuffer::setColor(m_colorThemeMap.at("textLight"));
						Font::print(me->getName(), _iconWidth, Sint32(_height + _eHeight / 2));
						Font::print(me->getDesc(), _iconWidth + _subWidth, Sint32(_height + _eHeight / 2));
						if (me->getType() == ElementType::SUBMENU) {
							Shader::pushMatrixModel();
							Shader::translate(glm::vec3(_iconWidth + _subWidth + _descWidth - 10, Sint32(_height + _eHeight / 2), 0));
							GBuffer::renderTexture(m_texHasSubmenu, GBuffer::TextureAnchor::HALF, GBuffer::TextureAnchor::HALF);
							GBuffer::setTexture(m_texHasSubmenu->getGlId());
							Shader::popMatrixModel();
						}
					}
					_height += _eHeight;
				}
				_subWidthTotal += _subWidth + _descWidth + _iconWidth + m_slBuffer + 1;
				Shader::popMatrixModel();
				_selectHeight += _sh;
			}
		}
		GBuffer::setTexture(0);
		if (_splitDir[0] == m_submenu.getVisibleElements()[i]->getName()) {
			GBuffer::setColor(m_colorThemeMap.at("borderElementUnfocused"));
			GBuffer::addVertexQuad(0, 0);
			GBuffer::addVertexQuad(_buttonWidth, 0);
			GBuffer::addVertexQuad(_buttonWidth, m_size.y);
			GBuffer::addVertexQuad(0, m_size.y);

			GBuffer::setColor(getPrimaryColor());
			GBuffer::addVertexQuad(1, 1);
			GBuffer::addVertexQuad((_buttonWidth - 1), 1);
			GBuffer::addVertexQuad((_buttonWidth - 1), m_size.y);
			GBuffer::addVertexQuad(1, m_size.y);
		}
		else if (m_submenu.getVisibleElements()[i]->getHoverTimer() > 0) {
			GBuffer::setColor(m_colorThemeMap.at("actionHover").applyScale(Color(1, 1, 1, m_submenu.getVisibleElements()[i]->getHoverTimer() / 2.f)));
			GBuffer::addVertexQuad(1, 0);
			GBuffer::addVertexQuad(_buttonWidth, 0);
			GBuffer::addVertexQuad(_buttonWidth, m_size.y);
			GBuffer::addVertexQuad(1, m_size.y);
		}
		GBuffer::setColor(m_colorThemeMap.at("textLight"));
		Font::setAlignment(Alignment::ALIGN_CENTER);
		Font::print(m_submenu.getVisibleElements()[i]->getName(), _buttonWidth / 2, (m_size.y) / 2);
		Shader::translate(glm::vec3((GLfloat)_buttonWidth, 0.f, 0.f));
	}
	Shader::popMatrixModel();
	Shader::popMatrixModel();
}
