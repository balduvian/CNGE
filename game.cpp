
#include "window.h"
#include "loop.h"
#include <iostream>
#include "camera.h"
#include "vao.h"
#include "rect.h"
#include "shader.h"
#include "color_shader.h"
#include "transform.h"
#include "util.h"

int main()
{
	CNGE7::Window::init();

	auto monitor = CNGE7::Window::get_first_monitor();

	auto window = CNGE7::Window(3, 3, true, true, "Skeekers", monitor, false, true, [](auto w, auto h) {});

	CNGE7::Window::wrangle();

	//load stuffs
	auto camera = CNGE7::Camera();
	camera.set_ortho(16, 9);

	auto rect = CNGE7::Rect();

	auto color_shader = Game::Color_Shader();

	auto transform = CNGE7::Transform();
	transform.translation = { 0.1f, 0.1f, 0 };
	transform.scale = { 1, 1, 1 };

	// begin the game
	CNGE7::Loop(window.get_refresh_rate(), std::bind(&CNGE7::Window::get_should_close, window), [&](auto fps, auto delta, auto time)
	{ 
		window.poll();

		camera.update();

		CNGE7::clear(.65f, 1, .1f, 1);

		// do the render
		color_shader.enable(transform.to_model(), camera.get_projview());
		color_shader.give_color(1, 0.5f, 0.f, 1);
		rect.render();

		//std::cout << "hek" << time << std::endl;

		window.swap();
	});

	window.close();

	return 0;
}