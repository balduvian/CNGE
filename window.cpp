
#define GLEW_STATIC

#include <GL\glew.h>
#include <gl/gl.h>

#include <string>
#include <iostream>

#include "window.h"

namespace CNGE7
{

	/// initializes GLFW
	/// you must call this before any other functions
	void Window::init()
	{
		if (glfwInit() == GL_FALSE)
		{
			std::cout << "glfw failed to initialize" << std::endl;
			exit(-1);
		}

		glfwSetErrorCallback([](auto error, auto description)
			{
				std::cout << "GLFW ERROR CODE " << error << " | " << description << std::endl;
				exit(-1);
			});
	}

	/// throws if glew does not initialize
	void Window::wrangle()
	{
		glewExperimental = true;

		if (glewInit() != GLEW_OK)
		{
			std::cout << "glew failed to initialize" << std::endl;
			exit(-1);
		}
	}

	/// returns an array of pointers to each monitor
	/// the monitor count is passed in and will be sent out
	GLFWmonitor** Window::get_monitors(int& monitor_count)
	{
		return glfwGetMonitors(&monitor_count);
	}

	/// returns the primary monitor for simple monitor getting
	GLFWmonitor* Window::get_first_monitor()
	{
		return glfwGetPrimaryMonitor();
	}

	/// creates the window
	/// call more methods after this to modify window
	Window::Window(
		int major_version,
		int minor_version,
		bool resizable,
		bool decorated,
		const char* title,
		GLFWmonitor* monitor,
		bool full,
		bool v_sync,
		Resize_Callback resize_callback
	) :
		keys_pressed()
	{
		// set hints
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major_version);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor_version);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, resizable);
		glfwWindowHint(GLFW_DECORATED, decorated);

		// set members
		this->resize_callback = resize_callback;
		this->full = full;
		this->monitor = monitor;

		// get the video mode from the monitor you pass in
		video_mode = glfwGetVideoMode(monitor);

		// get dimensions from the vidmode used for calculations
		auto v_width = video_mode->width;
		auto v_height = video_mode->height;

		// set the positions of the window to center inside the monitor
		width = v_width / 2;
		height = v_height / 2;
		x = v_width / 4;
		y = v_height / 4;

		// create the window, 
		window = glfwCreateWindow(width, height, title, full ? monitor : nullptr, nullptr);

		// set the context
		glfwMakeContextCurrent(window);
		glfwSwapInterval(v_sync);

		// store this into glfw, "thanks c libraries" -slab 2019
		glfwSetWindowUserPointer(window, this);

		glfwSetWindowSizeCallback(window, [](auto window, auto width, auto height)
			{
				// grab this window class
				auto this_ptr = (Window*)glfwGetWindowUserPointer(window);

				// first set internal dimension values
				this_ptr->width = width;
				this_ptr->height = height;

				// then do user defined things
				this_ptr->resize_callback(width, height);
			});

		// set keys pressed in an array
		glfwSetKeyCallback(window, [](auto window, auto key, auto scancode, auto action, auto mods)
			{
				auto this_ptr = (Window*)glfwGetWindowUserPointer(window);

				// make sure we don't have garbage keys
				if (key >= 0 && key < GLFW_KEY_LAST)
				{
					if (action == GLFW_PRESS)
					{
						this_ptr->keys_pressed[key] = true;
					}
					else if (action == GLFW_RELEASE)
					{
						this_ptr->keys_pressed[key] = false;
					}
				}
			});

		glfwShowWindow(window);
		glfwFocusWindow(window);
	}

	bool Window::get_key_pressed(int _key_code)
	{
		return keys_pressed[_key_code];
	}

	/// switches which monitor this window uses for fullscreen
	void Window::set_monitor(GLFWmonitor* monitor)
	{
		this->monitor = monitor;
	}

	/// makes the window either fullscreen or not
	/// with the stored internal monitor
	void Window::set_fullscreen(bool full)
	{
		this->full = full;

		glfwSetWindowMonitor(window, full ? monitor : nullptr, x, y, width, height, GLFW_DONT_CARE);
	}

	/// collects glfw events
	/// put this at the start of the game loop
	void Window::poll()
	{
		glfwPollEvents();
	}

	/// put this at the end of the game loop
	void Window::swap()
	{
		glfwSwapBuffers(window);
	}

	/// as soon as events are polled this window is going bye bye
	void Window::close()
	{
		glfwSetWindowShouldClose(window, true);
	}

	/*
		* getters
		*/

		/// put thisd in a loop to know when the window is closed
	bool Window::get_should_close()
	{
		auto ret = glfwWindowShouldClose(window);
		if (ret == GLFW_TRUE)
			glfwTerminate();
		return ret;
	}

	int Window::get_refresh_rate()
	{
		return video_mode->refreshRate;
	}

}
