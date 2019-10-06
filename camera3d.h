
#pragma once

#include <glm/mat4x4.hpp>

#include "transform3d.h"

namespace CNGE7
{

	class Camera3D
	{
	public:

		float v_fov;
		float aspect;

		Transform3D transform3d;

		/// default initialization
		Camera3D();

		/// a pos up orthographic projection matrix
		/// for 2d games
		void set_perspective(float _v_fov, float _aspect);

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
