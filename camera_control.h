#pragma once

#include "camera3d.h"

namespace Game
{
	/// controls 
	class Camera_Control
	{
	public:

		/// init everything
		Camera_Control(
			CNGE7::Camera3D* _camera3d,
			float _min_height,
			float _max_height,
			float _away
		) : camera3d(_camera3d),
			min_height(_min_height),
			max_height(_max_height),
			away(_away),
			height((min_height + max_height) / 2),
			angle(0) {}

		/// makes the camera point dutifully at
		/// the player, which will be at position _focus
		void update(const glm::vec3&);

		/// move the rotation of this camera control
		/// this is cumulative, not a setter
		void rotate(float _delta);

		/// moves the camera relatively
		/// up or down
		/// limited by its own bounds
		void move_vertical(float _delta);

		float get_angle();

	private:

		CNGE7::Camera3D* camera3d;

		float min_height;
		float max_height;

		float away;

		float height;
		float angle;

	};
}
