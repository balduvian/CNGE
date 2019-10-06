

#include "util.h"

#include "glm/glm.hpp"
#include "glm/vec3.hpp"
#include "glm/gtx/vector_angle.hpp"
#include "camera3d.h"
#include "rect.h"
#include "color_shader.h"
#include "transform3d.h"
#include "window.h"

#include "player.h"

namespace Game
{
	float Player::default_width = 0.5f;
	float Player::default_height = 1.f;
	float Player::default_friction = 1.f;
	float Player::default_max_vel = 3.f;
	float Player::default_speed = 6.f;

	Player::Player(
		float _x,
		float _y,
		CNGE7::Camera3D* _camera3d,
		Player_Rect* _player_rect,
		Color_Shader* _color_shader
	) :
		camera3d(_camera3d),
		player_rect(_player_rect),
		color_shader(_color_shader),
		transform3d(),

		width(default_width),
		height(default_height),

		velocity(0, 0),
		friction(default_friction),
		max_vel(default_max_vel),
		speed(default_speed)
	{
		// make the player the correct dimensions
		transform3d.translation = { _x, 0, _y };
		transform3d.scale = { width, height, 1 };
	}

	void Player::update(CNGE7::Window &window, float camera_angle, float time)
	{
		// angle you towards the camera to make a billboard
		transform3d.rotation.y = -camera_angle - CNGE7::PI * 0.5f;

		// if we are wanting to move this frame
		bool active = false;

		auto direction = glm::vec2{};

		// get inputs and keep track of angles of keys
		if (window.get_key_pressed(GLFW_KEY_A))
		{
			direction += glm::vec2{ 0, 1 };
			active = true;
		}
		if (window.get_key_pressed(GLFW_KEY_D))
		{
			direction += glm::vec2{ 0, -1 };
			active = true;
		}
		if (window.get_key_pressed(GLFW_KEY_W))
		{
			direction += glm::vec2{ 1, 0 };
			active = true;
		}
		if (window.get_key_pressed(GLFW_KEY_S))
		{
			direction += glm::vec2{ -1, 0 };
			active = true;
		}

		// if we have contradicting keypresses
		if (direction == glm::vec2(0, 0))
		{
			active = false;
		}

		if (active)
		{
			float facing_angle = (camera_angle + CNGE7::PI);

			// get angle of resulting movement
			float angle = glm::orientedAngle(glm::normalize(direction), glm::rotate(glm::vec2(1, 0), facing_angle));

			// add velocity for this frame
			velocity += glm::vec2(cos(angle) * speed * time, sin(angle) * speed * time);

			// velocity limiter
			if (glm::length(velocity) > max_vel)
			{
				velocity = glm::normalize(velocity) * max_vel;
			}

			auto norm_velocity = velocity;
			glm::normalize(norm_velocity);

			// friction counteracts velocity
			float friction_angle = glm::orientedAngle(norm_velocity, glm::vec2(1, 0)) + CNGE7::PI;

			// friction can't be greater than your movement
			float friction_length = min(glm::length(velocity), friction);

			auto friction_force = glm::vec2(cos(friction_angle), sin(friction_angle)) * friction_length;

			velocity += friction_force * time;

			//no skek tiny velocities
			//if (glm::length(velocity) < 0.1f)
			//{
			//	velocity = glm::vec2(0, 0);
			//}
		}

		transform3d.translation += glm::vec3{ velocity.x * time, 0, velocity.y * time};
	}

	void Player::render()
	{
		color_shader->enable(transform3d.to_model(), camera3d->get_projview());
		color_shader->give_color(0.4f, 0.1f, 0.97f, 1);

		player_rect->render();
	}

	int Player::get_x()
	{
		return round(transform3d.translation.x);
	}

	int Player::get_z()
	{
		return round(transform3d.translation.z);
	}

}
