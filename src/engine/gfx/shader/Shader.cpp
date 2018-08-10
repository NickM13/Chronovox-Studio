#include "engine\gfx\shader\Shader.h"
#include "engine\gfx\texture\MTexture.h"
#include "engine\utils\directory\LDirectory.h"
#include <glm\gtc\matrix_transform.hpp>

Shader::Program* Shader::m_current = 0;

std::map<std::string, Shader::Program*> Shader::m_shaderPrograms;
std::stack<glm::mat4> Shader::m_projStack, Shader::m_viewStack, Shader::m_modelStack;

Shader::Program::Program() {
	program = glCreateProgram();
}

Shader::Program* Shader::Program::loadShader(GLint p_shaderType, std::string p_shaderFile) {
	p_shaderFile = LDirectory::getProjectPath() + "res\\shader\\" + p_shaderFile;
	std::string code;
	std::ifstream shaderFile;
	shaderFile.exceptions(std::ifstream::badbit);
	try {
		
		shaderFile.open(p_shaderFile.c_str());
		std::stringstream shaderStream;
		shaderStream << shaderFile.rdbuf();
		shaderFile.close();
		code = shaderStream.str();
	}
	catch(std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const GLchar* shaderCode = code.c_str();

	GLuint shader;
	GLint success;
	GLchar infoLog[512];
	shader = glCreateShader(p_shaderType);
	glShaderSource(shader, 1, &shaderCode, NULL);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	glAttachShader(program, shader);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if(!success) {
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(shader);
	return this;
}

void Shader::init() {
	loadIdentityProjection();
	loadIdentityView();
	loadIdentityModel();
}
void Shader::terminate() {

}

Shader::Program* Shader::getProgram(std::string p_programName) {
	if(m_shaderPrograms.find(p_programName) == m_shaderPrograms.end()) {
		Program* p = new Program();
		m_shaderPrograms.emplace(p_programName, p);
	}
	return m_shaderPrograms.at(p_programName);
}

Shader::Program* Shader::useProgram(std::string p_programName) {
	Program* p = m_shaderPrograms.at(p_programName);
	glUseProgram(p->program);
	m_current = p;
	return p;
}

void Shader::use(GLuint p_program) {
	glUseProgram(p_program);
	m_current = 0;
}

void Shader::applyModel() {
	glUniformMatrix4fv(2, 1, GL_FALSE, &m_modelStack.top()[0][0]);
}
void Shader::applyView() {
	glUniformMatrix4fv(1, 1, GL_FALSE, &m_viewStack.top()[0][0]);
}
void Shader::applyProjection() {
	glUniformMatrix4fv(0, 1, GL_FALSE, &m_projStack.top()[0][0]);
}

void Shader::setColor(glm::vec4 p_colorScalar) {
	glUniform4fv(3, 1, &p_colorScalar.x);
}
void Shader::setTexture(GLint p_activeTexture, GLint p_textureId) {
	glActiveTexture(GL_TEXTURE0 + p_activeTexture);
	glBindTexture(GL_TEXTURE_2D, p_textureId);
}
void Shader::setTexture(GLint p_activeTexture, std::string p_textureName) {
	glActiveTexture(GL_TEXTURE0 + p_activeTexture);
	glBindTexture(GL_TEXTURE_2D, MTexture::getTexture(p_textureName)->getGlId());
}

void Shader::setShadowsEnabled(bool p_enabled) {
	glUniform1i(20, (GLint)p_enabled);
}
void Shader::setLightEnabled(bool p_enabled) {
	glUniform1i(6, (GLint)p_enabled);
}
void Shader::setLightDirection(glm::vec3 p_light) {
	glUniform3f(5, p_light.x, p_light.y, p_light.z);
	glUniform1i(6, 1);
}

glm::mat4 Shader::getMVP() {
	return getMatrixProjection() * getMatrixView() * getMatrixModel();
}

void Shader::pushMatrixModel() {
	if(m_modelStack.empty())	m_modelStack.push(glm::mat4(1.f));
	else						m_modelStack.push(glm::mat4(1.f) * m_modelStack.top());
}
glm::mat4& Shader::getMatrixModel() {
	return m_modelStack.top();
}
void Shader::transformModel(glm::mat4 p_matrix) {
	m_modelStack.top() *= p_matrix;
}
void Shader::translate(const glm::vec3& p_value) {
	m_modelStack.top() *= glm::translate(glm::mat4(), p_value);
}
void Shader::rotate(GLfloat p_angle, const glm::vec3& p_value) {
	m_modelStack.top() *= glm::rotate(glm::mat4(), glm::radians(p_angle), p_value);
}
void Shader::scale(const glm::vec3& p_value) {
	m_modelStack.top() *= glm::scale(glm::mat4(), p_value);
}
void Shader::loadIdentityModel() {
	while(m_modelStack.size() > 0)
		m_modelStack.pop();
	m_modelStack.push(glm::mat4(1.f));
}
void Shader::popMatrixModel() {
	m_modelStack.pop();
}

void Shader::pushMatrixView() {
	if(m_viewStack.empty())		m_viewStack.push(glm::mat4(1.f));
	else						m_viewStack.push(glm::mat4(1.f) * m_viewStack.top());
}
glm::mat4& Shader::getMatrixView() {
	return m_viewStack.top();
}
void Shader::transformView(glm::mat4 p_matrix) {
	m_viewStack.top() = m_viewStack.top() * p_matrix;
}
void Shader::loadIdentityView() {
	while(m_viewStack.size() > 0)
		m_viewStack.pop();
	m_viewStack.push(glm::mat4(1.f));
}
void Shader::popMatrixView() {
	m_viewStack.pop();
}

void Shader::pushMatrixProjection() {
	if(m_projStack.empty())		m_projStack.push(glm::mat4(1.f));
	else						m_projStack.push(glm::mat4(1.f) * m_projStack.top());
}
glm::mat4& Shader::getMatrixProjection() {
	return m_projStack.top();
}
void Shader::transformProjection(glm::mat4 p_matrix) {
	m_projStack.top() = m_projStack.top() * p_matrix;
}
void Shader::loadIdentityProjection() {
	while(m_projStack.size() > 0)
		m_projStack.pop();
	m_projStack.push(glm::mat4(1.f));
}
void Shader::popMatrixProjection() {
	m_projStack.pop();
}
