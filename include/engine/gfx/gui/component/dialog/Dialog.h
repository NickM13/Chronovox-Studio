#pragma once
#include "..\container\Container.h"
#include "..\button\Button.h"

class CDialog : public Container {
private:
	Sint32 m_buffer = 0, m_spacing = 0, m_currWidth = 0;
	Sint32 m_optionBorder, m_optionWidth, m_optionHeight;

	bool m_active = false;

	std::vector<function> m_functionList;

	struct Option {
		CButton* button;
		Sint32 function;
	};
	std::vector<Option> m_optionList;
public:
	CDialog() {};
	CDialog(std::string p_compName, std::string p_title, Vector2<Sint32> p_pos, Vector2<Sint32> p_size);

	void setActive(bool p_active);
	bool isActive();

	Sint32 addFunction(function p_func) { m_functionList.push_back(p_func); return m_functionList.size(); }
	CDialog* addOption(std::string p_title, std::string p_description, Sint32 p_funcIndex);

	void input(Sint8& p_interactFlags);
	void update(GLfloat p_updateTime);
	void render();
};
