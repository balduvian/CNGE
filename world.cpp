
#include <time.h> 

#include "util.h"
#include "game_utils.h"

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
		transform3d(),
		tick_timer(tick_time)
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

		// generate bushes
		auto total = (int)roundf(width * height * 0.15f);

		srand(time(nullptr));

		for (auto i = 0; i < total; ++i)
		{
			auto x = rand() % width;
			auto y = rand() % height;

			auto inital_growth = rand() % 3;

			world[x][y] = inital_growth + 1;
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
		auto left = _x - radius;
		auto right = _x + radius;

		auto down = _y - radius;
		auto up = _y + radius;

		for (auto i = left; i <= right; ++i)
		{
			for (auto j = down; j <= up; ++j)
			{
				// for displaying rocks
				bool out_of_bounds = i < 0 || j < 0 || i >= width || j >= height;

				grass_texture->bind();

				transform3d.scale = { 1, 1, 1 };
				transform3d.rotation = { CNGE7::PI / 2, 0, 0 };
				transform3d.translation = { i, base_height, j };

				fog_shader->enable(transform3d.to_model(), camera3d->get_projview());

				auto color = CNGE7::noise_map(i * 0.98f, j * 0.98f);

				// range limit the color
				auto high = 1.f;
				auto low = 0.65f;
				color = (high - low) * color + low;

				fog_shader->give_params(
					1 * color, 1 * color, 1 * color , 1,
					FOG_RED, FOG_GREEN, FOG_BLUE, FOG_ALPHA,
					player_pos,
					FOG_NEAR,
					FOG_FAR,
					grass_texture->get_sheet(out_of_bounds ? 1 : 0)
				);

				rect->render();
			}
		}

		auto limit_left = limit(left, width);
		auto limit_right = limit(right, width);

		auto limit_down = limit(down, height);
		auto limit_up = limit(up, height);

		// second pass for bushes
		for (auto i = limit_left; i <= limit_right; ++i)
		{
			for (auto j = limit_down; j <= limit_up; ++j)
			{
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
						bush_texture->get_sheet(world[i][j] - 1),
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

	int World::get_width()
	{
		return width;
	}

	int World::get_height()
	{
		return height;
	}

	int World::loc_to_tile(float loc)
	{
		return (int)floorf(loc);
	}

	int World::get_tile(int x, int y)
	{
		if (x < 0 || y < 0 || !(x < width && y < height))
		{
			return -1;
		}
		
		return world[x][y];
	}

	void World::set_tile(int x, int y, int tile)
	{
		world[x][y] = tile;
	}

	void World::update(double time)
	{
		tick_timer -= time;
		if (tick_timer <= 0)
		{
			//reset
			tick_timer = tick_time - tick_timer;

			for (auto i = 0; i < tick_speed; ++i)
			{
				auto x = rand() % width;
				auto y = rand() % height;

				// try to grow a berry
				if (world[x][y] != 0)
				{
					if (world[x][y] == 8)
						world[x][y] = 0;
					else
						++world[x][y];
				}

			}
		}
	}

	double World::tick_time = 0.05;
	int World::tick_speed = 3;
}
