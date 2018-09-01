#include "engine\gfx\gui\component\Component.h"
#include "engine\gfx\texture\MTexture.h"
#include "engine\gfx\gui\global\GGui.h"

std::map<Component::Theme, Component::ColorTheme> Component::m_colorThemes;

Component::Component() {
	m_colorTheme = m_colorThemes[Theme::PRIMARY];
}
Component::Component(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, Theme p_colorTheme) {
	m_compName = p_compName;
	m_title = p_title;
	m_posInit = m_pos = p_pos;
	m_sizeInit = m_size = p_size;
	m_colorTheme = m_colorThemes[p_colorTheme];
	m_texture = 0;
}
Component::~Component() {

}

void Component::init() {
	m_colorThemes = {
		//	ID								Border		Primary		Select		Hover		Text		Text Info	Border Highlight
		{Theme::PRIMARY,		ColorTheme(0x101010,	0x414141,	0x818181,	0x818181,	0xF0F0F0,	0x0F0F0F,	0x007ACC)},
	{Theme::MENUBAR,		ColorTheme(0x101010,	0x414141,	0x262626,	0x616161,	0xF0F0F0,	0x0F0F0F,	0x007ACC)},
	{Theme::INFO,			ColorTheme(0x444444,	0x007ACC,	0xBCBCBC,	0x818181,	0xF0F0F0,	0xF0F0F0,	0x007ACC)},
	{Theme::ACTION,			ColorTheme(0x101010,	0x414141,	0x303030,	0x707070,	0xF0F0F0,	0xF0F0F0,	0x007ACC)},
	{Theme::ACTION_LIGHT,	ColorTheme(0x101010,	0xFFFFFF,	0xBBBBBB,	0xDDDDDD,	0x0F0F0F,	0x0F0F0F,	0x007ACC)}
	};// 0x007ACC - light blue
}
void Component::terminate() {
	m_colorThemes.clear();
}

Component* Component::addComponent(Component* p_comp, Anchor p_posAnchor, Anchor p_sizeAnchor) { return this; }
Component* Component::findComponent(std::string p_compName) { return this; }
Component* Component::addButton(std::string p_dir, std::string p_buttonName, std::string p_desc, function p_func) { return this; }
Component* Component::addItem(std::string p_item) { return this; }
Uint16 Component::getItemCount() { return 0; }
void Component::setList(std::vector<std::string> p_items) { }

Component* Component::setPressFunction(function p_func) {
	m_pressFunction = p_func;
	return this;
}
Component* Component::setHoldFunction(function p_func) {
	m_holdFunction = p_func;
	return this;
}
Component* Component::setReleaseFunction(function p_func) {
	m_releaseFunction = p_func;
	return this;
}
Component* Component::callPressFunction() {
	if (m_pressFunction) m_pressFunction();
	return this;
}
Component* Component::callHoldFunction() {
	if (m_holdFunction) m_holdFunction();
	return this;
}
Component* Component::callReleaseFunction() {
	if (m_releaseFunction) m_releaseFunction();
	return this;
}

void Component::setSelectedItem(Uint16 p_selectedItem) { }
void Component::setSelectedButton(Uint16 p_selectedButton) { }
Uint8 Component::isUpdated() { return 0; }
Sint16 Component::getSelectedItem() { return 0; }
Sint16 Component::getPrevSelectedItem() { return 0; }
std::string Component::getName() { return m_compName; }
std::string Component::getTitle() { return m_title; }

void Component::setTooltip(std::string p_tooltip) { m_tooltip = p_tooltip; }
void Component::addTooltip() {
	if (!GMouse::mouseMoved() || m_tooltipTime > 0) {
		m_tooltipCounted = true;
		m_tooltipTime += GScreen::m_deltaTime;
		if (m_tooltipTime > 0.25f)
			GGui::setTooltip(m_tooltip, GMouse::getMousePos());
	}
	else
		m_tooltipTime = 0;
}
void Component::resetTooltip() {
	if (!m_tooltipCounted)
		m_tooltipTime = 0;
	m_tooltipCounted = false;
}

void Component::resize() {}
void Component::setTitle(std::string p_title) { m_title = p_title; }
void Component::setPosition(Vector2<Sint32> p_pos) { m_pos = p_pos; }
void Component::setSize(Vector2<Sint32> p_size) { m_size = p_size; resize(); }
Vector2<Sint32> Component::getInitialPosition() { return m_posInit; }
Vector2<Sint32> Component::getInitialSize() { return m_sizeInit; }
Vector2<Sint32> Component::getPosition() { return m_pos; }
Vector2<Sint32> Component::getSize() { return m_size; }
Vector2<Sint32> Component::getRealPosition() { return m_pos; }
Vector2<Sint32> Component::getRealSize() { return m_size; }
void Component::setState(Sint8 p_selected) { m_selected = p_selected; }
Component* Component::setPauseScreen(std::string p_screen) { return this; }

Component* Component::setVisible(bool p_visible)
{
	m_visible = p_visible;
	return this;
}
bool Component::isVisible() { return m_visible; }

void Component::input() { }
void Component::input(Sint8& p_interactFlags) { }
void Component::update(GLfloat p_deltaUpdate) { }
void Component::renderBack() {

}
void Component::renderFill(bool p_setColor) {
	Shader::pushMatrixModel();
	Shader::translate(glm::vec3((GLfloat)m_pos.x, (GLfloat)m_pos.y, 0.f));
	if (p_setColor) {
		if (isSelected())	GBuffer::setColor(m_colorTheme.m_select);
		else if (m_hovered)	GBuffer::setColor(m_colorTheme.m_hover);
		else				GBuffer::setColor(m_colorTheme.m_primary);
	}
	GBuffer::setTexture(0);
	GBuffer::addVertexQuad(0, 0);
	GBuffer::addVertexQuad(m_size.x, 0);
	GBuffer::addVertexQuad(m_size.x, m_size.y);
	GBuffer::addVertexQuad(0, m_size.y);
	if (m_texture) {
		glColor3f(1, 1, 1);
		GBuffer::setColor(Color(1.f, 1.f, 1.f, 1.f));
		GBuffer::setTexture(m_texture->getGlId());
		Vector2<Sint32> _texSize = m_texture->getSize();
		GLfloat _height, _heightF, _width, _widthF;
		switch (m_textureStyle) {
		case TextureStyle::STRETCH:
			GBuffer::setUV(0, 0); GBuffer::addVertexQuad(0, 0);
			GBuffer::setUV(1, 0); GBuffer::addVertexQuad(GLfloat(m_size.x), 0);
			GBuffer::setUV(1, 1); GBuffer::addVertexQuad(GLfloat(m_size.x), GLfloat(m_size.y));
			GBuffer::setUV(0, 1); GBuffer::addVertexQuad(0, GLfloat(m_size.y));
			break;
		case TextureStyle::WRAP:
			GBuffer::setUV(0, 0);															GBuffer::addVertexQuad(0, 0);
			GBuffer::setUV(GLfloat(m_size.x) / _texSize.x, 0);								GBuffer::addVertexQuad(GLfloat(m_size.x), 0);
			GBuffer::setUV(GLfloat(m_size.x) / _texSize.x, GLfloat(m_size.y) / _texSize.y);	GBuffer::addVertexQuad(GLfloat(m_size.x), GLfloat(m_size.y));
			GBuffer::setUV(0, GLfloat(m_size.y) / _texSize.y);								GBuffer::addVertexQuad(0, GLfloat(m_size.y));
			break;
		case TextureStyle::SCALE:
			// Top Left corner
			GBuffer::setUV(0, 1);			GBuffer::addVertexQuad(0, 0);
			GBuffer::setUV(0.25f, 1);		GBuffer::addVertexQuad(_texSize.x / 4.f, 0);
			GBuffer::setUV(0.25f, 0.75f);	GBuffer::addVertexQuad(_texSize.x / 4.f, _texSize.y / 4.f);
			GBuffer::setUV(0, 0.75f);		GBuffer::addVertexQuad(0, _texSize.y / 4.f);

			// Top Right corner
			GBuffer::setUV(0.75f, 1);		GBuffer::addVertexQuad(GLfloat(m_size.x) - _texSize.x / 4.f, 0);
			GBuffer::setUV(1, 1);			GBuffer::addVertexQuad(GLfloat(m_size.x), 0);
			GBuffer::setUV(1, 0.75f);		GBuffer::addVertexQuad(GLfloat(m_size.x), _texSize.y / 4.f);
			GBuffer::setUV(0.75f, 0.75f);	GBuffer::addVertexQuad(GLfloat(m_size.x) - _texSize.x / 4.f, _texSize.y / 4.f);

			// Bottom Left corner
			GBuffer::setUV(0, 0.25f);		GBuffer::addVertexQuad(0, GLfloat(m_size.y) - _texSize.y / 4.f);
			GBuffer::setUV(0.25f, 0.25f);	GBuffer::addVertexQuad(_texSize.x / 4.f, GLfloat(m_size.y) - _texSize.y / 4.f);
			GBuffer::setUV(0.25f, 0);		GBuffer::addVertexQuad(_texSize.x / 4.f, GLfloat(m_size.y));
			GBuffer::setUV(0, 0);			GBuffer::addVertexQuad(0, GLfloat(m_size.y));

			// Bottom Right corner
			GBuffer::setUV(0.75f, 0.25f);	GBuffer::addVertexQuad(GLfloat(m_size.x) - _texSize.x / 4.f, GLfloat(m_size.y) - _texSize.y / 4.f);
			GBuffer::setUV(1, 0.25f);		GBuffer::addVertexQuad(GLfloat(m_size.x), GLfloat(m_size.y) - _texSize.y / 4.f);
			GBuffer::setUV(1, 0);			GBuffer::addVertexQuad(GLfloat(m_size.x), GLfloat(m_size.y));
			GBuffer::setUV(0.75f, 0);		GBuffer::addVertexQuad(GLfloat(m_size.x) - _texSize.x / 4.f, GLfloat(m_size.y));

			_width = floor((GLfloat(m_size.x) - _texSize.x / 2.f) / (_texSize.x / 2.f));
			_widthF = ((GLfloat(m_size.x) - _texSize.x / 2.f) / (_texSize.x / 2.f));

			// Top and Bottom side
			for (Uint16 i = 0; i < _width; i++) {
				GBuffer::setUV(0.25f, 1);		GBuffer::addVertexQuad(_texSize.x / 4.f + (i * _texSize.x / 2.f), 0);
				GBuffer::setUV(0.75f, 1);		GBuffer::addVertexQuad(_texSize.x / 4.f + ((i + 1) * _texSize.x / 2.f), 0);
				GBuffer::setUV(0.75f, 0.75f);	GBuffer::addVertexQuad(_texSize.x / 4.f + ((i + 1) * _texSize.x / 2.f), _texSize.y / 4.f);
				GBuffer::setUV(0.25f, 0.75f);	GBuffer::addVertexQuad(_texSize.x / 4.f + (i * _texSize.x / 2.f), _texSize.y / 4.f);

				GBuffer::setUV(0.25f, 0.25f);	GBuffer::addVertexQuad(_texSize.x / 4.f + (i * _texSize.x / 2.f), GLfloat(m_size.y) - _texSize.y / 4.f);
				GBuffer::setUV(0.75f, 0.25f);	GBuffer::addVertexQuad(_texSize.x / 4.f + ((i + 1) * _texSize.x / 2.f), GLfloat(m_size.y) - _texSize.y / 4.f);
				GBuffer::setUV(0.75f, 0);		GBuffer::addVertexQuad(_texSize.x / 4.f + ((i + 1) * _texSize.x / 2.f), GLfloat(m_size.y));
				GBuffer::setUV(0.25f, 0);		GBuffer::addVertexQuad(_texSize.x / 4.f + (i * _texSize.x / 2.f), GLfloat(m_size.y));
			}
			GBuffer::setUV(0.25f, 1);									GBuffer::addVertexQuad(_texSize.x / 4.f + (_width * _texSize.x / 2.f), 0);
			GBuffer::setUV(0.25f + 0.5f * (_widthF - _width), 1);		GBuffer::addVertexQuad(_texSize.x / 4.f + ((_width + (_widthF - _width)) * _texSize.x / 2.f), 0);
			GBuffer::setUV(0.25f + 0.5f * (_widthF - _width), 0.75f);	GBuffer::addVertexQuad(_texSize.x / 4.f + ((_width + (_widthF - _width)) * _texSize.x / 2.f), _texSize.y / 4.f);
			GBuffer::setUV(0.25f, 0.75f);								GBuffer::addVertexQuad(_texSize.x / 4.f + (_width * _texSize.x / 2.f), _texSize.y / 4.f);

			GBuffer::setUV(0.25f, 0.25f);								GBuffer::addVertexQuad(_texSize.x / 4.f + (_width * _texSize.x / 2.f), GLfloat(m_size.y) - _texSize.y / 4.f);
			GBuffer::setUV(0.25f + 0.5f * (_widthF - _width), 0.25f);	GBuffer::addVertexQuad(_texSize.x / 4.f + ((_width + (_widthF - _width)) * _texSize.x / 2.f), GLfloat(m_size.y) - _texSize.y / 4.f);
			GBuffer::setUV(0.25f + 0.5f * (_widthF - _width), 0);		GBuffer::addVertexQuad(_texSize.x / 4.f + ((_width + (_widthF - _width)) * _texSize.x / 2.f), GLfloat(m_size.y));
			GBuffer::setUV(0.25f, 0);									GBuffer::addVertexQuad(_texSize.x / 4.f + (_width * _texSize.x / 2.f), GLfloat(m_size.y));

			_height = floor((GLfloat(m_size.y) - _texSize.y / 2.f) / (_texSize.y / 2.f));
			_heightF = ((GLfloat(m_size.y) - _texSize.y / 2.f) / (_texSize.y / 2.f));

			// Left and Right side
			for (Uint16 i = 0; i < _height; i++) {
				GBuffer::setUV(0, 0.75f);		GBuffer::addVertexQuad(0, _texSize.y / 4.f + (i * _texSize.y / 2.f));
				GBuffer::setUV(0, 0.25f);		GBuffer::addVertexQuad(0, (_texSize.y / 4.f + ((i + 1) * _texSize.y / 2.f)));
				GBuffer::setUV(0.25f, 0.25f);	GBuffer::addVertexQuad(_texSize.x / 4.f, (_texSize.y / 4.f + ((i + 1) * _texSize.y / 2.f)));
				GBuffer::setUV(0.25f, 0.75f);	GBuffer::addVertexQuad(_texSize.x / 4.f, _texSize.y / 4.f + (i * _texSize.y / 2.f));

				GBuffer::setUV(0.75f, 0.75f);	GBuffer::addVertexQuad(GLfloat(m_size.x) - _texSize.y / 4.f, _texSize.y / 4.f + (i * _texSize.y / 2.f));
				GBuffer::setUV(0.75f, 0.25f);	GBuffer::addVertexQuad(GLfloat(m_size.x) - _texSize.y / 4.f, _texSize.y / 4.f + ((i + 1) * _texSize.y / 2.f));
				GBuffer::setUV(1, 0.25f);		GBuffer::addVertexQuad(GLfloat(m_size.x), _texSize.y / 4.f + ((i + 1) * _texSize.y / 2.f));
				GBuffer::setUV(1, 0.75f);		GBuffer::addVertexQuad(GLfloat(m_size.x), _texSize.y / 4.f + (i * _texSize.y / 2.f));
			}
			GBuffer::setUV(0, 0.75f);									GBuffer::addVertexQuad(0, _texSize.y / 4.f + (_height * _texSize.y / 2.f));
			GBuffer::setUV(0, 0.75f - 0.5f * (_heightF - _height));		GBuffer::addVertexQuad(0, _texSize.y / 4.f + ((_height + (_heightF - _height)) * _texSize.y / 2.f));
			GBuffer::setUV(0.25f, 0.75f - 0.5f * (_heightF - _height));	GBuffer::addVertexQuad(_texSize.x / 4.f, _texSize.y / 4.f + ((_height + (_heightF - _height)) * _texSize.y / 2.f));
			GBuffer::setUV(0.25f, 0.75f);								GBuffer::addVertexQuad(_texSize.x / 4.f, _texSize.y / 4.f + (_height * _texSize.y / 2.f));

			GBuffer::setUV(0.75f, 0.75f);								GBuffer::addVertexQuad(GLfloat(m_size.x) - _texSize.x / 4.f, _texSize.y / 4.f + (_height * _texSize.y / 2.f));
			GBuffer::setUV(0.75f, 0.75f - 0.5f * (_heightF - _height));	GBuffer::addVertexQuad(GLfloat(m_size.x) - _texSize.x / 4.f, _texSize.y / 4.f + ((_height + (_heightF - _height)) * _texSize.y / 2.f));
			GBuffer::setUV(1, 0.75f - 0.5f * (_heightF - _height));		GBuffer::addVertexQuad(GLfloat(m_size.x), _texSize.y / 4.f + ((_height + (_heightF - _height)) * _texSize.y / 2.f));
			GBuffer::setUV(1, 0.75f);									GBuffer::addVertexQuad(GLfloat(m_size.x), _texSize.y / 4.f + (_height * _texSize.y / 2.f));

			// Center
			for (Uint16 x = 0; x < _width; x++) {
				for (Uint16 y = 0; y < _height; y++) {
					GBuffer::setUV(0.25f, 0.75f);	GBuffer::addVertexQuad(_texSize.x / 4.f + (x * _texSize.x / 2.f), _texSize.y / 4.f + (y * _texSize.y / 2.f));
					GBuffer::setUV(0.75f, 0.75f);	GBuffer::addVertexQuad(_texSize.x / 4.f + ((x + 1) * _texSize.x / 2.f), _texSize.y / 4.f + (y * _texSize.y / 2.f));
					GBuffer::setUV(0.75f, 0.25f);	GBuffer::addVertexQuad(_texSize.x / 4.f + ((x + 1) * _texSize.x / 2.f), _texSize.y / 4.f + ((y + 1) * _texSize.y / 2.f));
					GBuffer::setUV(0.25f, 0.25f);	GBuffer::addVertexQuad(_texSize.x / 4.f + (x * _texSize.x / 2.f), _texSize.y / 4.f + ((y + 1) * _texSize.y / 2.f));

				}
			}
			for (Uint16 x = 0; x < _width; x++) {
				GBuffer::setUV(0.25f, 0.75f);								GBuffer::addVertexQuad(_texSize.x / 4.f + (x * _texSize.x / 2.f), _texSize.y / 4.f + (_height * _texSize.y / 2.f));
				GBuffer::setUV(0.75f, 0.75f);								GBuffer::addVertexQuad(_texSize.x / 4.f + ((x + 1) * _texSize.x / 2.f), _texSize.y / 4.f + (_height * _texSize.y / 2.f));
				GBuffer::setUV(0.75f, 0.75f - 0.5f * (_heightF - _height));	GBuffer::addVertexQuad(_texSize.x / 4.f + ((x + 1) * _texSize.x / 2.f), _texSize.y / 4.f + ((_height + (_heightF - _height)) * _texSize.y / 2.f));
				GBuffer::setUV(0.25f, 0.75f - 0.5f * (_heightF - _height));	GBuffer::addVertexQuad(_texSize.x / 4.f + (x * _texSize.x / 2.f), _texSize.y / 4.f + ((_height + (_heightF - _height)) * _texSize.y / 2.f));
			}
			for (Uint16 y = 0; y < _height; y++) {
				GBuffer::setUV(0.25f, 0.75f);								GBuffer::addVertexQuad(_texSize.x / 4.f + (_width * _texSize.y / 2.f), _texSize.y / 4.f + (y * _texSize.y / 2.f));
				GBuffer::setUV(0.25f + 0.5f * (_widthF - _width), 0.75f);	GBuffer::addVertexQuad(_texSize.x / 4.f + (_width * _texSize.y / 2.f), _texSize.y / 4.f + ((y + 1) * _texSize.y / 2.f));
				GBuffer::setUV(0.25f + 0.5f * (_widthF - _width), 0.25f);	GBuffer::addVertexQuad(_texSize.x / 4.f + ((_width + (_widthF - _width)) * _texSize.y / 2.f), _texSize.y / 4.f + ((y + 1) * _texSize.y / 2.f));
				GBuffer::setUV(0.25f, 0.25f);								GBuffer::addVertexQuad(_texSize.x / 4.f + ((_width + (_widthF - _width)) * _texSize.y / 2.f), _texSize.y / 4.f + (y * _texSize.y / 2.f));
			}
			GBuffer::setUV(0.25f, 0.75f);															GBuffer::addVertexQuad(_texSize.x / 4.f + (_width * _texSize.x / 2.f), _texSize.y / 4.f + (_height * _texSize.y / 2.f));
			GBuffer::setUV(0.25f + 0.5f * (_widthF - _width), 0.75f);								GBuffer::addVertexQuad(_texSize.x / 4.f + ((_width + (_widthF - _width)) * _texSize.x / 2.f), _texSize.y / 4.f + (_height * _texSize.y / 2.f));
			GBuffer::setUV(0.25f + 0.5f * (_widthF - _width), 0.75f - 0.5f * (_heightF - _height));	GBuffer::addVertexQuad(_texSize.x / 4.f + ((_width + (_widthF - _width)) * _texSize.x / 2.f), _texSize.y / 4.f + ((_height + (_heightF - _height)) * _texSize.y / 2.f));
			GBuffer::setUV(0.25f, 0.75f - 0.5f * (_heightF - _height));								GBuffer::addVertexQuad(_texSize.x / 4.f + (_width * _texSize.x / 2.f), _texSize.y / 4.f + ((_height + (_heightF - _height)) * _texSize.y / 2.f));
			break;
		case TextureStyle::CENTERED:
		default:
			GBuffer::setUV(0, 0); GBuffer::addVertexQuad(_texSize.x / -2 + m_size.x / 2, _texSize.y / 2 + m_size.y / 2);
			GBuffer::setUV(1, 0); GBuffer::addVertexQuad(_texSize.x / 2 + m_size.x / 2, _texSize.y / 2 + m_size.y / 2);
			GBuffer::setUV(1, 1); GBuffer::addVertexQuad(_texSize.x / 2 + m_size.x / 2, _texSize.y / -2 + m_size.y / 2);
			GBuffer::setUV(0, 1); GBuffer::addVertexQuad(_texSize.x / -2 + m_size.x / 2, _texSize.y / -2 + m_size.y / 2);
			break;
		}
	}
	Shader::popMatrixModel();
}
void Component::renderBorder() {
	GBuffer::setTexture(0);
	Shader::pushMatrixModel();
	Shader::translate(glm::vec3((GLfloat)m_pos.x, (GLfloat)m_pos.y, 0.f));
	GBuffer::setColor(m_colorTheme.m_border);
	if (m_border & (Sint8)BorderFlag::TOP) {
		GBuffer::addVertexQuad(0, 0);
		GBuffer::addVertexQuad(GLfloat(m_size.x), 0);
		GBuffer::addVertexQuad(GLfloat(m_size.x), -1);
		GBuffer::addVertexQuad(0, -1);
		/* For rounded corners
		if (m_border & (Sint8)BorderFlag::LEFT) {
			GBuffer::addVertexQuad(0, 0);
			GBuffer::addVertexQuad(1, 0);
			GBuffer::addVertexQuad(1, 1);
			GBuffer::addVertexQuad(0, 1);
		}
		if (m_border & (Sint8)BorderFlag::RIGHT) {
			GBuffer::addVertexQuad((GLfloat)m_size.x, 0);
			GBuffer::addVertexQuad((GLfloat)m_size.x - 1, 0);
			GBuffer::addVertexQuad((GLfloat)m_size.x - 1, 1);
			GBuffer::addVertexQuad((GLfloat)m_size.x, 1);
		}
		*/
	}
	if (m_border & (Sint8)BorderFlag::RIGHT) {
		GBuffer::addVertexQuad(GLfloat(m_size.x), 0);
		GBuffer::addVertexQuad(GLfloat(m_size.x), GLfloat(m_size.y));
		GBuffer::addVertexQuad(GLfloat(m_size.x) + 1, GLfloat(m_size.y));
		GBuffer::addVertexQuad(GLfloat(m_size.x) + 1, 0);
	}
	if (m_border & (Sint8)BorderFlag::BOTTOM) {
		GBuffer::addVertexQuad(GLfloat(m_size.x), GLfloat(m_size.y));
		GBuffer::addVertexQuad(0, GLfloat(m_size.y));
		GBuffer::addVertexQuad(0, GLfloat(m_size.y) + 1);
		GBuffer::addVertexQuad(GLfloat(m_size.x), GLfloat(m_size.y) + 1);
		/*
		if (m_border & (Sint8)BorderFlag::RIGHT) {
			GBuffer::addVertexQuad((GLfloat)m_size.x, (GLfloat)m_size.y);
			GBuffer::addVertexQuad((GLfloat)m_size.x - 1, (GLfloat)m_size.y);
			GBuffer::addVertexQuad((GLfloat)m_size.x - 1, (GLfloat)m_size.y - 1);
			GBuffer::addVertexQuad((GLfloat)m_size.x, (GLfloat)m_size.y - 1);
		}
		if (m_border & (Sint8)BorderFlag::LEFT) {
			GBuffer::addVertexQuad(0, (GLfloat)m_size.y);
			GBuffer::addVertexQuad(1, (GLfloat)m_size.y);
			GBuffer::addVertexQuad(1, (GLfloat)m_size.y - 1);
			GBuffer::addVertexQuad(0, (GLfloat)m_size.y - 1);
		}
		*/
	}
	if (m_border & (Sint8)BorderFlag::LEFT) {
		GBuffer::addVertexQuad(0, 0);
		GBuffer::addVertexQuad(0, GLfloat(m_size.y));
		GBuffer::addVertexQuad(-1, GLfloat(m_size.y));
		GBuffer::addVertexQuad(-1, 0);
	}
	Shader::popMatrixModel();
}
void Component::render() {
	//renderBack();
	renderFill();
	renderBorder();
}
Sint8 Component::isSelected() {
	return m_selected;
}

void Component::setValue(GLfloat p_value) { m_numValue = p_value; }
GLfloat Component::getValue() { return m_numValue; }
Sint8 Component::getPriorityLayer() { return m_priority + m_moveToFront; }
bool Component::hasList() { return false; }
Component* Component::setPriorityLayer(Sint8 p_priority) {
	m_priority = p_priority;
	return this;
}
