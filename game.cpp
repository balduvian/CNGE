

#include "window.h"

#include "util.h"

#include "loop.h"

#include "camera.h"
#include "camera3d.h"

#include "rect.h"
#include "cube.h"
#include "texture.h"
#include "tile_shader.h"
#include "color_shader.h"
#include "fog_shader.h"

#include "camera_control.h"

#include "game_utils.h"

#include "player.h"
#include "world.h"

int main()
{
	CNGE7::Window::init();

	auto monitor = CNGE7::Window::get_first_monitor();

	auto window = CNGE7::Window(3, 3, true, true, "Skeekers", monitor, false, true, [](auto w, auto h) {});

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
	auto cube = Game::Cube();

	auto grass_texture = CNGE7::Texture("images/grass.png");
	auto bush_texture = CNGE7::Texture("images/bush.png", 2);

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

	auto player = Game::Player(4.5f, 4.5f, &camera3d, &player_rect, &color_shader);

	//load world
	auto world = Game::World(65, 65, &camera3d, &grass_texture, &bush_texture, &color_shader, &fog_shader, &rect, &player_rect);

	transform3d.scale = { 1, 2, 1 };
	transform3d.rotation = { 0, 0.324f, 1.324f };
	transform3d.translation = { 0, -1, -3 };

	// begin the game
	CNGE7::Loop(window.get_refresh_rate(), std::bind(&CNGE7::Window::get_should_close, window), [&](auto fps, auto delta, auto time)
		{
			window.poll();

			player.update(window, camera_control.get_angle(), time);

			camera.update();

			//camera_control.rotate(time * CNGE7::PI * 0.5f);
			camera_control.update(player.transform3d.translation);
			camera3d.update();

			CNGE7::clear(Game::FOG_RED, Game::FOG_GREEN, Game::FOG_BLUE, Game::FOG_ALPHA);

			glEnable(GL_TEXTURE_2D);

			//debug render
			grass_texture.bind();
			tile_shader.enable(transform.to_model(), camera.get_projview());
			tile_shader.give_params(1, 0.5f, 0.f, 1, grass_texture.get_sheet(0));
			//color_shader.enable(transform.to_model(), camera.get_projview());
			//color_shader.give_color(1, 0.5f, 0.f, 1);
			rect.render();

			// render world
			world.render(player.get_x(), player.get_z(), player.transform3d.translation, &camera_control, 5, 0);

			player.render();

			window.swap();
		});

	window.close();

	return 0;
}