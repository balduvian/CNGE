#pragma once

#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/GLU.h>
#include <filesystem>
#include <fstream>

namespace CNGE7
{
	class Shader
	{
	public:

		const char* MODEL_NAME = "model";
		const char* PROJVIEW_NAME = "proj_view";

		/// constructs the shader
		Shader(const char* _vertex_path, const char* _fragment_path, const char** _uniforms)
		{
			auto program = glCreateProgram();

			auto vertex_shader = load_shader(_vertex_path, GL_VERTEX_SHADER);
			auto fragment_shader = load_shader(_fragment_path, GL_FRAGMENT_SHADER);

			glAttachShader(program, vertex_shader);
			glAttachShader(program, fragment_shader);

			glLinkProgram(program);

			glDetachShader(program, vertex_shader);
			glDetachShader(program, fragment_shader);

			glDeleteShader(vertex_shader);
			glDeleteShader(fragment_shader);

			model_location = getUniform(MODEL_NAME);
			model_location = getUniform(PROJVIEW_NAME);
		}

		/// getter
		int getUniform(const char* _name)
		{
			return glGetUniformLocation(program, _name);
		}

		/// start using this shader
		/// after calling this then give other things
		void enable(float* _model, float* _projview)
		{
			glUseProgram(program);

			give_matrix4(model_location, _model);
			give_matrix4(projview_location, _projview);
		}

		/*
		 * GIVERS
		 */

		void give_float(int _location, float _x)
		{
			glUniform1f(_location, _x);
		}

		void give_vector2(int _location, float _x, float _y)
		{
			glUniform2f(_location, _x, _y);
		}

		void give_vector3(int _location, float _x, float _y, float _z)
		{
			glUniform3f(_location, _x, _y, _z);
		}

		void give_vector4(int _location, float _x, float _y, float _z, float _w)
		{
			glUniform4f(_location, _x, _y, _z, _w);
		}

		void give_matrix4(int _location, float* _values)
		{
			glUniformMatrix4fv(_location, 16, false, _values);
		}

	private:

		GLuint program;

		GLint model_location;
		GLint projview_location;

		/// internally loads a fragment or vertex shader
		/// from a file
		GLuint load_shader(const char* _path, int _type)
		{
			auto file_path = std::filesystem::path(_path);

			if (std::filesystem::exists(file_path))
			{
				// get the size of the file in bytes
				auto size = (GLint)std::filesystem::file_size(file_path);

				// create the buffer
				// which will be a single line of text essentially
				auto buffer = std::make_unique<char[]>(size);

				// get a pointer to the buffer
				auto buf_ptr = buffer.get();

				auto file = std::ifstream{};

				file.open(file_path);

				// read the whole file into buffer
				while (file.good())
					*buf_ptr++ = file.get();

				// close file reading
				file.close();

				// create the shader to return
				auto shader = glCreateShader(_type);

				// put the shader into opengl
				glShaderSource(shader, size, &buf_ptr, &size);
				glCompileShader(shader);
				
				// see if the shader is properly compiled
				GLint status;
				glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

				if (status != GL_TRUE)
				{
					// create a buffer for the info log
					constexpr auto err_size = 128;
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
	};
}
