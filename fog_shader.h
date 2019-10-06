
#include "shader.h"

namespace Game
{
	class Fog_Shader : public CNGE7::Shader
	{
	public:

		Fog_Shader();

		void give_params(float _cr, float _cg, float _cb, float _ca, float _fr, float _fg, float _fb, float _fa, glm::vec3 &player_pos, float fog_near, float fog_far, float*);

	private:

		int color_loc;
		int fog_color_loc;
		int player_loc;
		int fog_loc;
		int tex_modif_loc;

	};
}
