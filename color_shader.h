
#include "shader.h"

namespace Game
{
	class Color_Shader : public CNGE7::Shader
	{
	public:
		Color_Shader() : Shader("shaders/color/vert.glsl", "shaders/color/frag.glsl"),
			color_loc(Shader::get_uniform("in_color")) {}

		void give_color(float _r, float _g, float _b, float _a)
		{
			Shader::give_vector4(color_loc, _r, _g, _b, _a);
		}

	private:

		int color_loc;

	};
}
