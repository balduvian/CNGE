
#pragma once

#include <glm/vec3.hpp>

#include "camera_control.h"
#include "transform3d.h"
#include "camera3d.h"
#include "rect.h"
#include "texture.h"
#include "fog_shader.h"

namespace Game
{
	void render_billboard(
		Camera_Control* camera_control,
		CNGE7::Transform3D* transform3d,
		CNGE7::Camera3D* camera3d,
		Player_Rect* player_rect,
		CNGE7::Texture* texture,
		Fog_Shader* fog_shader,
		float r, float g, float b, float a,
		float* tex_modif,
		glm::vec3 &player_pos
	);

	extern float FOG_NEAR;
	extern float FOG_FAR;

	extern float FOG_RED;
	extern float FOG_GREEN;
	extern float FOG_BLUE;
	extern float FOG_ALPHA;
}
