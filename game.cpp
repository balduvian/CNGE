
#include "window.h"

#include "util.h"

#include "loop.h"

#include "camera.h"
#include "camera3d.h"

#include "rect.h"
#include "texture.h"
#include "tile_shader.h"
#include "color_shader.h"
#include "fog_shader.h"

#include "camera_control.h"

#include "game_utils.h"

#include "title.h"
#include "player.h"
#include "world.h"

#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

int main()
{
	CNGE7::Window::init();

	auto monitor = CNGE7::Window::get_first_monitor();

	auto window = CNGE7::Window(3, 3, true, true, "Skek Grove", monitor, false, true, [](auto w, auto h) {});

	CNGE7::Window::wrangle();

	//load stuffs
	auto camera = CNGE7::Camera();
	camera.set_ortho(16, 9);

	auto camera3d = CNGE7::Camera3D();
	camera3d.set_perspective(CNGE7::PI / 2.f, 16.f / 9.f);
	// move our camera
	camera3d.transform3d.translation = { 4, 0, 4 };
	camera3d.transform3d.rotation = { CNGE7::PI * 0.2f, 0, 0 };

	auto player_rect = Game::Player_Rect();
	auto rect = Game::Rect();

	auto grass_texture = CNGE7::Texture("images/grass.png", 2);
	auto bush_texture = CNGE7::Texture("images/bush.png", 8);
	auto player_texture = CNGE7::Texture("images/player.png", 18);
	auto title_texture = CNGE7::Texture("images/title.png", 1);
	auto berry_texture = CNGE7::Texture("images/berry.png", 1);

	auto tile_shader = Game::Tile_Shader();
	auto color_shader = Game::Color_Shader();
	auto fog_shader = Game::Fog_Shader();

	auto transform3d = CNGE7::Transform3D();

	auto transform = CNGE7::Transform();
	transform.translation = { 0.f, 0.f, 0 };
	transform.scale = { 1, 1, 1 };

	//enables
	CNGE7::default_enables();
	//CNGE7::winding_order(true);
	//CNGE7::set_cull(false, true);

	auto camera_control = Game::Camera_Control(&camera3d, 2, 4, 3);

	//auto player = Game::Player(4.5f, 4.5f, &camera3d, &player_rect, &color_shader);

	std::unique_ptr<Game::Player> player = {};

	// load in title
	auto title = Game::Title();
	//load world
	auto world = Game::World(65, 65, &camera3d, &grass_texture, &bush_texture, &color_shader, &fog_shader, &rect, &player_rect);

	float spawn_x = world.get_width() / 2.f;
	float spawn_z = world.get_height() / 2.f;
	auto spawn_vec = glm::vec3(spawn_x, 0, spawn_z);

	transform3d.scale = { 1, 2, 1 };
	transform3d.rotation = { 0, 0.324f, 1.324f };
	transform3d.translation = { 0, -1, -3 };

	// begin the game
	CNGE7::Loop(window.get_refresh_rate() + 5, std::bind(&CNGE7::Window::get_should_close, window), [&](auto fps, auto delta, auto time)
		{
			window.poll();

			/*
			 * UPDATE
			 */

			// on the title screen
			if (player.get() == nullptr)
			{
				if (title.update(window, time))
					// create the p[layer
					player = std::unique_ptr<Game::Player>(new Game::Player(spawn_x, spawn_z, &camera3d, &player_rect, &color_shader));

				camera_control.rotate(window.get_cursor_x() * 0.01f);
				camera_control.update(spawn_vec);
			}
			// when we're playing the game
			else
			{
				
				world.update(time);

				player->update(window, camera_control.get_angle(), time, 0, world.get_width(), 0, world.get_height(), world);

				camera_control.rotate(window.get_cursor_x() * 0.01f);
				camera_control.update(player->transform3d.translation);
			}

			camera.update();
			camera3d.update();

			/*
			 * RENDER
			 */

			CNGE7::clear(Game::FOG_RED, Game::FOG_GREEN, Game::FOG_BLUE, Game::FOG_ALPHA);

			if (player.get() == nullptr)
			{
				world.render(world.get_width() / 2.f, world.get_height() / 2.f, spawn_vec, &camera_control, Game::FOG_FAR + 1, 0);

				title.render(camera, title_texture, tile_shader, player_rect);
			}
			else
			{
				world.render(player->get_x(), player->get_z(), player->transform3d.translation, &camera_control, Game::FOG_FAR + 1, 0);
				player->render(&camera_control, &player_texture, &fog_shader);
				
				// render inventory of berries
				auto num = player->get_inventory();

				berry_texture.bind();

				transform.scale = { 1, 1, 1 };
				transform.translation = { 0, 1, 0 };
				transform.rotation = 0;

				auto total_width = 16;
				auto using_width = 8;

				auto start_x = (total_width - using_width) / 2.f;

				auto width_fraction = (1.f / num) * using_width;

				glDisable(GL_DEPTH_TEST);

				for (auto i = 0; i < num; ++i)
				{
					transform.translation.x = start_x + (((i * width_fraction) + ((i + 1) * width_fraction)) / 2.f);

					tile_shader.enable(transform.to_model(), camera.get_projection());
					tile_shader.give_params(1, 1, 1, 1, berry_texture.get_sheet(0));

					player_rect.render();
				}

				glEnable(GL_DEPTH_TEST);
			}

			window.swap();
		});

	window.close();

	return 0;

}
