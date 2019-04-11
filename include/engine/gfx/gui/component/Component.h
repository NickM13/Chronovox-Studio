#pragma once

#include "engine\gfx\texture\MTexture.h"
#include "engine\gfx\shader\Shader.h"
#include "engine\utils\Utilities.h"
#include "..\global\GGui.h"
#include "..\buffer\GBuffer.h"
#include <functional>

/*
		1	2	4
	____________________
   |					|
8  |	9	10	12		|
16 |	17	18	20		|
32 |	33	34	36		|
   |____________________|
*/

class Component {
protected:
	typedef std::function<void()> function;
	enum class BAnchor {
		LEFT = 1,
		CENTER = 2,
		RIGHT = 4,
		TOP = 8,
		MIDDLE = 16,
		BOTTOM = 32
	};
public:
	enum class Anchor {
		NONE = 0,
		TOP_LEFT = 9,
		TOP_CENTER = 10,
		TOP_RIGHT = 12,
		MIDDLE_LEFT = 17,
		MIDDLE_CENTER = 18,
		MIDDLE_RIGHT = 20,
		BOTTOM_LEFT = 33,
		BOTTOM_CENTER = 34,
		BOTTOM_RIGHT = 36
	};
	enum class EventFlag {
		MOUSEOVER = 1,
		KEYPRESS = 2,
		MOUSESCROLL = 4,
		ALL = 7
	};
	enum class BorderFlag : Sint8 {
		NONE = 0,
		TOP = 1,
		RIGHT = 2,
		BOTTOM = 4,
		LEFT = 8,
		ALL = 15
	};

protected:
	function m_pressFunction = 0, m_holdFunction = 0, m_releaseFunction = 0;

	std::string m_compName, m_title;
	Vector2<Sint32> m_posInit, m_pos, m_sizeInit, m_size;
	Sint8 m_selected = 0;
	bool m_hovered = false;
	GLfloat m_hoverTimer = 0;
	// Appends to primary to link with ColorTheme color
	std::string m_primaryPos = "";

	// Type is Component because Container includes Component.h, just use
	// a static_cast<Container*>
	Component* m_parentContainer = 0;

	Texture* m_texture = 0;
	GBuffer::TextureStyle m_textureStyle = GBuffer::TextureStyle::STRETCH;

	Sint8 m_border = (Sint8)BorderFlag::ALL;

	GLfloat m_numValue = 0;

	// Default color themes, loaded from res/config/ColorTheme.ini
	static std::map<std::string, Color> m_colorThemeMap;
	bool m_visible = true;
	bool m_highlighting = true;
	GGui::CursorType m_hoverCursor = GGui::CursorType::NONE;
	Sint8 m_moveToFront = 0;
	Sint8 m_priority = 0;

	std::string m_tooltip = "";
	GLfloat m_tooltipTime = 0;
	bool m_tooltipCounted = false;
	Vector2<Sint32> m_tooltipMouse = {};

	void setHovered(bool p_hovered);
public:
	Component();
	Component(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Vector2<Sint32> p_size);
	virtual ~Component();

	static void init();
	static void loadTheme();
	static void terminate();

	Component* getParent() const { return m_parentContainer; }
	void setParent(Component* p_parentContainer) { m_parentContainer = p_parentContainer; }
	virtual Component* addComponent(Component* p_comp, Anchor p_posAnchor = Anchor::NONE, Anchor p_sizeAnchor = Anchor::NONE);
	virtual Component* findComponent(std::string p_compName);

	static Color getElementColor(std::string p_element);
	Color getPrimaryColor();
	virtual std::string getPrimaryPos();
	virtual void setPrimaryPos(std::string p_pp);
	virtual Component* addItem(std::string p_item);
	virtual Uint16 getItemCount();
	virtual void setList(std::vector<std::string> p_items);
	virtual bool hasList();

	virtual bool exists();

	Component* setPressFunction(function p_func);
	Component* setHoldFunction(function p_func);
	Component* setReleaseFunction(function p_func);
	Component* callPressFunction();
	Component* callHoldFunction();
	Component* callReleaseFunction();

	virtual void setSelectedItem(Uint16 p_selectedItem);
	virtual void setSelectedButton(Uint16 p_selectedButton);
	virtual Sint16 getSelectedItem();
	virtual Sint16 getPrevSelectedItem();

	std::string getName();
	virtual std::string getTitle();

	void setTooltip(std::string p_tooltip);
	void addTooltip();
	void resetTooltip();

	virtual void setTitle(std::string p_title);
	Component* setBorderFlag(Sint8 p_border) { m_border = p_border; return this; } // Use flags from enum Component::BorderFlag
	virtual void resize();
	virtual void setPosition(Vector2<Sint32> p_pos);
	virtual void setSize(Vector2<Sint32> p_size);
	Vector2<Sint32> getInitialPosition();
	Vector2<Sint32> getInitialSize();
	Vector2<Sint32> getPosition();
	Vector2<Sint32> getSize();
	virtual Vector2<Sint32> getRealPosition();
	virtual Vector2<Sint32> getRealSize();

	virtual Component* setVisible(bool p_visible);
	bool isVisible();
	Component* setHighlightActive(bool p_highlighting) { m_highlighting = p_highlighting; return this; }
	Component* setHoverCursor(GGui::CursorType p_cursor) { m_hoverCursor = p_cursor; return this; }

	void renderBack();
	void renderFill(bool p_setColor = true);
	void renderBorder();
	void renderShadow();
	virtual void render();
	virtual Sint8 isSelected();
	virtual void setValue(GLfloat p_value);
	GLfloat getValue();

	Component* setPriorityLayer(Sint8 p_priority);
	Sint8 getPriorityLayer();

	virtual void setState(Sint8 p_selected);
	virtual Uint8 isUpdated();

	virtual void input();
	virtual void input(Sint8& p_interactFlags);
	virtual void update(GLfloat p_deltaUpdate);
};
