#pragma once

#include "..\Component.h"
#include "engine\gfx\font\Font.h"

#include "engine\utils\global\GScreen.h"
#include <algorithm>
#include <map>
#include <thread>

class Container : public Component {
protected:
	struct Comp {
		Anchor m_posAnchor = {};
		Anchor m_sizeAnchor = {};
		Component* m_component = 0;
		Comp() { m_component = 0; }
		Comp(Anchor p_posAnchor, Anchor p_sizeAnchor, Component* p_comp) : m_posAnchor(p_posAnchor), m_sizeAnchor(p_sizeAnchor), m_component(p_comp) {};
	};
	std::map<std::string, Comp> m_componentMap;
	std::vector<std::string> m_componentOrder;
	Vector4<Sint32> m_contentArea;
	Component* m_currDialog = 0;

	void sortInComponent(Comp comp);

	void anchorComponent(Component* p_component, Anchor p_posAnchor = Anchor::NONE, Anchor p_sizeAnchor = Anchor::NONE);
public:
	Container() {};
	Container(std::string p_compName, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, bool p_visible);
	virtual ~Container();

	virtual Component* addComponent(Component* p_component, Anchor p_posAnchor = Anchor::NONE, Anchor p_sizeAnchor = Anchor::NONE);
	Component* findComponent(std::string p_compName);
	void removeComponent(std::string p_compName);

	Component* setVisible(bool p_visible);

	virtual void resize();
	Vector2<Sint32> getRealPosition();
	Vector2<Sint32> getRealSize();

	Component* openDialog(Component* p_dialog);
	Component* closeDialog();

	void updateSize();

	virtual void input();
	virtual void input(Sint8& p_interactFlags);
	virtual void update(GLfloat p_updateTime);
	virtual void render();
};
