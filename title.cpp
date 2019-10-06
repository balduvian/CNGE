
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "title.h"

namespace Game
{
	
	float Title::swoop_time = 1;

	glm::vec3 Title::initial_destination = { 8, 2.5f, 0 };
	glm::vec3 Title::final_destination = { 8, 9, 0 };

	Title::Title() :
		transform(),
		swooping(false), 
		swoop_timer(swoop_time),
		along(0)
	{
		// put in center of screen
		transform.translation = initial_destination;
		transform.scale = {10.8333f, 5, 0};
	}

	bool Title::update(CNGE7::Window& window, double time)
	{
		// initiate start game sequence
		if (window.get_key_pressed(GLFW_KEY_SPACE))
		{
			swooping = true;
		}

		if (swooping)
		{
			// move that timer
			swoop_timer -= time;
			along = 1 - (swoop_timer / swoop_time);

			// set position of title in movement
			transform.translation = glm::mix(initial_destination, final_destination, along);

			if (swoop_timer <= 0)
			{
				return true;
			}
		}

		// we're not finished yet
		return false;
	}

	void Title::render(CNGE7::Camera& camera, CNGE7::Texture& title_texture, Tile_Shader& tile_shader, Player_Rect& player_rect)
	{
		title_texture.bind();
		tile_shader.enable(transform.to_model(), camera.get_projection());
		tile_shader.give_params(1, 1, 1, 1, title_texture.get_sheet(0));

		player_rect.render();
	}

}
