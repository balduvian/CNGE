#pragma once

#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/GLU.h>
#include <filesystem>
#include <fstream>

#include <iostream>

namespace CNGE7
{
	class Shader
	{
	public:

		static const char* MODEL_NAME;
		static const char* PROJVIEW_NAME;

		/// constructs the shader
		Shader(const char* _vertex_path, const char* _fragment_path);

		/// getter
		int get_uniform(const char* _name);

		/// start using this shader
		/// after calling this then give other things
		void enable(float* _model, float* _projview);

		/*
		 * GIVERS
		 */

		void give_float(int _location, float _x);

		void give_vector2(int _location, float _x, float _y);

		void give_vector3(int _location, float _x, float _y, float _z);

		void give_vector4(int _location, float _x, float _y, float _z, float _w);

		void give_matrix4(int _location, float* _values);

	private:

		GLuint program;

		GLint model_location;
		GLint projview_location;

		/// internally loads a fragment or vertex shader
		/// from a file
		GLuint load_shader(const char* _path, int _type);

	};

}
