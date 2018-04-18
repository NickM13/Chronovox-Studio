#pragma once

#include "Command.h"
#include <vector>

template <class T>
class CommandCombo : public Command {
private:
	std::vector<T> m_commands;
public:
	CommandCombo(std::vector<T> p_commands) {
		m_commands = p_commands;
	}
	void undo() {
		for(Sint32 i = (Sint32)m_commands.size() - 1; i >= 0; i--) {
			static_cast<Command*>(m_commands[i])->undo();
		}
	}
	void redo() {
		for(Sint32 i = 0; i < (Sint32)m_commands.size(); i++) {
			static_cast<Command*>(m_commands[i])->redo();
		}
	}
};
