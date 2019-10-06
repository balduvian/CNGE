
#include "util.h"

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "game_utils.h"

#include "player.h"

namespace Game
{
	float Player::default_width = 0.5f;
	float Player::default_height = 1.f;
	float Player::default_friction = 24.f;
	float Player::default_max_vel = 3.5f;
	float Player::default_speed = 24.f;

	double Player::gather_time = 1.f;
	double Player::walk_speed = 0.24f;

	int Player::walk_frames = 8;

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
		speed(default_speed),

		walk_timer(0),
		walk_frame(0),
		active(false),
		velocity_angle(0),
		gather_timer(0),
		gathering(false),
		gather_is_taking(false),
		inventory(0)
	{
		// make the player the correct dimensions
		transform3d.translation = { _x, 0, _y };
		transform3d.scale = { width, height, 1 };
	}

	void Player::update(
		CNGE7::Window &window,
		float camera_angle,
		float time,
		float left_bound,
		float right_bound,
		float down_bound,
		float up_bound,
		World& world
	)
	{
		// if we are wanting to move this frame
		active = false;

		auto direction = glm::vec2{};

		// no walking when gathering
		if (gathering)
		{
			gather_timer -= time;
			if(gather_timer <= 0)
			{
				// now collect the berry after gathering
				auto tile_x = world.loc_to_tile(transform3d.translation.x);
				auto tile_y = world.loc_to_tile(transform3d.translation.z);

				auto tile = world.get_tile(tile_x, tile_y);

				int amount_lookup[9] = {0, 0, 0, 0, 1, 2, 3, -3, 0};

				if (gather_is_taking)
				{
					if (tile != 0)
					{
						inventory += amount_lookup[tile];

						if (inventory < 0)
						{
							inventory = 0;
						}

						world.set_tile(tile_x, tile_y, 0);
					}
				}
				else
				{
					if (tile == 0)
					{
						if (inventory > 0)
						{
							world.set_tile(tile_x, tile_y, 1);

							--inventory;
						}
					}
				}

				// stop gathering
				gathering = false;
			}
		}
		else
		{	
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

			// check for gathering
			// start gather timer
			if (window.get_left_click())
			{
				gathering = true;
				gather_is_taking = true;
				gather_timer = gather_time;
			}
			else if (window.get_right_click())
			{
				gathering = true;
				gather_is_taking = false;
				gather_timer = gather_time;
			}

			// if we have contradicting keypresses
			if (direction == glm::vec2(0, 0))
			{
				active = false;
			}
		}

		if (active)
		{
			// do animations
			walk_timer += time;
			if (walk_timer >= walk_speed)
			{
				++walk_frame;
				walk_frame %= walk_frames;

				// reset
				walk_timer = 0;
			}

			// do actual game stuff
			float facing_angle = (camera_angle + CNGE7::PI);

			// get angle of resulting movement
			float angle = glm::orientedAngle(glm::normalize(direction), glm::rotate(glm::vec2(1, 0), facing_angle));

			// add velocity for this frame

			auto velocity_add = glm::vec2(cos(angle) * speed * time, sin(angle) * speed * time);

			velocity += velocity_add;

			// velocity limiter
			if (glm::length(velocity) > max_vel)
			{
				velocity = glm::normalize(velocity) * max_vel;
			}

			if (glm::angle(velocity, velocity_add) > CNGE7::PI)
			{
				if (velocity != glm::vec2(0, 0))
				{
					auto sub = glm::normalize(velocity) * friction * time;

					velocity -= sub;
				}
			}

			// get angle of velocity
			velocity_angle = glm::orientedAngle(glm::normalize(velocity), glm::vec2(1, 0));
		}
		else
		{
			if (velocity != glm::vec2(0,0))
			{
				auto sub = glm::normalize(velocity) * friction * time;

				velocity -= sub;
			}
		}

		if (glm::length(velocity) < 0.1f)
		{
			velocity = { 0, 0 };
		}

		transform3d.translation += glm::vec3{ velocity.x * time, 0, velocity.y * time};

		// collide with world boundary

		if (transform3d.translation.x < left_bound)
			transform3d.translation.x = left_bound;
		
		if (transform3d.translation.x > right_bound)
			transform3d.translation.x = right_bound;
		
		if (transform3d.translation.z < down_bound)
			transform3d.translation.z = down_bound;
		
		if (transform3d.translation.z > up_bound)
			transform3d.translation.z = up_bound;
		
	}

	void Player::render(Camera_Control* camera_control, CNGE7::Texture* player_texture, Fog_Shader* fog_shader)
	{
		bool forward_facing = fabs(velocity_angle - (camera_control->get_angle() - CNGE7::PI * 0.5f)) < CNGE7::PI;

		int anim_tile{};

		if (gathering)
		{
			anim_tile = forward_facing ? 8 : 17;
		}
		else if (active)
		{
			anim_tile = forward_facing ? walk_frame : walk_frame + 9;
		}
		else
		{
			anim_tile = forward_facing ? 0 : 9;
		}

		render_billboard(
			camera_control,
			&transform3d,
			camera3d,
			player_rect,
			player_texture,
			fog_shader,
			1, 1, 1, 1,
			player_texture->get_sheet(anim_tile),
			transform3d.translation
		);
	}

	int Player::get_x()
	{
		return round(transform3d.translation.x);
	}

	int Player::get_z()
	{
		return round(transform3d.translation.z);
	}

	int Player::get_inventory()
	{
		return inventory;
	}

}
