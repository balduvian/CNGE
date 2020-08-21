
#ifndef CNGE_TEXTURE
#define CNGE_TEXTURE

#include "cnge8/types.h"
#include "cnge8/image/image.h"
#include "cnge8/load/resource.h"
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

		virtual auto  customGather() -> LoadError override;
		virtual auto customProcess() -> LoadError override;
		virtual auto customDiscard() -> LoadError override;
		virtual auto  customUnload() -> LoadError override;

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
