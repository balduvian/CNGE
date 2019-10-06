#pragma once

#include "shader.h"

namespace Game
{
	class Tile_Shader : public CNGE7::Shader
	{
	public:

		Tile_Shader();

		void give_params(float, float, float, float, float*);

	private:

		int color_loc;
		int tex_modif_loc;

	};
}
