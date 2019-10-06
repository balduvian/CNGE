
#pragma once

#include "shader.h"

namespace Game
{
	class Color_Shader : public CNGE7::Shader
{
	public:

		Color_Shader();

		void give_color(float _r, float _g, float _b, float _a);

	private:

		int color_loc;

	};
}
