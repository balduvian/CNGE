
#pragma once

#include <glm/vec3.hpp>

#include "transform3d.h"
#include "camera3d.h"
#include "texture.h"
#include "color_shader.h"
#include "fog_shader.h"
#include "rect.h"
#include "camera_control.h"

namespace Game
{

	class World
	{
	public:

		World(
			int _width,
			int _height,
			CNGE7::Camera3D* _camera3d,
			CNGE7::Texture* grass_texture,
			CNGE7::Texture* bush_texture,
			Color_Shader* _color_shader,
			Fog_Shader* _fog_shader,
			Rect* _rect,
			Player_Rect* _player_rect
		);

		~World();

		int** get();

		void update(double);

		void render(
			int _x,
			int _y,
			glm::vec3 &player_pos,
			Camera_Control*,
			int radius,
			float base_height);
		
		int get_width();

		int get_height();

		int loc_to_tile(float loc);

		int get_tile(int, int);

		void set_tile(int, int, int);

	private:

		/// makes sure this position doesn't go over the edge
		int limit(int in, int bound);

		CNGE7::Transform3D transform3d;
		CNGE7::Camera3D* camera3d;
		CNGE7::Texture* grass_texture;
		CNGE7::Texture* bush_texture;
		Color_Shader* color_shader;
		Fog_Shader* fog_shader;
		Rect* rect;
		Player_Rect* player_rect;

		static double tick_time;
		static int tick_speed;

		double tick_timer;

		int width;
		int height;

		int** world;

	};

}
