
#include <filesystem>
#include <fstream>
#include <iostream>

#include "shader.h"

#include "cnge8/engine/transform.h"

namespace CNGE {

	/// constructs the shader
	Shader::Shader(const char* vertexPath, const char* fragmentPath)
	: Resource(true), program(), vertexPath(vertexPath), fragmentPath(fragmentPath), vertexData(nullptr), fragmentData(nullptr), modelLocation(), projviewLocation() {}

	auto Shader::customGather() -> LoadError {
		auto readFile = [](const char* path, LoadError& error) -> char* {
			auto filePath = std::filesystem::path(path);
			if (!std::filesystem::exists(filePath))
				return error.makeErr(std::string("source file ") + path + " does not exist"), nullptr;

			/* extra byte for null terminator */
			auto size = std::filesystem::file_size(filePath) + 1_u32;
			auto buffer = new char[size];

			auto stream = std::ifstream(filePath, std::ios::binary);
			if (stream.bad())
				return error.makeErr(std::string("error encountered while reading source file ") + path), nullptr;

			stream.read(buffer, size - 1);
			if (stream.bad())
				return error.makeErr(std::string("error encountered while reading source file ") + path), nullptr;

			buffer[size - 1] = '\0';

			return buffer;
		};

		auto error = LoadError::none();

		vertexData = readFile(vertexPath, error);
		fragmentData = readFile(fragmentPath, error);

		return error;
	}

	auto Shader::customProcess() -> LoadError {
		auto loadShader = [](char* data, int type) -> u32 {
			auto shader = glCreateShader(type);

			// put the shader into opengl
			glShaderSource(shader, 1, &data, nullptr);
			glCompileShader(shader);

			// see if the shader is properly compiled
			auto status = GLint();
			glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

			if (status != GL_TRUE)
				return printShaderError(shader), false;

			return shader;
		};

		program = glCreateProgram();

		GLint vertex_shader;
		GLint fragment_shader;

		vertex_shader = loadShader(vertexData, GL_VERTEX_SHADER);
		fragment_shader = loadShader(fragmentData, GL_FRAGMENT_SHADER);

		glAttachShader(program, vertex_shader);
		glAttachShader(program, fragment_shader);

		glLinkProgram(program);

		glDetachShader(program, vertex_shader);
		glDetachShader(program, fragment_shader);

		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);

		// load base uniforms
		modelLocation = getUniform(MODEL_NAME);
		projviewLocation = getUniform(PROJVIEW_NAME);

		// now load custom uniforms
		getUniforms();

		return LoadError::none();
	}

	auto Shader::customUnload() -> LoadError {
		glDeleteProgram(program);

		return LoadError::none();
	}

	auto Shader::customDiscard() -> LoadError {
		delete[] vertexData;
		delete[] fragmentData;

		return LoadError::none();
	}

	auto Shader::printShaderError(GLuint shader) -> void {
		constexpr auto err_size = 256;
		char err_buffer[err_size];

		auto len_return = i32();
		glGetShaderInfoLog(shader, err_size, &len_return, err_buffer);

		std::cout << "CNGE SHADER ERROR | " << err_buffer << std::endl;
	}

	/*
	 * use
	 */

	auto Shader::getUniform(const char* _name) -> i32 {
		return glGetUniformLocation(program, _name);
	}

	/// start using this shader
	/// after calling this then give other things
	auto Shader::enable(f32 model[], f32 projview[]) -> void {
		glUseProgram(program);

		giveMatrix4(modelLocation, model);
		giveMatrix4(projviewLocation, projview);
	}

	auto Shader::enable() -> void {
		glUseProgram(program);

		giveMatrix4(modelLocation, Transform::defaultModel);
		giveMatrix4(projviewLocation, Transform::defaultProjview);
	}

	/*
	 * GIVERS
	 */

	auto Shader::giveFloat(const int location, const float x) -> void {
		glUniform1f(location, x);
	}

	auto Shader::giveVector2(const int location, const float x, const float y) -> void {
		glUniform2f(location, x, y);
	}

	auto Shader::giveVector3(const int location, const float x, const float y, const float z) -> void {
		glUniform3f(location, x, y, z);
	}

	auto Shader::giveVector4(const int location, const float x, const float y, const float z, const float w) -> void {
		glUniform4f(location, x, y, z, w);
	}

	auto Shader::giveVector4(const int location, const float values[]) -> void {
		glUniform4fv(location, 1, values);
	}

	auto Shader::giveMatrix4(const int location, const float values[]) -> void {
		glUniformMatrix4fv(location, 1, false, values);
	}

	Shader::~Shader() {
		destroy();
	}
}
