#include "engine\editor\animation\Animation.h"
#include "engine\editor\menu\EditorOverlay.h"
#include "engine\editor\animation\menu\AnimationOverlay.h"
#include <algorithm>

Animation::Animation()
	: TEMode() {
}
Animation::~Animation() {

}
void Animation::init(SimpleModel* p_model) {
	m_model = p_model;
	m_keyframeTimeline = (KeyframeTimeline*)AnimationOverlay::getContainer()->findComponent("TIMELINE_KEYFRAME");
	setAnimationLength(2000);
}

void Animation::loadKeyframeProperties() {
	/*
	if(m_keyframeTimeline->getSelectedKeyframe() == -1 || m_keyframeTimeline->getSelectedKeyframe() >= m_keyframes.size()) return;
	Keyframe* k = m_keyframes.at(m_keyframeTimeline->getSelectedKeyframe());
	if(!k) return;
	AnimationOverlay::getContainer()->findComponent("GUI_DETAILS\\GUI_PROPERTIES\\DROPDOWN_TRANSFORMATION")->setSelectedItem(k->m_transformationType);
	AnimationOverlay::getContainer()->findComponent("GUI_DETAILS\\GUI_PROPERTIES\\TEXTFIELD_MATRIX")->setTitle(k->m_matrix);
	AnimationOverlay::getContainer()->findComponent("GUI_DETAILS\\GUI_PROPERTIES\\FOCUS_X")->setValue(k->m_focus.x);
	AnimationOverlay::getContainer()->findComponent("GUI_DETAILS\\GUI_PROPERTIES\\FOCUS_Y")->setValue(k->m_focus.y);
	AnimationOverlay::getContainer()->findComponent("GUI_DETAILS\\GUI_PROPERTIES\\FOCUS_Z")->setValue(k->m_focus.z);
	AnimationOverlay::getContainer()->findComponent("GUI_DETAILS\\GUI_PROPERTIES\\VALUE_X")->setValue(k->m_value.x);
	AnimationOverlay::getContainer()->findComponent("GUI_DETAILS\\GUI_PROPERTIES\\VALUE_Y")->setValue(k->m_value.y);
	AnimationOverlay::getContainer()->findComponent("GUI_DETAILS\\GUI_PROPERTIES\\VALUE_Z")->setValue(k->m_value.z);
	AnimationOverlay::getContainer()->findComponent("GUI_DETAILS\\GUI_PROPERTIES\\START")->setValue(k->m_start / 1000.f);
	AnimationOverlay::getContainer()->findComponent("GUI_DETAILS\\GUI_PROPERTIES\\LENGTH")->setValue(k->m_length / 1000.f);
	*/
}
void Animation::saveKeyframeProperties() {
	/*
	if(m_keyframeTimeline->getSelectedKeyframe() == -1 || m_keyframeTimeline->getSelectedKeyframe() >= m_keyframes.size()) return;
	Keyframe* k = m_keyframes.at(m_keyframeTimeline->getSelectedKeyframe());
	if(!k) return;
	k->m_transformationType = (Keyframe::TransformationType)AnimationOverlay::getContainer()->findComponent("GUI_DETAILS\\GUI_PROPERTIES\\DROPDOWN_TRANSFORMATION")->getSelectedItem();
	k->m_matrix = AnimationOverlay::getContainer()->findComponent("GUI_DETAILS\\GUI_PROPERTIES\\TEXTFIELD_MATRIX")->getTitle();
	k->m_focus.x = AnimationOverlay::getContainer()->findComponent("GUI_DETAILS\\GUI_PROPERTIES\\FOCUS_X")->getValue();
	k->m_focus.y = AnimationOverlay::getContainer()->findComponent("GUI_DETAILS\\GUI_PROPERTIES\\FOCUS_Y")->getValue();
	k->m_focus.z = AnimationOverlay::getContainer()->findComponent("GUI_DETAILS\\GUI_PROPERTIES\\FOCUS_Z")->getValue();
	k->m_value.x = AnimationOverlay::getContainer()->findComponent("GUI_DETAILS\\GUI_PROPERTIES\\VALUE_X")->getValue();
	k->m_value.y = AnimationOverlay::getContainer()->findComponent("GUI_DETAILS\\GUI_PROPERTIES\\VALUE_Y")->getValue();
	k->m_value.z = AnimationOverlay::getContainer()->findComponent("GUI_DETAILS\\GUI_PROPERTIES\\VALUE_Z")->getValue();
	k->m_start = AnimationOverlay::getContainer()->findComponent("GUI_DETAILS\\GUI_PROPERTIES\\START")->getValue() * 1000;
	k->m_length = AnimationOverlay::getContainer()->findComponent("GUI_DETAILS\\GUI_PROPERTIES\\LENGTH")->getValue() * 1000;
	syncKeyframeList();
	*/
}

void Animation::setAnimationLength(Sint32 p_length) {
	m_animationLength = p_length;
	m_keyframeTimeline->setLength(m_animationLength);
}

void Animation::syncKeyframeList() {
	/*
	m_keyframeTimeline->clear();
	std::sort(m_keyframes.begin(), m_keyframes.end(), [](Keyframe* k1, Keyframe* k2) -> bool { return k1->m_start < k2->m_start; });
	for(Keyframe* k : m_keyframes) {
		m_keyframeTimeline->addKeyframe(k);
		if(k->m_start + k->m_length > m_animationLength)
		setAnimationLength(k->m_start + k->m_length);
	}
	*/
}

void Animation::applyMatrixTransformation(Matrix* p_matrix) {
	Matrix* p = m_model->getMatrix(p_matrix->getParent());
	if(p) applyMatrixTransformation(p);

	p_matrix->applyAnimation(m_keyframeTimeline->getTime());
}

void Animation::inputEditor(Sint8 p_guiFlags) {
	m_model->input(p_guiFlags);
	if(!EditorOverlay::getContainer()->isPaused() && (p_guiFlags & (Sint8)Component::EventFlag::KEYPRESS)) {
		if(GKey::keyPressed(GLFW_KEY_SPACE)) toggleAnimationPlaying();
		else if(GKey::keyPressed(GLFW_KEY_ENTER)) saveKeyframeProperties();
		//else if((GKey::m_keyStates[GLFW_KEY_BACKSPACE] & GKey::KEY_PRESS) || (GKey::m_keyStates[GLFW_KEY_DELETE] & GKey::KEY_PRESS)) delKeyframe(m_keyframeTimeline->getSelectedKeyframe());
	}
}
void Animation::updateEditor(GLfloat p_deltaUpdate) {
	m_model->update(p_deltaUpdate);
}
void Animation::renderEditor() {
	if(m_model) {
		for(Matrix* m : *m_model->getMatrixList()) m->clearAnimation();
		/*
		 * for(Keyframe* k : m_keyframes)
		 * 	if(m_model->getMatrix(k->m_matrix))
		 * 		m_model->getMatrix(k->m_matrix)->addKeyframe(k);
		 */
		for(Matrix* m : *m_model->getMatrixList()) {
			Shader::pushMatrixModel();
			{
				applyMatrixTransformation(m);
				m->renderMatrix();
			}
			Shader::popMatrixModel();
		}
	}
}

void Animation::fileNew() {

}
void Animation::fileOpen() {

}
bool Animation::fileSave() {
	return true;
}
void Animation::fileExit() {

}

void Animation::editUndo() {

}
void Animation::editRedo() {

}

void Animation::viewLoadModel() {
	m_model->open();
}
