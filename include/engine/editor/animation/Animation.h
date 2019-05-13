#pragma once
#include "keyframe\Keyframe.h"
#include "..\TEMode.h"
#include "..\model\SimpleModel.h"
#include "engine\gfx\gui\LGui.h"
#include "menu\KeyframeTimeline.h"

class Animation : public TEMode {
private:
	SimpleModel* m_model; // Visual aid for animation, animation skeletons are based on matrices

	std::string m_animationName;
	Sint32 m_animationLength;
	Sint32 m_animationLoopStart;

	KeyframeTimeline* m_keyframeTimeline;

	void applyMatrixTransformation(Matrix* p_matrix);
public:
	Animation();
	~Animation();
	void init(SimpleModel* p_model);
	void setTimeline(KeyframeTimeline* p_keyframeTimeline) { m_keyframeTimeline = p_keyframeTimeline; }
	KeyframeTimeline* getTimeline() { return m_keyframeTimeline; }

	void loadKeyframeProperties();
	void saveKeyframeProperties();

	void setAnimationPlaying(bool p_playing) { m_keyframeTimeline->setPlaying(p_playing); }
	void toggleAnimationPlaying() { m_keyframeTimeline->setPlaying(!m_keyframeTimeline->isPlaying()); }
	void setAnimationLength(Sint32 p_length);

	void syncKeyframeList();

	void inputEditor(Sint8 p_guiFlags);
	void updateEditor(GLfloat p_deltaUpdate);
	void renderEditor();

	void fileNew();
	void fileOpen();
	bool fileSave();
	void fileExit();

	void editUndo();
	void editRedo();

	void viewLoadModel();
};
