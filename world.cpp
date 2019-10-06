//


#include "rect.h"
#include "util.h"
#include "transform3d.h"

#include "texture.h"

#include "fog_shader.h"
#include "color_shader.h"

#include "camera_control.h"
#include "game_utils.h"

#include "camera3d.h"
#include "world.h"

namespace Game
{

	World::World(
		int _width,
		int _height,
		CNGE7::Camera3D* _camera3d,
		CNGE7::Texture* _grass_texture,
		CNGE7::Texture* _bush_texture,
		Color_Shader* _color_shader,
		Fog_Shader* _fog_shader,
		Rect* _rect,
		Player_Rect* _player_rect
	) :
		camera3d(_camera3d),
		width(_width),
		height(_height),
		grass_texture(_grass_texture),
		bush_texture(_bush_texture),
		color_shader(_color_shader),
		fog_shader(_fog_shader), 
		rect(_rect),
		player_rect(_player_rect),
		transform3d()
	{
		// fill world array
		world = new int* [width];
		for (auto w_ptr = world; w_ptr < world + width; ++w_ptr)
		{
			*w_ptr = new int[height];
			for (auto c_ptr = *w_ptr; c_ptr < *w_ptr + height; ++c_ptr)
			{
				// make sure merory is set to 0
				*c_ptr = 0;
			}
		}

	}

	World::~World()
	{
		// delete entire world array
		for (auto w_ptr = world; w_ptr < world + width; ++w_ptr)
			delete[] * w_ptr;

		delete[] world;
	}

	int** World::get()
	{
		return world;
	}

	void World::render(int _x, int _y, glm::vec3& player_pos, Camera_Control* camera_control, int radius, float base_height)
	{
		auto left = limit(_x - radius, width);
		auto right = limit(_x + radius, width);

		auto down = limit(_y - radius, height);
		auto up = limit(_y + radius, height);

		for (auto i = left; i <= right; ++i)
		{
			for (auto j = down; j <= up; ++j)
			{
				grass_texture->bind();

				transform3d.scale = { 1, 1, 1 };
				transform3d.rotation = { CNGE7::PI / 2, 0, 0 };
				transform3d.translation = { i, base_height, j };

				fog_shader->enable(transform3d.to_model(), camera3d->get_projview());

				auto color = CNGE7::noise_map(i * 0.98f, j * 0.98f);
				fog_shader->give_params(
					color * 0.1f, color, color * 0.1f, 1,
					0.1f, 0.17f, 0.43f, 1,
					player_pos,
					3,
					4,
					grass_texture->get_sheet(0)
				);

				rect->render();

				// if we draw a bush
				if (world[i][j] != 0)
				{
					transform3d.scale = { 1, 1, 1 };
					transform3d.rotation = { 0, 0, 0 };
					transform3d.translation = { i + 0.5f, base_height, j + 0.5f };

					render_billboard(
						camera_control,
						&transform3d,
						camera3d,
						player_rect,
						bush_texture,
						fog_shader,
						1, 1, 1, 1,
						bush_texture->get_sheet(world[i][j]),
						player_pos
					);
				}
			}
		}
	}

	int World::limit(int in, int bound)
	{
		if (in < 0)
			in = 0;
		else if (in >= width)
			in = width - 1;

		return in;
	}

}