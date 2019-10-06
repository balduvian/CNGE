
#pragma once

#include <glm/vec2.hpp>

#include "camera3d.h"
#include "rect.h"
#include "color_shader.h"
#include "window.h"

namespace Game
{
	class Player
	{
	public:

		static float default_width;
		static float default_height;
		static float default_friction;
		static float default_max_vel;
		static float default_speed;

		CNGE7::Transform3D transform3d;

		Player(float, float, CNGE7::Camera3D*, Player_Rect*, Color_Shader*);

		void update(CNGE7::Window& window, float, float);

		void render();

		/// gets the world tile x value
		int get_x();

		/// world tile z value
		int get_z();

	private:

		CNGE7::Camera3D* camera3d;
		Player_Rect* player_rect;
		Color_Shader* color_shader;
		
		float width;
		float height;

		float friction;
		float max_vel;

		float speed;

		glm::vec2 velocity;
	};
}
