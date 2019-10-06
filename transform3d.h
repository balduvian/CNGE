
#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace CNGE7
{
	/// note that this class shares a bunch of static stuff
	/// with the transform class (is a friend)
	class Transform3D
	{
	public:

		/*
		 * initializers for the default model and projection value arrays
		 */

		glm::vec3 translation;
		glm::vec3 scale;
		glm::vec3 rotation;

		/// default initialization
		Transform3D();

		/// returns the float values for a model matrix based on this transform
		float* to_model();

		/// returns the float values for a model matrix based on given params
		/// no instance of a transform needed
		/// no rotation
		static float* to_model(float _x, float _y, float _z, float _width, float _height, float _depth);

		/// modifies a mat4 based on this transform
		void transform(glm::mat4& _mat);

		/// modify this matrix for the camera, everything is negative
		glm::mat4 camera_transform();

	};

}
