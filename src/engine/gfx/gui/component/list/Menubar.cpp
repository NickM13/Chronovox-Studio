#include "engine\gfx\gui\component\list\Menubar.h"

CMenubar::CMenubar(std::string p_compName, Vector2<Sint32> p_pos, Vector2<Sint32> p_size)
	: Component(p_compName, "", p_pos, p_size, Theme::MENUBAR) {
	m_panelMain = new Panel("", "", { 0, 0 }, p_size, Theme::MENUBAR, (Sint8)BorderFlag::BOTTOM);
	m_panelSub = new Panel("", "", { 0, 0 }, { 0, 0 }, Theme::MENUBAR, 0);
}
CMenubar::~CMenubar() {
	delete m_panelMain;
	delete m_panelSub;
}

//Directory splits with '\' 
Component* CMenubar::addButton(std::string p_dir, std::string p_buttonName, std::string p_desc, function p_func) {
	Uint16 i = 0, j = 0;
	std::vector<std::string> _splitDir;
	if (p_dir != "") {
		while (i < p_dir.length()) {
			if (p_dir[i] == '\\') {
				_splitDir.push_back(p_dir.substr(j, i));
				j = i + 1;
			}
			i++;
		}
		if (p_dir[p_dir.length() - 1] != '\\') {
			_splitDir.push_back(p_dir.substr(j, i));
		}
	}

	SubList* _subList = &m_buttonsMain;
	for (i = 0; i < _splitDir.size(); i++) {
		_subList = _subList->find(_splitDir[i]);
	}
	_subList->addButton(p_buttonName, p_desc, p_func);
	return this;
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
	if ((p_interactFlags & (Sint8)EventFlag::MOUSEOVER) && GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT)) {
		SubList* _subList = &m_buttonsMain;
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
			_subList = _subList->find(_splitDir[k]);
		}

		if (_splitDir.size() == 1 && m_currDir == m_selected) {
			m_currDir = "";
		}
		else if (_subList != 0 && _subList->getFunction() != 0) {
			m_currDir = "";
			_subList->callPressFunction();
			p_interactFlags -= (Sint8)EventFlag::MOUSEOVER;
		}
		else {
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
					_splitDir.push_back(m_currDir.substr(j, i));
					j = i + 1;
				}
				i++;
			}
			if (m_currDir[m_currDir.length() - 1] != '\\') {
				_splitDir.push_back(m_currDir.substr(j, i));
			}
		}

		w = 0;
		Sint32 _subWidthTotal;
		Sint32 _selectHeight = 0;
		for (i = 0; i < m_buttonsMain.m_buttons.size(); i++) {
			_subWidthTotal = 0;
			_buttonWidth = Font::getMessageWidth(m_buttonsMain.m_buttons[i].m_name).x;
			if (m_currDir != "" && _splitDir[0] == m_buttonsMain.m_buttons[i].m_name) {
				std::vector<std::string> _path;
				SubList* _subList = &m_buttonsMain;
				if (_subList != 0) {
					Sint32 _subWidthTotal = 0;
					for (j = 0; j < _splitDir.size(); j++) {
						Sint32 _sh = 0;
						_path.push_back(_splitDir[j]);
						_subList = _subList->find(_splitDir[j]);
						_subWidth = _buttonWidth;
						_descWidth = 0;
						for (Uint16 k = 0; k < _subList->m_buttons.size(); k++) {
							_subWidth = std::fmaxf(_subWidth, Font::getMessageWidth(_subList->m_buttons[k].m_name).x);
							_descWidth = std::fmaxf(_descWidth, Font::getMessageWidth(_subList->m_buttons[k].m_desc).x);
							if (j < _splitDir.size() - 1 && _splitDir.at(j + 1) == _subList->m_buttons[k].m_name) {
								_sh = k * Font::getSpacingHeight();
							}
						}
						_subWidth += _descWidth + 32;
						for (Uint16 g = 0; g < _subList->m_buttons.size(); g++) {
							if (_mousePos.x - w - _subWidthTotal >= 0 && _mousePos.x - w - _subWidthTotal < _subWidth
								&& _mousePos.y - m_size.y - _selectHeight >= g * Font::getSpacingHeight()
								&& _mousePos.y - m_size.y - _selectHeight < (g + 1) * Font::getSpacingHeight()) {
								m_selected = "";
								for (Uint16 h = 0; h < _path.size(); h++) {
									m_selected += (_path[h] + "\\");
								}
								m_selected += _subList->m_buttons[g].m_name;
								p_interactFlags -= (Sint8)EventFlag::MOUSEOVER;
								break;
							}
						}
						_selectHeight += _sh;
						_subWidthTotal += _subWidth;
					}
				}
			}
			if (_mousePos.x - w >= 0 && _mousePos.x - w < Sint32(_buttonWidth + 16) &&
				_mousePos.y >= 0 && _mousePos.y < m_size.y) {
				m_selected = m_buttonsMain.m_buttons[i].m_name;
				p_interactFlags -= (Sint8)EventFlag::MOUSEOVER;
				break;
			}
			w += Sint16(_buttonWidth + 16);
		}
	}
	if (m_currDir != "") {
		w = 0;
		for (i = 0; i < m_buttonsMain.m_buttons.size(); i++) {
			_buttonWidth = Font::getMessageWidth(m_buttonsMain.m_buttons[i].m_name).x;
			if (_mousePos.x - w >= 0 && _mousePos.x - w < Sint32(_buttonWidth + 16) &&
				_mousePos.y >= 0 && _mousePos.y < m_size.y) {
				m_currDir = m_buttonsMain.m_buttons[i].m_name;
				if (p_interactFlags & (Sint8)EventFlag::MOUSEOVER)
					p_interactFlags -= (Sint8)EventFlag::MOUSEOVER;
				break;
			}
			w += Sint16(_buttonWidth + 16);
		}
	}
	if ((p_interactFlags & (Sint8)EventFlag::MOUSEOVER) &&
		_mousePos.x >= 0 && _mousePos.x < m_size.x &&
		_mousePos.y >= 0 && _mousePos.y < m_size.y)
		p_interactFlags -= (Sint8)EventFlag::MOUSEOVER;
}
void CMenubar::update(GLfloat p_deltaUpdate) {

}
void CMenubar::render() {
	std::vector<std::string> _splitDir, _splitSelect;
	if (m_currDir != "") {
		Uint16 i = 0, j = 0;
		while (i < m_currDir.length()) {
			if (m_currDir[i] == '\\') {
				_splitDir.push_back(m_currDir.substr(j, i));
				j = i + 1;
			}
			i++;
		}
		if (m_currDir[m_currDir.length() - 1] != '\\') {
			_splitDir.push_back(m_currDir.substr(j, i));
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
	Sint32 _subWidthTotal = 0;
	Sint32 _selectHeight = 0;
	Font::setAlignment(ALIGN_LEFT);
	for (Uint16 i = 0; i < m_buttonsMain.m_buttons.size(); i++) {
		_buttonName = m_buttonsMain.m_buttons[i].m_name;
		_buttonWidth = Font::getMessageWidth(_buttonName).x + 16;

		if (_splitDir[0] == m_buttonsMain.m_buttons[i].m_name) {
			Font::setAlignment(Alignment::ALIGN_LEFT);
			SubList* _subList = &m_buttonsMain;
			std::string _subName;
			Sint32 _subWidth, _descWidth;
			_selectHeight = 0;
			for (Uint16 j = 0; j < _splitDir.size(); j++) {
				Sint32 _sh = 0;
				_subList = _subList->find(_splitDir[j]);
				_subName = _subList->m_name;
				_subWidth = _buttonWidth;
				_descWidth = 0;
				for (Uint16 k = 0; k < _subList->m_buttons.size(); k++) {
					_subWidth = std::fmaxf(_subWidth, Font::getMessageWidth(_subList->m_buttons[k].m_name).x);
					_descWidth = std::fmaxf(_descWidth, Font::getMessageWidth(_subList->m_buttons[k].m_desc).x);
					if (j < _splitDir.size() - 1 && _splitDir.at(j + 1) == _subList->m_buttons[k].m_name) {
						_sh = k * Font::getSpacingHeight();
					}
				}
				Shader::pushMatrixModel();

				Shader::translate(glm::vec3((GLfloat)_subWidthTotal, (GLfloat)m_size.y + _selectHeight, 0.f));
				m_panelSub->setState(1);
				m_panelSub->setBorderFlag((Sint8)Component::BorderFlag::ALL);
				m_panelSub->setSize(Vector2<Sint32>(Sint32(_subWidth + _descWidth + 32), Sint32((_subList->m_buttons.size()) * Font::getSpacingHeight() + 4)));
				m_panelSub->render();

				GBuffer::setColor(m_colorTheme.m_text);
				for (Uint16 k = 0; k < _subList->m_buttons.size(); k++) {
					if ((Uint16(_splitDir.size()) > j + 1 && _splitDir[j + 1] == _subList->m_buttons[k].m_name)
						|| (Uint16(_splitSelect.size()) > j + 1 && _splitSelect[j + 1] == _subList->m_buttons[k].m_name)) {
						GBuffer::setTexture(0);
						GBuffer::setColor(m_colorTheme.m_hover);
						GBuffer::addVertexQuad(0, (k * Font::getSpacingHeight()));
						GBuffer::addVertexQuad((_subWidth + _descWidth + 32), (k * Font::getSpacingHeight()));
						GBuffer::addVertexQuad((_subWidth + _descWidth + 32), ((k + 1) * Font::getSpacingHeight()));
						GBuffer::addVertexQuad(0, ((k + 1) * Font::getSpacingHeight()));
						GBuffer::setColor(m_colorTheme.m_text);
					}
					Font::print(_subList->m_buttons[k].m_name, 8, Sint32((k + 0.5f) * Font::getSpacingHeight()));
					Font::print(_subList->m_buttons[k].m_desc, 24 + _subWidth, Sint32((k + 0.5f) * Font::getSpacingHeight()));
				}
				_subWidthTotal += _subWidth + _descWidth + 32;
				Shader::popMatrixModel();
				_selectHeight += _sh;
			}
		}
		GBuffer::setTexture(0);
		if (_splitDir[0] == m_buttonsMain.m_buttons[i].m_name) {
			GBuffer::setColor(m_colorTheme.m_border);
			GBuffer::addVertexQuad(-1, 0);
			GBuffer::addVertexQuad(_buttonWidth, 0);
			GBuffer::addVertexQuad(_buttonWidth, m_size.y);
			GBuffer::addVertexQuad(-1, m_size.y);

			GBuffer::setColor(m_colorTheme.m_select);
			GBuffer::addVertexQuad(0, 1);
			GBuffer::addVertexQuad((_buttonWidth - 1), 1);
			GBuffer::addVertexQuad((_buttonWidth - 1), m_size.y);
			GBuffer::addVertexQuad(0, m_size.y);
		}
		else if (m_selected == m_buttonsMain.m_buttons[i].m_name) {
			GBuffer::setColor(m_colorTheme.m_hover);
			GBuffer::addVertexQuad(0, 0);
			GBuffer::addVertexQuad(_buttonWidth, 0);
			GBuffer::addVertexQuad(_buttonWidth, m_size.y);
			GBuffer::addVertexQuad(0, m_size.y);
		}
		GBuffer::setColor(m_colorTheme.m_text);
		Font::setAlignment(Alignment::ALIGN_CENTER);
		Font::print(m_buttonsMain.m_buttons[i].m_name, _buttonWidth / 2, (m_size.y) / 2);
		Shader::translate(glm::vec3((GLfloat)_buttonWidth, 0.f, 0.f));
	}
	Shader::popMatrixModel();
	Shader::popMatrixModel();
}
