
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
		std::unique_ptr<float[]> data;
		GLuint buffer;

		Attribute(int _per_vertex, float* _data)
		{
			per_vertex = _per_vertex;
			data = std::unique_ptr<float[]>(_data);

			glGenBuffers(1, &buffer);
		}
	};

	class VAO
	{
	public:

		VAO()
		{
			glCreateVertexArrays(1, &vao);

			auto at = Attribute(3, new float[2]{0.0f,1.0f});
		}

		void add_attribute(Attribute attribute)
		{

		}
	private:
		GLuint vao;
		GLuint ibo;

		int vertex_count;
		int attrib_count;

		std::unique_ptr<Attribute> attribs;

		int draw_mode;
	};
}
