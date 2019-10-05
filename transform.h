#pragma once

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

namespace CNGE7
{
	class Transform
	{
	public:

		/*
		 * initializers for the default model and projection value arrays
		 */

		static float default_model[16];
		static float default_projview[16];

		glm::vec3 translation;
		glm::vec3 scale;
		float rotation;

		/// default initialization
		Transform() : translation(0, 0, 0), scale(1, 1, 1), rotation(0) {}

		/// returns the float values for a model matrix based on this transform
		float* to_model()
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
		static float* to_model(float x, float y, float width, float height)
		{
			// transform a matrix with the values stored in this transform
			auto mat = glm::scale(glm::translate(glm::vec3(x, y, 0)), glm::vec3(width, height, 1));

			// copy down the values
			copy_mat(mat, values);

			// return static value array
			return values;
		}

		/// modifies a mat4 based on this transform
		void transform(glm::mat4 &_mat)
		{
			glm::scale(glm::rotate(glm::translate(_mat, translation), rotation, glm::vec3(0, 0, 1)), scale);
		}

		/// modify this matrix for the camera, everything is negative
		void camera_transform(glm::mat4 &_mat)
		{
			glm::scale(glm::rotate(glm::translate(_mat, -translation), -rotation, glm::vec3(0, 0, 1)), scale);
		}

		static void copy_mat(glm::mat4 &_from_mat, float* _to_values)
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

		static void __DEBUG_PRINT(float* _mat)
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
		
	private:
		
		/// used for returning model matrix values
		static float values[16];
	};
}