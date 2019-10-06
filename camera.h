
#pragma once

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
		Camera();

		/// a pos up orthographic projection matrix
		/// for 2d games
		void set_ortho(float _width, float _height);

		/// sets the view based off the transform
		void update();

		/// use this in shaders to get a non moving projection
		float* get_projection();

		/// use this in shaders to get a projection that moves
		/// with the camera
		float* get_projview();

	private:

		float projection_values[16];
		float projview_values[16];

		glm::mat4 projection;
		glm::mat4 projview;

	};
}