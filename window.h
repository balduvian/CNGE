#pragma once

#define GLEW_STATIC

#include <GL\glew.h>
#include <GL\wglew.h>
#include <gl/gl.h>
#include "GLFW/glfw3.h"
#include <string>
#include <functional>
#include <iostream>

namespace CNGE7
{
	class Window
	{
	public:

		using Resize_Callback = std::function<void(int, int)>;

		/// initializes GLFW
		/// you must call this before any other functions
		static void init()
		{
			if (glfwInit() == GL_FALSE)
			{
				std::cout << "glfw failed to initialize" << std::endl;
				exit(-1);
			}
		}

		/// throws if glew does not initialize
		static void wrangle()
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
		static GLFWmonitor** get_monitors(int& monitor_count)
		{
			return glfwGetMonitors(&monitor_count);
		}

		/// returns the primary monitor for simple monitor getting
		static GLFWmonitor* get_first_monitor()
		{
			return glfwGetPrimaryMonitor();
		}

		/// creates the window
		/// call more methods after this to modify window
		Window(int major_version, int minor_version, bool resizable, bool decorated, const char* title, GLFWmonitor* monitor, bool full, bool v_sync, Resize_Callback resize_callback)
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
		}

		/// switches which monitor this window uses for fullscreen
		void set_monitor(GLFWmonitor* monitor)
		{
			this->monitor = monitor;
		}

		/// makes the window either fullscreen or not
		/// with the stored internal monitor
		void set_fullscreen(bool full)
		{
			this->full = full;

			glfwSetWindowMonitor(window, full ? monitor : nullptr, x, y, width, height, GLFW_DONT_CARE);
		}

		/// collects glfw events
		/// put this at the start of the game loop
		void poll()
		{
			glfwPollEvents();
		}

		/// put this at the end of the game loop
		void swap()
		{
			glfwSwapBuffers(window);
		}

		/// as soon as events are polled this window is going bye bye
		void close()
		{
			glfwSetWindowShouldClose(window, true);
		}

		/*
		 * getters
		 */

		 /// put thisd in a loop to know when the window is closed
		bool get_should_close()
		{
			return glfwWindowShouldClose(window);
		}

	private:

		GLFWwindow* window;
		GLFWmonitor* monitor;
		const GLFWvidmode* video_mode;

		Resize_Callback resize_callback;

		int x, y, width, height;
		bool full;
	};
}
