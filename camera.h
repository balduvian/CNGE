#pragma once

#include <GL\glew.h>
#include <GL\wglew.h>
#include <gl/gl.h>
#include <string>
#include <functional>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>

namespace CNGE7
{
	class Camera
	{
	public:

		glm::mat4 projection;
		glm::mat4 projview;

	private:



	};
}