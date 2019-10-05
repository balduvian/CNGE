#pragma once

#include <gl/glew.h>
#include <GL/GL.h>

namespace CNGE7
{
	void clear(float _r, float _g, float _b, float _a)
	{
		glClearColor(_r, _g, _b, _a);
		glClear(GL_COLOR_BUFFER_BIT);
	}
}
