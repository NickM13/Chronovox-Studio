#pragma once

#include "..\..\command\Command.h"
#include "..\keyframe\Keyframe.h"

class KDeleteCommand : public Command {
public:
	KDeleteCommand(Keyframe* p_keyframe, std::vector<Keyframe*>& p_keyframes, Uint16 p_index) {
		m_commandType = "KDeleteCommand";
		m_keyframe = p_keyframe;
		m_keyframes = &p_keyframes;
		m_index = p_index;
		m_staticChange = false;
	}
	~KDeleteCommand() {
		delete m_keyframe;
	}
	void undo() {
		MKeyframe::addKeyframe(m_keyframe);
	}
	void redo() {

	}

private:
	Keyframe* m_keyframe;
	std::vector<Keyframe*>* m_keyframes;
	Uint16 m_index;
};
