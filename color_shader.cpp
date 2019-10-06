
#include "color_shader.h"

namespace Game
{
	Color_Shader::Color_Shader() : Shader("shaders/color/vert.glsl", "shaders/color/frag.glsl"),
		color_loc(Shader::get_uniform("in_color")) {}

	void Color_Shader::give_color(float _r, float _g, float _b, float _a)
	{
		Shader::give_vector4(color_loc, _r, _g, _b, _a);
	}

}
