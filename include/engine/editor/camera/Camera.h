#pragma once
#include "engine\utils\Utilities.h"
#include "engine\gfx\texture\MTexture.h"

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

class Camera {
private:
	static glm::vec3 m_position, m_rotation;
	static GLfloat m_zoom, m_tarZoom, m_zoomSpeed;
	static Texture* m_skyTexture;

	static void zoom(GLfloat p_scroll);
	static void turn(Vector2<Sint32> p_mouseMove);
	static void pan(Vector3<GLfloat> p_panDir);

public:
	static void init();
	static void reset();

	static void setPosition(glm::vec3 p_position);

	static glm::vec3 getFocus();
	static glm::vec3 getPosition();
	static glm::vec3 getRotation();
	static glm::vec3 getDirection();
	static glm::vec3 getMouseDirection();

	static glm::mat4 getViewMatrix();
	static glm::mat4 getModelViewProjection();

	static void applyLightDirection();
	static void applyTransformation();

	static void input(Sint8 p_guiFlags);
	static void update(GLfloat p_deltaUpdate);
	static void renderSkybox();
	static void renderFocus();
};
