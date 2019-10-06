
#include <glm/gtc/matrix_transform.hpp>

#include "transform.h"

#include "camera3d.h"

namespace CNGE7
{
	/// default initialization
	Camera3D::Camera3D() : transform3d(), projection(), projview(), v_fov(0), aspect(0), projection_values(), projview_values() {}

	/// a pos up orthographic projection matrix
	/// for 2d games
	void Camera3D::set_perspective(float _v_fov, float _aspect)
	{
		v_fov = _v_fov;
		aspect = _aspect;

		projection = glm::perspective(v_fov, aspect, 0.0001f, 256.f);

		/// copy values
		Transform::copy_mat(projection, projection_values);
	}

	/// sets the view based off the transform
	void Camera3D::update()
	{
		projview = transform3d.camera_transform();
		projview = projection * projview;

		Transform::copy_mat(projview, projview_values);
	}

	/// use this in shaders to get a non moving projection
	float* Camera3D::get_projection()
	{
		return projection_values;
	}

	/// use this in shaders to get a projection that moves
	/// with the camera
	float* Camera3D::get_projview()
	{
		return projview_values;
	}

}
