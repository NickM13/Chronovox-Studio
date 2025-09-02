#include "engine\gfx\gui\component\list\ListThick.h"
#include <chrono>
#include <iomanip>
#include <fstream>
#include <filesystem>
#include <sys/types.h>
#include <sys/stat.h>
#include <cerrno>

std::string shortDate(std::string verbose) {
	std::string week, month, day, hour, min, sec, year;
	std::string date;
	week = verbose.substr(0, 3);
	month = verbose.substr(4, 3);
	day = verbose.substr(8, 2);
	hour = verbose.substr(11, 2);
	min = verbose.substr(14, 2);
	sec = verbose.substr(17, 2);
	year = verbose.substr(20, 4);

	if (month == "Jan")		 date += "1";
	else if (month == "Feb") date += "2";
	else if (month == "Mar") date += "3";
	else if (month == "Apr") date += "4";
	else if (month == "May") date += "5";
	else if (month == "Jun") date += "6";
	else if (month == "Jul") date += "7";
	else if (month == "Aug") date += "8";
	else if (month == "Sep") date += "9";
	else if (month == "Oct") date += "10";
	else if (month == "Nov") date += "11";
	else if (month == "Dec") date += "12";

	date += "/";

	if (day[0] == ' ')
		date += day[1];
	else
		date += day;

	date += "/" + year + " ";

	int h = std::stoi(hour);
	if (h > 12) date += std::to_string(h - 12);
	else date += std::to_string(h);
	date += ":";

	date += std::to_string(std::stoi(min));

	date += ((h > 12) ? " PM" : " AM");

	return date;
}

CListThick::ListThickItem::ListThickItem(std::string name) {
	if (FILE* file = fopen(name.c_str(), "r")) {
		fclose(file);
	}
	else {
		valid = false;
		return;
	}
	this->name = name.substr(name.find_last_of('\\') + 1);
	this->path = name;
	struct stat fileInfo;
	stat(name.c_str(), &fileInfo);
	std::string mod = std::ctime(&fileInfo.st_mtime);
	this->modified = shortDate(mod);
	this->mtime = fileInfo.st_mtime;
}

CListThick::CListThick(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Uint16 p_itemHeight, Texture* p_listIcon)
	: Component(p_compName, p_title, p_pos, p_size) {
	p_size = p_size - Vector2<Sint32>(0, 1);
	m_itemHeight = p_itemHeight;
	m_scroll = m_maxScroll = 0;
	m_selectedItem = m_hoveredItem = -1;
	m_listIcon = p_listIcon;
	resize();
}

void CListThick::resize() {
	m_maxVisible = (GLfloat)m_size.y / m_itemHeight;
	m_maxScroll = std::fmaxf(0, Sint16((m_itemList.size() - m_maxVisible) * m_itemHeight));
	m_scrollBarHeight = (Sint32)(powf(m_size.y, 2) / (m_itemList.size() * m_itemHeight));
}

Component* CListThick::addItem(std::string p_itemName) {
	ListThickItem lti = ListThickItem(p_itemName);
	if (!lti.valid) return this;
	for (Sint32 i = 0; i <= static_cast<Sint32>(m_itemList.size()); i++) {
		if (i == m_itemList.size() || lti.mtime > m_itemList.at(i).mtime) {
			m_itemList.insert(m_itemList.begin() + i, lti);
			break;
		}
	}
	m_maxScroll = std::fmaxf(0, Sint16((m_itemList.size() - m_maxVisible) * m_itemHeight));
	resize();
	return this;
}
Component* CListThick::insertItem(Uint16 p_index, std::string p_itemName) {
	ListThickItem lti = ListThickItem(p_itemName);
	if (!lti.valid) return this;
	m_itemList.insert(m_itemList.begin() + p_index, lti);
	m_maxScroll = m_maxScroll = std::fmaxf(0, Sint16((m_itemList.size() - m_maxVisible) * m_itemHeight));
	resize();
	return this;
}
void CListThick::removeItem(Uint16 p_index) {
	if (m_itemList.size() > p_index) {
		m_itemList.erase(m_itemList.begin() + p_index);
		resize();
	}
}
Uint16 CListThick::getItemCount() {
	return Uint16(m_itemList.size());
}
CListThick::ListThickItem& CListThick::getItem(Sint16 p_index) {
	return m_itemList.at(p_index);
}
void CListThick::clear() {
	m_itemList.clear();
	m_selectedItem = m_hoveredItem = -1;
	resize();
}

void CListThick::selectItem(Sint16 id) {
	m_selectedItem = id;
	callPressFunction();
}
Sint16 CListThick::getSelectedItem() {
	return m_selectedItem;
}

void CListThick::input(Sint8& p_interactFlags) {
	Vector2<Sint32> _mousePos = GMouse::getMousePos() - m_pos;
	m_hover = ((p_interactFlags & (Sint8)EventFlag::MOUSEOVER) &&
		_mousePos.x >= 0 && _mousePos.x <= m_size.x &&
		_mousePos.y >= 0 && _mousePos.y <= m_size.y);

	m_hoveredItem = -1;

	if ((p_interactFlags & (Sint8)EventFlag::MOUSEOVER) &&
		_mousePos.x >= 0 && _mousePos.x <= m_size.x &&
		_mousePos.y >= 0 && _mousePos.y <= m_size.y) {
		if (_mousePos.x < m_size.x - 12) {
			if (Sint32((_mousePos.y + (GLfloat(m_scroll) / m_itemHeight) * m_itemHeight) / m_itemHeight) < Sint32(m_itemList.size())) {
				Uint16 _hoveredItem = Uint16((_mousePos.y + (GLfloat(m_scroll) / m_itemHeight) * m_itemHeight) / m_itemHeight);
				if (GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT)) {
					selectItem(_hoveredItem);
				}
				else {
					m_hoveredItem = _hoveredItem;
				}
				GGui::setCursorType(GGui::CursorType::LINK);
			}
		}
		else {
			if (GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT)) {
				if (_mousePos.y < ((GLfloat)m_scroll / m_maxScroll) * (m_size.y - m_scrollBarHeight)) {
					m_scroll -= 24;
				}
				else if (_mousePos.y > ((GLfloat)m_scroll / m_maxScroll)* (m_size.y - m_scrollBarHeight) + m_scrollBarHeight) {
					m_scroll += 24;
				}
				m_dragging = true;
			}
		}
		p_interactFlags -= (Sint8)EventFlag::MOUSEOVER;
	}

	if (m_dragging) {
		if (!GMouse::mouseDown(GLFW_MOUSE_BUTTON_LEFT)) {
			m_dragging = false;
		}
		else {
			m_scroll += static_cast<Sint16>((GLfloat(_mousePos.y - m_mouseBuffer.y) / (m_size.y - m_scrollBarHeight)) * m_maxScroll);
			if (p_interactFlags & (Sint8)EventFlag::MOUSEOVER)
				p_interactFlags -= (Sint8)EventFlag::MOUSEOVER;
		}
	}
	if ((p_interactFlags & (Sint8)EventFlag::MOUSESCROLL) && m_hover) {
		m_scroll = m_scroll - GMouse::getMouseScroll() * m_itemHeight;
		p_interactFlags -= (Sint8)EventFlag::MOUSESCROLL;
	}

	if (m_scroll > m_maxScroll)	m_scroll = m_maxScroll;
	if (m_scroll < 0)			m_scroll = 0;

	m_mouseBuffer = _mousePos;
}
void CListThick::update(GLfloat p_deltaUpdate) {
	for (Uint16 y = 0; y <= m_maxVisible; y++) {
		if (m_scroll / m_itemHeight + y >= Uint16(m_itemList.size())) continue;

		if (m_hoveredItem == m_scroll / m_itemHeight + y) {
			m_itemList.at(m_scroll / m_itemHeight + y).hoverTimer += p_deltaUpdate * 8;
			if (m_itemList.at(m_scroll / m_itemHeight + y).hoverTimer > 1) {
				m_itemList.at(m_scroll / m_itemHeight + y).hoverTimer = 1;
			}
		}
		else {
			m_itemList.at(m_scroll / m_itemHeight + y).hoverTimer -= p_deltaUpdate * 8;
			if (m_itemList.at(m_scroll / m_itemHeight + y).hoverTimer < 0) {
				m_itemList.at(m_scroll / m_itemHeight + y).hoverTimer = 0;
			}
		}
	}
}
void CListThick::renderItems() {
	Shader::pushMatrixModel();
	GBuffer::setColor(getElementColor(getElementPos() + "Text1"));
	Font::setAlignment(ALIGN_CENTER);
	Font::print(m_title, m_size.x / 2, -12);

	GBuffer::pushScissor(Rect(0, 0, GLfloat(m_size.x), GLfloat(m_size.y)));

	GBuffer::setTexture(0);
	Shader::translate(glm::vec3(0.f, -(GLfloat)(m_scroll % m_itemHeight), 0.f));

	for (Uint16 y = 0; y <= m_maxVisible; y++) {
		if (m_scroll / m_itemHeight + y >= Uint16(m_itemList.size())) continue;

		Shader::pushMatrixModel();
		Shader::translate(glm::vec3(0, y * m_itemHeight, 0));

		if (m_itemList.at(m_scroll / m_itemHeight + y).hoverTimer > 0) {
			GBuffer::setColor(getElementColor(getElementPos() + "ActionHovered").applyScale(1.f, 1.f, 1.f, m_itemList.at(m_scroll / m_itemHeight + y).hoverTimer));
			GBuffer::addQuadFilled({ 0, 1 }, { m_size.x, m_itemHeight - 1 });
		}

		GBuffer::setTexture(0);

		Shader::popMatrixModel();
	}

	GBuffer::setColor(getElementColor(getElementPos() + "Text1"));
	std::string name, path, modi;
	for (Uint16 i = 0; i <= m_maxVisible; i++) {
		if (i + round(m_scroll / m_itemHeight) < m_itemList.size()) {
			name = m_itemList[i + m_scroll / m_itemHeight].name;
			path = m_itemList[i + m_scroll / m_itemHeight].path;
			modi = m_itemList[i + m_scroll / m_itemHeight].modified;
			name = Font::getMessageSubstr(m_itemList[i + m_scroll / m_itemHeight].name, m_size.x - 112);
			
			GBuffer::setColor(Color());
			GBuffer::renderTexture(m_listIcon, { 16, Sint32((i + 0.333f) * m_itemHeight) }, {}, GBuffer::TextureStyle::CENTERED);

			Font::setFont("Bold");
			Font::setAlignment(ALIGN_LEFT);
			GBuffer::setColor(getElementColor(getElementPos() + "Text1"));
			Font::print(name, 32, Sint32((i + 0.333f) * m_itemHeight) - 1);

			Font::setFont("Body");
			GBuffer::setColor(getElementColor(getElementPos() + "Text3"));
			Font::print(path, 32, Sint32((i + 0.7f) * m_itemHeight) - 1);
			Font::setAlignment(ALIGN_RIGHT);
			Font::print(modi, m_size.x - 24, Sint32((i + 0.333f) * m_itemHeight) - 1);
		}
	}
	GBuffer::setTexture(0);
	Shader::popMatrixModel();
}
void CListThick::render() {
	Shader::pushMatrixModel();
	GBuffer::setTexture(0);
	Shader::translate(glm::vec3((GLfloat)m_pos.x, (GLfloat)m_pos.y, 0.f));
	renderItems();

	GBuffer::setColor(getElementColor(getElementPos() + "Primary"));
	GBuffer::addVertexQuad(0, m_size.y);
	GBuffer::addVertexQuad(m_size.x, m_size.y);
	GBuffer::setColor(getElementColor(getElementPos() + "Primary").applyScale(1, 1, 1, 0));
	GBuffer::addVertexQuad(m_size.x, m_size.y - m_itemHeight / 2);
	GBuffer::addVertexQuad(0, m_size.y - m_itemHeight / 2);

	if (m_maxScroll > 0 && (m_hover || m_dragging)) {
		Shader::pushMatrixModel();

		GBuffer::setColor(getElementColor(getElementPos() + "Primary"));
		GBuffer::addVertexQuad(m_size.x - 10, 0);
		GBuffer::addVertexQuad(m_size.x, 0);
		GBuffer::addVertexQuad(m_size.x, m_size.y);
		GBuffer::addVertexQuad(m_size.x - 10, m_size.y);

		Shader::translate(glm::vec3(m_size.x - 12, ((GLfloat)m_scroll / m_maxScroll) * (m_size.y - m_scrollBarHeight), 0.f));

		if (m_dragging) GBuffer::setColor(getElementColor(getElementPos() + "ActionHovered"));
		else GBuffer::setColor(getElementColor(getElementPos() + "ActionPressed"));
		GBuffer::addVertexQuad(4, 2);
		GBuffer::addVertexQuad(10, 2);
		GBuffer::addVertexQuad(10, m_scrollBarHeight - 2);
		GBuffer::addVertexQuad(4, m_scrollBarHeight - 2);

		Shader::popMatrixModel();
	}

	GBuffer::popScissor();
	Shader::popMatrixModel();
}
