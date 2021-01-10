
#ifndef CNGE_TEXTURE
#define CNGE_TEXTURE

#include "cnge/types.h"
#include "cnge/image/image.h"
#include "textureParams.h"

namespace CNGE {
	class Texture {
	public:
		Texture(u32, u32, u8*, TextureParams = TextureParams());
		~Texture();

		const static float DEFAULT_TILE_VALUES[4];

		auto bind(i32 = 0) -> void;
		auto get() -> u32;

	protected:
		static float tileValues[4];

		u32 width, height;

	private:
		u32 texture;
		i32 horzWrap, vertWrap, minFilter, magFilter;
	};
}

#endif
