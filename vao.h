
#include <GL/glew.h>
#include <gl/GL.h>
#include <gl/GLU.h>

namespace CNGE7
{
	class Attribute
	{
	public:

	private:

	};

	class VAO
	{
	public:

		VAO()
		{
			glCreateVertexArrays(1, &vao);

		}

		void add_attribute(Attribute attribute)
		{

		}
	private:
		GLuint vao;
	};
}
