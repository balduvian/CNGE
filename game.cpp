
#include "window.h"
#include "loop.h"
#include <iostream>

int main()
{
	CNGE7::Window::init();

	auto monitor = CNGE7::Window::get_first_monitor();

	auto window = CNGE7::Window(3, 3, true, true, "Skeekers", monitor, false, true, [](auto w, auto h) {});

	CNGE7::Window::wrangle();

	// begin the game
	CNGE7::Loop(window.get_refresh_rate(), std::bind(&CNGE7::Window::get_should_close, window), [&](auto fps, auto delta, auto time)
	{ 
		window.poll();

		std::cout << "hek" << time << std::endl;

		window.swap();
	});

	window.close();

	return 0;
}