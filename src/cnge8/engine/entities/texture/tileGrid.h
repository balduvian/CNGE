
#ifndef CNGE_TILE_GRID
#define CNGE_TILE_GRID

#include "texture.h"

#include "cnge8/types.h"

namespace CNGE {
	class TileGrid : public Texture {
	private:
		u32 tilesWide;
		u32 tilesTall;

		u32 gap;

	public:
		TileGrid(u32, u32, u8*, TextureParams, u32, u32, u32);
		TileGrid(u32, u32, u8*, TextureParams, u32, u32);

		f32* getSheet(u32, u32);
		f32* getSheet(u32);
	};
}

#endif
