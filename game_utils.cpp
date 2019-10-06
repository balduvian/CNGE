
#include "util.h"

#include "game_utils.h"

namespace Game
{
	void render_billboard(
		Game::Camera_Control* camera_control,
		CNGE7::Transform3D* transform3d,
		CNGE7::Camera3D* camera3d,
		Player_Rect* player_rect,
		CNGE7::Texture* texture,
		Fog_Shader* fog_shader,
		float r, float g, float b, float a,
		float* tex_modif,
		glm::vec3 &player_pos
	)
	{
		// angle you towards the camera to make a billboard
		transform3d->rotation.y = -camera_control->get_angle() - CNGE7::PI * 0.5f;

		texture->bind();

		fog_shader->enable(transform3d->to_model(), camera3d->get_projview());
		fog_shader->give_params(r, g, b, a, FOG_RED, FOG_GREEN, FOG_BLUE, FOG_ALPHA, player_pos, FOG_NEAR, FOG_FAR, tex_modif);

		player_rect->render();
	}

	float FOG_NEAR = 3;
	float FOG_FAR = 7;

	float FOG_RED = 0.1f;
	float FOG_GREEN = 0.12f;
	float FOG_BLUE = 0.45f;
	float FOG_ALPHA = 1.f;
}
