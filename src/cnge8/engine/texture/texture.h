
#ifndef CNGE_TEXTURE
#define CNGE_TEXTURE

#include "../../types.h"
#include "../../image/image.h"
#include "../../load/resource.h"
#include "textureParams.h"

namespace CNGE {
	class Texture : public Resource {
	public:
		Texture(const char*, TextureParams = TextureParams());

		const static float DEFAULT_TILE_VALUES[4];

		auto bind(i32 = 0) -> void;
		auto get() -> u32;

		~Texture();

	protected:
		static float tileValues[4];

		virtual auto  customGather() -> bool override;
		virtual auto customProcess() -> bool override;
		virtual auto customDiscard() -> bool override;
		virtual auto  customUnload() -> bool override;

		u32 width;
		u32 height;

	private:
		const char* assetPath;
		Image assetImage;

		u32 texture;

		i32 horzWrap, vertWrap, minFilter, magFilter;
	};
}

#endif
