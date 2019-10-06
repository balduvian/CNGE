
#pragma once

#include <glm/vec3.hpp>

#include "rect.h"
#include "tile_shader.h"
#include "camera.h"
#include "texture.h"
#include "transform.h"
#include "window.h"

namespace Game
{
	class Title
	{
	public:

		Title();

		// will tell the game whether it is done with the sequence
		bool update(CNGE7::Window &window, double time);

		void render(CNGE7::Camera &camera, CNGE7::Texture &title_texture, Tile_Shader &tile_shader, Player_Rect &rect);

	private:

		static float swoop_time;

		static glm::vec3 initial_destination;
		static glm::vec3 final_destination;

		bool swooping;
		float swoop_timer;
		float along;

		CNGE7::Transform transform;

	};

}
