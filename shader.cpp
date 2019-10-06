
#include <filesystem>
#include <fstream>
#include <iostream>

#include "shader.h"

namespace CNGE7
{

	const char* Shader::MODEL_NAME = "model";
	const char* Shader::PROJVIEW_NAME = "proj_view";

	/// constructs the shader
	Shader::Shader(const char* _vertex_path, const char* _fragment_path) :
		program(glCreateProgram())
	{
		GLint vertex_shader;
		GLint fragment_shader;

		try
		{
			vertex_shader = load_shader(_vertex_path, GL_VERTEX_SHADER);
			fragment_shader = load_shader(_fragment_path, GL_FRAGMENT_SHADER);
		}
		catch (const char* err)
		{
			std::cout << err << std::endl;
			exit(-1);
		}

		glAttachShader(program, vertex_shader);
		glAttachShader(program, fragment_shader);

		glLinkProgram(program);

		glDetachShader(program, vertex_shader);
		glDetachShader(program, fragment_shader);

		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);

		model_location = get_uniform(MODEL_NAME);
		projview_location = get_uniform(PROJVIEW_NAME);
	}

	/// getter
	int Shader::get_uniform(const char* _name)
	{
		return glGetUniformLocation(program, _name);
	}

	/// start using this shader
	/// after calling this then give other things
	void Shader::enable(float* _model, float* _projview)
	{
		glUseProgram(program);

		give_matrix4(model_location, _model);
		give_matrix4(projview_location, _projview);
	}

	/*
		* GIVERS
		*/

	void Shader::give_float(int _location, float _x)
	{
		glUniform1f(_location, _x);
	}

	void Shader::give_vector2(int _location, float _x, float _y)
	{
		glUniform2f(_location, _x, _y);
	}

	void Shader::give_vector3(int _location, float _x, float _y, float _z)
	{
		glUniform3f(_location, _x, _y, _z);
	}

	void Shader::give_vector4(int _location, float _x, float _y, float _z, float _w)
	{
		glUniform4f(_location, _x, _y, _z, _w);
	}

	void Shader::give_matrix4(int _location, float* _values)
	{
		glUniformMatrix4fv(_location, 1, false, _values);
	}

	/// internally loads a fragment or vertex shader
	/// from a file
	GLuint Shader::load_shader(const char* _path, int _type)
	{
		auto file_path = std::filesystem::path(_path);

		if (std::filesystem::exists(file_path))
		{
			// get the size of the file in bytes
			// add a byte at the end for the null character
			auto size = (GLint)std::filesystem::file_size(file_path) + 1;

			// create the buffer
			// which will be a single line of text essentially
			auto buffer = std::make_unique<char[]>(size);

			// get a pointer to the buffer
			auto buf_ptr = buffer.get();

			// a pointer to buffer that can iterate
			auto buf_itr = buf_ptr - 1;

			auto file = std::ifstream{};

			file.open(file_path);

			// read the whole file into buffer
			while (file.good())
				* ++buf_itr = file.get();

			// add null character
			*buf_itr = 0;

			// close file reading
			file.close();

			// create the shader to return
			auto shader = glCreateShader(_type);

			// put the shader into opengl
			glShaderSource(shader, 1, &buf_ptr, nullptr);
			glCompileShader(shader);

			// see if the shader is properly compiled
			GLint status;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

			if (status != GL_TRUE)
			{
				// create a buffer for the info log
				constexpr auto err_size = 256;
				char err_buffer[err_size];

				int len_return;
				glGetShaderInfoLog(shader, err_size, &len_return, err_buffer);

				throw err_buffer;
			}

			return shader;
		}
		else
			throw "file does not exist";
	}

}
