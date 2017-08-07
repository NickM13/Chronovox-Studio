#include "engine\gfx\gui\container\Container.h"

Container::Container(std::string p_compName, Vector2<Sint32> p_pos, Vector2<Sint32> p_size, bool p_visible)
{
	m_compName = p_compName;
	m_pos = p_pos;
	m_size = p_size;
	m_visible = p_visible;

	m_contentArea = Vector4<Sint32>();
}

Component* Container::addComponent(Component* p_component, Sint8 p_alignment)
{
	switch(p_alignment)
	{
	case PANEL_ALIGN_NONE:
		p_component->setPosition(p_component->getPosition());
		break;
	case PANEL_ALIGN_CENTER:
		p_component->setPosition(p_component->getPosition() + Vector2<Sint32>(Sint32(m_size.x - p_component->getSize().x) / 2, Sint32(m_size.y - p_component->getSize().y) / 2));
		break;
	case PANEL_ALIGN_LEFT:
		p_component->setPosition(p_component->getPosition() + Vector2<Sint32>(0, Sint32(m_size.y - p_component->getSize().y) / 2));
		break;
	case PANEL_ALIGN_RIGHT:
		p_component->setPosition(p_component->getPosition() + Vector2<Sint32>(Sint32(m_size.x - p_component->getSize().x), Sint32(m_size.y - p_component->getSize().y) / 2) + Vector2<Sint32>(Sint32(p_component->getPosition().x * -2), 0));
		break;
	case PANEL_ALIGN_TOP:
		p_component->setPosition(p_component->getPosition() + Vector2<Sint32>(Sint32(m_size.x - p_component->getSize().x) / 2, 0));
		break;
	case PANEL_ALIGN_BOTTOM:
		p_component->setPosition(p_component->getPosition() + Vector2<Sint32>(Sint32(m_size.x - p_component->getSize().x) / 2, Sint32(m_size.y - p_component->getSize().y)) + Vector2<Sint32>(0, Sint32(p_component->getPosition().y * -2)));
		break;
	case PANEL_ALIGN_TOP_LEFT:
		p_component->setPosition(p_component->getPosition());
		break;
	case PANEL_ALIGN_TOP_RIGHT:
		p_component->setPosition(p_component->getPosition() + Vector2<Sint32>(Sint32(m_size.x - p_component->getSize().x), 0) + Vector2<Sint32>(Sint32(p_component->getPosition().x * -2), 0));
		break;
	case PANEL_ALIGN_BOTTOM_LEFT:
		p_component->setPosition(p_component->getPosition() + Vector2<Sint32>(0, Sint32(m_size.y - p_component->getSize().y)) + Vector2<Sint32>(0, Sint32(p_component->getPosition().y * -2)));
		break;
	case PANEL_ALIGN_BOTTOM_RIGHT:
		p_component->setPosition(p_component->getPosition() + Vector2<Sint32>(Sint32(m_size.x - p_component->getSize().x), Sint32(m_size.y - p_component->getSize().y)) + Vector2<Sint32>(Sint32(p_component->getPosition().x * -2), Sint32(p_component->getPosition().y * -2)));
		break;
	}
	if(m_componentList.empty())
		m_contentArea = Vector4<Sint32>(p_component->getRealPosition().x, p_component->getRealPosition().y, p_component->getRealPosition().x + p_component->getSize().x, p_component->getRealPosition().y + p_component->getSize().y);
	else if(p_component->isVisible())
		m_contentArea = Vector4<Sint32>(min(p_component->getRealPosition().x, m_contentArea.x1), min(p_component->getRealPosition().y, m_contentArea.y1), max(p_component->getRealPosition().x + p_component->getRealSize().x, m_contentArea.x2), max(p_component->getRealPosition().y + p_component->getRealSize().y, m_contentArea.y2));
	m_componentList.push_back({p_alignment, p_component});
	return p_component;
}
Container::~Container()
{
	for(Sint16 i = Sint16(m_componentList.size()) - 1; i >= 0; i--)
		m_componentList.erase(m_componentList.begin() + i);
}

Container::Component* Container::findComponent(std::string p_compName)
{
	for(Uint16 i = 0; i < m_componentList.size(); i++)
		if(m_componentList[i].m_component->getName() == p_compName)
			return m_componentList[i].m_component;
	return 0;
}
void Container::removeComponent(std::string p_compName)
{
	for(Uint16 i = 0; i < m_componentList.size(); i++)
		if(m_componentList[i].m_component->getName() == p_compName)
			m_componentList.erase(m_componentList.begin() + i);
}

Component* Container::setVisible(bool p_visible)
{
	m_visible = p_visible;
	if(p_visible == false)
		for(Uint16 i = 0; i < m_componentList.size(); i++)
			m_componentList[i].m_component->update(0);
	return this;
}

Vector2<Sint32> Container::getRealPosition()
{
	return Vector2<Sint32>(m_contentArea.x1, m_pos.y + m_contentArea.y1);
}
Vector2<Sint32> Container::getRealSize()
{
	return Vector2<Sint32>(m_contentArea.x2 - m_contentArea.x1, m_contentArea.y2 - m_contentArea.y1);
}

void Container::updateSize()
{
	m_contentArea = Vector4<Sint32>();
	for(Uint16 i = 0; i < m_componentList.size(); i++)
	{
		if(m_componentList[i].m_component->isVisible())
			m_contentArea = Vector4<Sint32>(min(m_componentList[i].m_component->getRealPosition().x, m_contentArea.x1), min(m_componentList[i].m_component->getRealPosition().y, m_contentArea.y1), max(m_componentList[i].m_component->getRealPosition().x + m_componentList[i].m_component->getRealSize().x, m_contentArea.x2), max(m_componentList[i].m_component->getRealPosition().y + m_componentList[i].m_component->getRealSize().y, m_contentArea.y2));
	}
}

void Container::input()
{
	Sint8 interactFlags = 0;
	Sint8* keyStates = KeyStates::m_keyStates;
	Sint8* mouseStates = MouseStates::m_mouseStates;
	Vector2<Sint32> mousePos = MouseStates::m_guiMousePos;

	input(interactFlags, keyStates, mouseStates, mousePos);
}
void Container::input(Sint8& p_interactFlags, Sint8* p_keyStates, Sint8* p_mouseStates, Vector2<Sint32> p_mousePos)
{
	Vector2<Sint32> _mousePos;
	if(m_visible)
	{
		for(Sint32 i = m_componentList.size() - 1; i >= 0; i--)
		{
			if(!m_componentList[i].m_component->isVisible())
				continue;
			_mousePos = p_mousePos - m_pos;
			m_componentList[i].m_component->input(p_interactFlags, p_keyStates, p_mouseStates, _mousePos);
		}
	}
}
void Container::update(GLfloat p_updateTime)
{
	if(m_visible)
		for(Uint16 i = 0; i < m_componentList.size(); i++)
			if(m_componentList[i].m_component->isVisible())
			{
				if(m_componentList[i].m_component->getPriorityLayer())
				{
					m_componentList.push_back(m_componentList[i]);
					m_componentList.erase(m_componentList.begin() + i);
				}
				m_componentList[i].m_component->update(p_updateTime);
				m_componentList[i].m_component->resetTooltip();
			}

	struct
	{
		bool operator()(Comp a, Comp b)
		{
			return (a.m_component->getPriorityLayer() < b.m_component->getPriorityLayer());
		}
	} sortPriority;
	std::sort(m_componentList.begin(), m_componentList.end(), sortPriority);
}
void Container::render()
{
	if(m_visible)
	{
		glPushMatrix();
		{
			glTranslatef(m_pos.x, m_pos.y, 0);
			for(Uint16 i = 0; i < m_componentList.size(); i++)
			{
				if(m_componentList[i].m_component->isVisible())
				{
					glPushMatrix();
					{
						m_componentList[i].m_component->render();
					}
					glPopMatrix();
				}
			}
		}
		glPopMatrix();
	}
}