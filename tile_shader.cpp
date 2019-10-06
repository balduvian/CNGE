
#include "tile_shader.h"

namespace Game
{
	Tile_Shader::Tile_Shader() : Shader("shaders/tile/vert.glsl", "shaders/tile/frag.glsl"),
		color_loc(Shader::get_uniform("in_color")),
		tex_modif_loc(Shader::get_uniform("tex_modif"))
	{}

	void Tile_Shader::give_params(float _r, float _g, float _b, float _a, float* tex_modif)
	{
		Shader::give_vector4(color_loc, _r, _g, _b, _a);
		Shader::give_vector4(tex_modif_loc, tex_modif[0], tex_modif[1], tex_modif[2], tex_modif[3]);
	}

}
