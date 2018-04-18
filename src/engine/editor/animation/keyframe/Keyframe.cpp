#include "engine\editor\animation\keyframe\Keyframe.h"

std::vector<Keyframe*> MKeyframe::m_keyframes;

void MKeyframe::init() {
	addKeyframe(new Keyframe("Body", 100, 500, Keyframe::TransformationType::TRANSLATION, {}, {0, 5, 0}));
	addKeyframe(new Keyframe("Head", 300, 1000, Keyframe::TransformationType::ROTATION, {}, {45, 45, 0}));
	addKeyframe(new Keyframe("Head", 200, 1000, Keyframe::TransformationType::SCALE, {}, {0.5f, 0.5f, 0.5f}));
	addKeyframe(new Keyframe("Head", 400, 1000, Keyframe::TransformationType::ROTATION, {}, {0, 45, 0}));
}

void MKeyframe::terminate() {
	for(Keyframe* k : m_keyframes)
		delete k;
	m_keyframes.clear();
}

void MKeyframe::addKeyframe(Keyframe* p_keyframe) {
	m_keyframes.push_back(p_keyframe);
	//syncKeyframeList();
}
void MKeyframe::delKeyframe(Sint32 i) {
	if(i == -1) return;
	m_keyframes.erase(m_keyframes.begin() + i);
	//m_keyframeTimeline->setSelectedKeyframe(-1);
	//syncKeyframeList();
}
