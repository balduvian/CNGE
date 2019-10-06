
#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include "transform3d.h"

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
		Transform();

		/// returns the float values for a model matrix based on this transform
		float* to_model();

		/// returns the float values for a model matrix based on given params
		/// no instance of a transform needed
		/// no rotation
		static float* to_model(float x, float y, float width, float height);

		/// modifies a mat4 based on this transform
		void transform(glm::mat4& _mat);

		/// modify this matrix for the camera, everything is negative
		glm::mat4 camera_transform();

		static void copy_mat(glm::mat4& _from_mat, float* _to_values);

		static void __DEBUG_PRINT(float* _mat);
		
	private:
		
		/// allow transform 3d to share values
		friend class Transform3D;

		/// used for returning model matrix values
		static float values[16];
	};
}