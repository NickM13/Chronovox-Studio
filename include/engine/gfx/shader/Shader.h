#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <stack>

#include <GL\glew.h>
#include <glm\glm.hpp>

// General 3d shader use for glPrograms and a Matrix stack
class Shader {
private:
	struct Program {
		GLuint program;

		Program();
		Program* loadShader(GLint p_shaderType, std::string p_shaderFile);
	};
	static Program* m_current;

	static std::map<std::string, Program*> m_shaderPrograms;
	static std::stack<glm::mat4> m_projStack, m_viewStack, m_modelStack;
public:
	static void init();
	static void terminate();
	static Program* getProgram(std::string p_programName);
	static Program* useProgram(std::string p_programName);
	static void use(GLuint p_program);

	static void applyModel();
	static void applyView();
	static void applyProjection();

	static void setColor(glm::vec4 p_colorScalar);
	static void setTexture(GLint p_activeTexture, GLint p_textureId);
	static void setTexture(GLint p_activeTexture, std::string p_textureName);
	static void setShadowsEnabled(bool p_enabled);
	static void setLightEnabled(bool p_enabled);
	static void setLightDirection(glm::vec3 p_light);

	static glm::mat4 getMVP();

	static void pushMatrixModel();
	static glm::mat4& getMatrixModel();
	// Transforms top matrix
	static void transformModel(glm::mat4 p_matrix);
	// Translate model matrix
	static void translate(const glm::vec3& p_value);
	// Rotate model matrix
	// * Degrees
	static void rotate(GLfloat p_angle, const glm::vec3& p_value);
	// Scale model matrix
	static void scale(const glm::vec3& p_value);
	// Clears Model stack
	static void loadIdentityModel();
	static void popMatrixModel();

	static void pushMatrixView();
	static glm::mat4& getMatrixView();
	// Transforms top matrix
	static void transformView(glm::mat4 p_matrix);
	// Clears View stack
	static void loadIdentityView();
	static void popMatrixView();

	static void pushMatrixProjection();
	static glm::mat4& getMatrixProjection();
	// Transforms top matrix
	static void transformProjection(glm::mat4 p_matrix);
	static void loadIdentityProjection();
	static void popMatrixProjection();
};
