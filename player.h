
#pragma once

#include <glm/vec2.hpp>

#include "camera_control.h"
#include "camera3d.h"
#include "rect.h"
#include "color_shader.h"
#include "window.h"
#include "texture.h"
#include "fog_shader.h"
#include "world.h"

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

		void update(CNGE7::Window& window, float, float, float, float, float, float, World&);

		void render(Camera_Control*, CNGE7::Texture*, Fog_Shader*);

		/// gets the world tile x value
		int get_x();

		/// world tile z value
		int get_z();

		int get_inventory();

	private:

		static double walk_speed;
		static double gather_time;

		double gather_timer;
		bool gathering;
		bool gather_is_taking;

		static int walk_frames;
		double walk_timer;
		int walk_frame;

		bool active;

		CNGE7::Camera3D* camera3d;
		Player_Rect* player_rect;
		Color_Shader* color_shader;
		
		float width;
		float height;

		float friction;
		float max_vel;

		float speed;

		glm::vec2 velocity;
		float velocity_angle;

		int inventory;
	};
}
