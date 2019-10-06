#pragma once

#include <GL/glew.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <memory>
#include <initializer_list>
#include <vector>

namespace CNGE7
{
	class Attribute
	{
	public:
		int per_vertex;
		int size;
		std::unique_ptr<float[]> data;

		int location;

		GLuint buffer;

		Attribute(int _per_vertex, int length, float _data[]);

		/// the vertices in a vao are an attribute
		/// use this function to shortcut the creation
		static Attribute* create_vertex_attribute(int _length, float _data[]);

		~Attribute();
	};

	class VAO
	{
	public:

		VAO(
			int _draw_mode,
			Attribute* _vertex_attrib,
			int _index_count, int _indices[],
			int _attrib_count, Attribute _attribs[]
		);

		~VAO();

		/// actually draws this vao
		void render();

	private:

		GLuint vao;

		int attrib_count;

		/* attributes */
		std::unique_ptr<Attribute> vertex_attrib;
		std::unique_ptr<Attribute[]> attribs;

		int index_count;

		GLuint ibo;

		int draw_mode;

		/// internally adds a vertex attribute to this vao
		/// creates and returns the opengl buffer
		void add_attribute(Attribute& _attrib, int _location);
	};

}
