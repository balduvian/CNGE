
#include "util.h"
#include "glm/gtx/rotate_vector.hpp"

#include "camera_control.h"

namespace Game
{
	void Camera_Control::update(const glm::vec3& _focus)
	{
		// camera is pointing 180 from how it goes away from the player
		// also look down at the player
		camera3d->transform3d.rotation = { glm::atan(away / height), angle - CNGE7::PI * 0.5f, 0 };

		// add a vector that's as long as the distance away the camera should be horizontally
		// rotate it properly then add it to player's position to get camera x and z location
		// also just set camera height as height
		auto camera_position = glm::rotate(glm::vec2(away, 0), angle) + glm::vec2(_focus.x, _focus.z);
		camera3d->transform3d.translation = { camera_position.x, height, camera_position.y };
	}

	void Camera_Control::rotate(float _delta)
	{
		angle += _delta;
		angle = CNGE7::mod(angle, CNGE7::PI * 2);
	}

	void Camera_Control::move_vertical(float _delta)
	{
		height += _delta;

		if (height < min_height)
			height = min_height;

		if (height > max_height)
			height = max_height;
	}

	float Camera_Control::get_angle()
	{
		return angle;
	}

}
