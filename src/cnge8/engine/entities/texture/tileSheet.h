
#ifndef CNGE_TILE_SHEET
#define CNGE_TILE_SHEET

#include "texture.h"

#include "cnge8/types.h"

namespace CNGE {
	class TileSheet : public Texture {
	private:
		u32 numPositions;
		const i32* inputPositions;

		f32* calculatedPositions;

	public:
		TileSheet(const char*, const u32, const i32*, TextureParams = TextureParams());

		auto customProcess() -> LoadError override;
		auto customUnload() -> LoadError override;

		f32* getSheet(u32);

		~TileSheet();
	};
}

#endif