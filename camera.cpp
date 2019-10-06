#pragma once

#include <GL\glew.h>
#include <GL\wglew.h>
#include <gl/gl.h>
#include <string>
#include <functional>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include "glm/gtc/matrix_transform.hpp"

#include "transform.h"

#include "camera.h"

namespace CNGE7
{
	/// default initialization
	Camera::Camera() : transform(), projection(), projview(), width(0), height(0), projection_values(), projview_values() {}

	/// a pos up orthographic projection matrix
	/// for 2d games
	void Camera::set_ortho(float _width, float _height)
	{
		width = _width;
		height = _height;

		projection = glm::ortho(.0f, _width, .0f, _height);

		/// copy values
		Transform::copy_mat(projection, projection_values);
	}

	/// sets the view based off the transform
	void Camera::update()
	{
		projview = transform.camera_transform();
		projview *= projection;

		Transform::copy_mat(projview, projview_values);
	}

	/// use this in shaders to get a non moving projection
	float* Camera::get_projection()
	{
		return projection_values;
	}

	/// use this in shaders to get a projection that moves
	/// with the camera
	float* Camera::get_projview()
	{
		return projview_values;
	}

}
