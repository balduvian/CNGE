
#include <iostream>

#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "transform.h"

namespace CNGE7
{
	float Transform::values[16] {};

	float Transform::default_model[16]
	{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	float Transform::default_projview[16]
	{
		2, 0, 0, 0,
		0, 2, 0, 0,
		0, 0, 1, 0,
		-1, -1, 0, 1
	};

	/// default initialization
	Transform::Transform() : translation(0, 0, 0), scale(1, 1, 1), rotation(0) {}

	/// returns the float values for a model matrix based on this transform
	float* Transform::to_model()
	{
		// transform a matrix with the values stored in this transform
		auto mat = glm::scale(glm::rotate(glm::translate(translation), rotation, glm::vec3(0, 0, 1)), scale);

		// copy down the values
		copy_mat(mat, values);

		// return static value array
		return values;
	}

	/// returns the float values for a model matrix based on given params
	/// no instance of a transform needed
	/// no rotation
	float* Transform::to_model(float x, float y, float width, float height)
	{
		// transform a matrix with the values stored in this transform
		auto mat = glm::scale(glm::translate(glm::vec3(x, y, 0)), glm::vec3(width, height, 1));

		// copy down the values
		copy_mat(mat, values);

		// return static value array
		return values;
	}

	/// modifies a mat4 based on this transform
	void Transform::transform(glm::mat4& _mat)
	{
		glm::scale(glm::rotate(glm::translate(_mat, translation), rotation, glm::vec3(0, 0, 1)), scale);
	}

	/// modify this matrix for the camera, everything is negative
	glm::mat4 Transform::camera_transform()
	{
		return glm::scale(glm::rotate(glm::translate(-translation), -rotation, glm::vec3(0, 0, 1)), scale);
	}

	/// puts all the values from a mat4 into
	/// a usable float array
	void Transform::copy_mat(glm::mat4& _from_mat, float* _to_values)
	{
		/// prepare pointers 1 behind
		--_to_values;
		auto mat_ptr = glm::value_ptr(_from_mat) - 1;

		/// copy 16 values from a mat4
		for (auto i = 0; i < 16; ++i)
		{
			*++_to_values = *++mat_ptr;
		}
	}

	/// for tesing purposes only
	void Transform::__DEBUG_PRINT(float* _mat)
	{
		for (auto i = 0; i < 4; ++i)
		{
			for (auto j = 0; j < 4; ++j)
			{
				std::cout << _mat[i * 4 + j] << " ";
			}

			std::cout << std::endl;
		}
	}

}
