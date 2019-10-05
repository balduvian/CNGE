#pragma once

#include <GL\glew.h>
#include <GL\wglew.h>
#include <gl/gl.h>
#include <string>
#include <functional>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>

#include "transform.h"

namespace CNGE7
{
	class Camera
	{
	public:

		float width;
		float height;

		Transform transform;

		/// default initialization
		Camera() : transform(), projection(), projview(), width(0), height(0), projection_values(), projview_values() {}

		/// a pos up orthographic projection matrix
		/// for 2d games
		void set_ortho(float _width, float _height)
		{
			width = _width;
			height = _height;

			projection = glm::ortho(.0f, _width, .0f, _height);

			/// copy values
			Transform::copy_mat(projection, projection_values);
		}

		/// sets the view based off the transform
		void update()
		{
			projview = projection;

			transform.camera_transform(projview);

			Transform::copy_mat(projview, projview_values);
		}

		/// use this in shaders to get a non moving projection
		float* get_projection()
		{
			return projection_values;
		}

		/// use this in shaders to get a projection that moves
		/// with the camera
		float* get_projview()
		{
			return projview_values;
		}

	private:

		float projection_values[16];
		float projview_values[16];

		glm::mat4 projection;
		glm::mat4 projview;

	};
}