
#ifndef GAME_TEXTURE_SHADER
#define GAME_TEXTURE_SHADER

#include "cnge/engine/entities/shader/shader.h"
#include "cnge/types.h"

namespace Game {
	class TextureShader : public CNGE::Shader {
	private:
		i32 colorLoc = 0;
		i32 texModifLoc = 0;
		
	public:
		TextureShader();

		auto getUniforms() -> void override;

		auto giveParams(f32, f32, f32, f32, const f32[]) -> void;

		auto giveParams(f32, f32, f32, f32) -> void;

		auto giveParams(const f32[]) -> void;

		auto giveParams() -> void;
	};
}

#endif
