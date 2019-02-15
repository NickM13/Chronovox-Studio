#pragma once
#include "engine\utils\Utilities.h"

struct Keyframe {
public:
	Sint32 id;
	std::string m_matrix;
	GLfloat m_start;
	GLfloat m_length;
	enum TransformationType {
		TRANSLATION = 0,
		ROTATION = 1,
		SCALE = 2
	} m_transformationType;
	Vector3<GLfloat> m_focus;
	Vector3<GLfloat> m_value;

	Keyframe() {}

	Keyframe(std::string p_matrix, GLfloat p_start, GLfloat p_length, TransformationType p_transformationType, Vector3<GLfloat> p_focus, Vector3<GLfloat> p_value)
		: m_matrix(p_matrix), m_start(p_start), m_length(p_length), m_transformationType(p_transformationType), m_focus(p_focus), m_value(p_value) {

	}

	bool operator<(Keyframe k) {
		return m_start < k.m_start;
	}
};

class MKeyframe {
private:
	static std::vector<Keyframe*> m_keyframes;
public:
	static void init();
	static void terminate();

	static void addKeyframe(Keyframe* p_keyframe);
	static void delKeyframe(Sint32 i);
};
