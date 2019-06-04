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

	static void setBool(const std::string& name, bool value) {
		glUniform1i(glGetUniformLocation(m_current->program, name.c_str()), (int)value);
	}
	static void setInt(const std::string& name, int value) {
		glUniform1i(glGetUniformLocation(m_current->program, name.c_str()), value);
	}
	static void setFloat(const std::string& name, float value) {
		glUniform1f(glGetUniformLocation(m_current->program, name.c_str()), value);
	}
	static void setVec2(const std::string& name, const glm::vec2& value) {
		glUniform2fv(glGetUniformLocation(m_current->program, name.c_str()), 1, &value[0]);
	}
	static void setVec2(const std::string& name, float x, float y) {
		glUniform2f(glGetUniformLocation(m_current->program, name.c_str()), x, y);
	}
	static void setVec3(const std::string& name, const glm::vec3& value) {
		glUniform3fv(glGetUniformLocation(m_current->program, name.c_str()), 1, &value[0]);
	}
	static void setVec3(const std::string& name, float x, float y, float z) {
		glUniform3f(glGetUniformLocation(m_current->program, name.c_str()), x, y, z);
	}
	static void setVec4(const std::string& name, const glm::vec4& value) {
		glUniform4fv(glGetUniformLocation(m_current->program, name.c_str()), 1, &value[0]);
	}
	static void setVec4(const std::string& name, float x, float y, float z, float w) {
		glUniform4f(glGetUniformLocation(m_current->program, name.c_str()), x, y, z, w);
	}
	static void setMat2(const std::string& name, const glm::mat2& value) {
		glUniformMatrix2fv(glGetUniformLocation(m_current->program, name.c_str()), 1, GL_FALSE, &value[0][0]);
	}
	static void setMat3(const std::string& name, const glm::mat3& value) {
		glUniformMatrix3fv(glGetUniformLocation(m_current->program, name.c_str()), 1, GL_FALSE, &value[0][0]);
	}
	static void setMat4(const std::string& name, const glm::mat4& value) {
		glUniformMatrix4fv(glGetUniformLocation(m_current->program, name.c_str()), 1, GL_FALSE, &value[0][0]);
	}

	static void applyModel();
	static void applyView();
	static void applyProjection();

	static void setColor(glm::vec4 p_colorScalar);
	static void setTexture(GLint p_activeTexture, GLint p_textureId);
	static void setTexture(GLint p_activeTexture, std::string p_textureName);
	static void setTextureCoords(glm::vec4& p_subtexCoords);

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
