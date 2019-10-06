
#include "fog_shader.h"

namespace Game
{
	Fog_Shader::Fog_Shader() : Shader("shaders/fog/vert.glsl", "shaders/fog/frag.glsl"),
		color_loc(Shader::get_uniform("in_color")),
		fog_color_loc(Shader::get_uniform("fog_color")),
		player_loc(Shader::get_uniform("player_pos")),
		fog_loc(Shader::get_uniform("fog")),
		tex_modif_loc(Shader::get_uniform("tex_modif"))
	{}

	void Fog_Shader::give_params(float _cr, float _cg, float _cb, float _ca, float _fr, float _fg, float _fb, float _fa, glm::vec3& player_pos, float fog_near, float fog_far, float* tex_modif)
	{
		Shader::give_vector4(color_loc, _cr, _cg, _cb, _ca);
		Shader::give_vector4(fog_color_loc, _fr, _fg, _fb, _fa);
		Shader::give_vector3(player_loc, player_pos.x, player_pos.y, player_pos.z);
		Shader::give_vector2(fog_loc, fog_near, fog_far);
		Shader::give_vector4(tex_modif_loc, tex_modif[0], tex_modif[1], tex_modif[2], tex_modif[3]);
	}

}
