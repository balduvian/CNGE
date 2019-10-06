#pragma once

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include "transform.h"

#include "transform3d.h"

namespace CNGE7
{
	/*
	 * initializers for the default model and projection value arrays
	 */

	glm::vec3 translation;
	glm::vec3 scale;
	glm::vec3 rotation;

	/// default initialization
	Transform3D::Transform3D() : translation(0, 0, 0), scale(1, 1, 1), rotation(0, 0, 0) {}

	/// returns the float values for a model matrix based on this transform
	float* Transform3D::to_model()
	{
		// transform a matrix with the values stored in this transform
		auto mat = glm::scale(glm::rotate(glm::rotate(glm::rotate(glm::translate(translation), rotation.x, glm::vec3(1, 0, 0)), rotation.y, glm::vec3(0, 1, 0)), rotation.z, glm::vec3(0, 0, 1)), scale);

		// copy down the values
		Transform::copy_mat(mat, Transform::values);

		// return static value array
		return Transform::values;
	}

	/// returns the float values for a model matrix based on given params
	/// no instance of a transform needed
	/// no rotation
	float* Transform3D::to_model(float _x, float _y, float _z, float _width, float _height, float _depth)
	{
		// transform a matrix with the values stored in this transform
		auto mat = glm::scale(glm::translate(glm::vec3(_x, _y, _z)), glm::vec3(_width, _height, _depth));

		// copy down the values
		Transform::copy_mat(mat, Transform::values);

		//Transform::__DEBUG_PRINT(Transform::values);

		// return static value array
		return Transform::values;
	}

	/// modifies a mat4 based on this transform
	void Transform3D::transform(glm::mat4& _mat)
	{
		glm::translate(glm::scale(glm::rotate(glm::rotate(glm::rotate(_mat, rotation.x, glm::vec3(1, 0, 0)), rotation.y, glm::vec3(0, 1, 0)), rotation.z, glm::vec3(0, 0, 1)), scale), translation);
	}

	/// modify this matrix for the camera, everything is negative
	glm::mat4 Transform3D::camera_transform()
	{
		return glm::translate(glm::scale(glm::rotate(glm::rotate(glm::rotate(rotation.x, glm::vec3(1, 0, 0)), rotation.y, glm::vec3(0, 1, 0)), rotation.z, glm::vec3(0, 0, 1)), scale), -translation);
	}

}
