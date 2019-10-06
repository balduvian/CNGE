
#include <gl/glew.h>
#include <GL/GL.h>
#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>

namespace CNGE7
{
	void clear(float _r, float _g, float _b, float _a)
	{
		glClearColor(_r, _g, _b, _a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void winding_order(bool _counter_clockwise)
	{
		glFrontFace(_counter_clockwise ? GL_CCW : GL_CW);
	}

	/// automatically enables cull face
	/// also sets which faces to cull
	void set_cull(bool _front, bool _back)
	{
		if (_front)
		{
			glEnable(GL_CULL_FACE);

			if (_back)
				glCullFace(GL_FRONT_AND_BACK);
			else
				glCullFace(GL_FRONT);
		}
		else if (_back)
		{
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
		}
		else
			glDisable(GL_CULL_FACE);
	}

	void default_enables()
	{
		//blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		glEnable(GL_CLIP_DISTANCE0);

		glEnable(GL_TEXTURE_2D);
	}

	float noise_map(float _x, float _y)
	{
		return glm::perlin(glm::vec2(_x, _y));
	}

	float mod(float i, float n) {
		return fmod(n + fmod(i, n), n);
	}

	float PI = 3.14159265359f;
}
