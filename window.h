
#pragma once

#include <gl/glew.h>
#include <GLFW/glfw3.h>

#include <functional>

namespace CNGE7
{

	using Resize_Callback = std::function<void(int, int)>;

	class Window
	{
	public:

		/// initializes GLFW
		/// you must call this before any other functions
		static void init();

		/// throws if glew does not initialize
		static void wrangle();

		/// returns an array of pointers to each monitor
		/// the monitor count is passed in and will be sent out
		static GLFWmonitor** get_monitors(int& monitor_count);
		/// returns the primary monitor for simple monitor getting
		static GLFWmonitor* get_first_monitor();

		/// creates the window
		/// call more methods after this to modify window
		Window(
			int major_version,
			int minor_version,
			bool resizable,
			bool decorated,
			const char* title,
			GLFWmonitor* monitor,
			bool full,
			bool v_sync,
			Resize_Callback resize_callback
		);

		bool get_key_pressed(int _key_code);

		/// switches which monitor this window uses for fullscreen
		void set_monitor(GLFWmonitor* monitor);

		/// makes the window either fullscreen or not
		/// with the stored internal monitor
		void set_fullscreen(bool full);

		/// collects glfw events
		/// put this at the start of the game loop
		void poll();

		/// put this at the end of the game loop
		void swap();

		/// as soon as events are polled this window is going bye bye
		void close();

		/*
		 * getters
		 */

		/// put this in a loop to know when the window is closed
		bool get_should_close();

		int get_refresh_rate();

		double get_cursor_x();

		bool get_left_click();

		bool get_right_click();

	private:

		//input
		bool keys_pressed[GLFW_KEY_LAST + 1];
		double cursor_x;
		double cursor_y;

		bool left_click;
		bool right_click;

		GLFWwindow* window;
		GLFWmonitor* monitor;
		const GLFWvidmode* video_mode;

		Resize_Callback resize_callback;

		int x, y, width, height;
		bool full;
	};

}
