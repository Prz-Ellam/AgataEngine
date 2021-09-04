#include <GL/glew.h>
#include "Shader.h"
#include "Log.h"
#include <sstream>
#include <fstream>
#include <vector>

namespace Agata {

	Shader::Shader(const std::string& vertexFilePath, const std::string& fragmentFilePath) : m_ID(0) {

		m_ID = glCreateProgram();
		uint32_t vs = createShader(GL_VERTEX_SHADER, vertexFilePath);
		uint32_t fs = createShader(GL_FRAGMENT_SHADER, fragmentFilePath);

		glAttachShader(m_ID, vs);
		glAttachShader(m_ID, fs);
		glLinkProgram(m_ID);
		glValidateProgram(m_ID);

		glDeleteShader(vs);
		glDeleteShader(fs);

	}

	Shader::Shader(Shader&& other) noexcept {

		this->m_ID = other.m_ID;
		this->m_UniformLocationsCache = other.m_UniformLocationsCache;
		other.m_ID = 0;

	}

	Shader::~Shader() {

		glDeleteProgram(m_ID);

	}

	std::string Shader::readShaderFile(const std::string& filePath) {

		std::ifstream file(filePath);

		if (!file.is_open()) {
			Log::error("El shader no pudo ser leido");
		}

		std::string buffer;
		std::stringstream source;

		while (!file.eof()) {
			std::getline(file, buffer);
			buffer += '\n';
			source << buffer;
		}

		buffer = source.str();
		return buffer;

	}

	uint32_t Shader::createShader(uint32_t shaderType, const std::string& filePath) {

		uint32_t shaderID = glCreateShader(shaderType);
		std::string sSource = readShaderFile(filePath);
		const char* cSource = sSource.c_str();
		glShaderSource(shaderID, 1, &cSource, nullptr);
		glCompileShader(shaderID);

		int sucess;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &sucess);
		if (!sucess) {
			int length;
			glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);
			std::vector<char> infoLog(length);
			glGetShaderInfoLog(shaderID, 512, nullptr, infoLog.data());
			Log::error("Error de compilacion");
			(shaderType == GL_VERTEX_SHADER) ? Log::error("Tipo: Vertex Shader ") : Log::error("Tipo: Fragment Shader ");
			Log::error("%s", infoLog.data());
		}

		return shaderID;

	}

	void Shader::bind() const {

		glUseProgram(m_ID);

	}

	void Shader::unbind() const {

		glUseProgram(0);

	}

	int Shader::getUniformLocation(const std::string& uniform) {

		if (m_UniformLocationsCache.find(uniform) != m_UniformLocationsCache.end()) {
			return m_UniformLocationsCache[uniform];
		}

		int location = glGetUniformLocation(m_ID, uniform.c_str());
		m_UniformLocationsCache[uniform] = location;
		return location;

	}

	void Shader::sendFloat(const std::string& uniform, float value) {

		glUniform1f(getUniformLocation(uniform), value);

	}

	void Shader::sendVec3(const std::string& uniform, const glm::vec3& value) {

		glUniform3f(getUniformLocation(uniform.c_str()), value.x, value.y, value.z);

	}

	void Shader::sendVec4(const std::string& uniform, const glm::vec4& value) {

		glUniform4f(getUniformLocation(uniform.c_str()), value.x, value.y, value.z, value.w);

	}

	void Shader::sendInt1(const std::string& uniform, int value) {

		glUniform1i(getUniformLocation(uniform), value);

	}

	void Shader::sendIntArray(const std::string& uniform, const int* data, size_t count) {

		glUniform1iv(getUniformLocation(uniform), count, data);

	}

	void Shader::sendMat4(const std::string& uniform, const glm::mat4& value) {

		glUniformMatrix4fv(getUniformLocation(uniform), 1, GL_FALSE, glm::value_ptr(value));

	}

}