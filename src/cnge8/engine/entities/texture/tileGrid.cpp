
#include "tileGrid.h"

namespace CNGE {
	TileGrid::TileGrid(u32 width, u32 height, u8* pixels, TextureParams params, u32 tilesWide, u32 tilesTall)
	: Texture(width, height, pixels, params), tilesWide(tilesWide), tilesTall(1), widthFraction(), heightFraction() {}

	TileGrid::TileGrid(u32 width, u32 height, u8* pixels, TextureParams params, u32 tilesWide)
	: Texture(width, height, pixels, params), tilesWide(tilesWide), tilesTall(1), widthFraction(), heightFraction() {}

	auto TileGrid::init() -> void {
		widthFraction = 1._f32 / tilesWide;
		heightFraction = 1._f32 / tilesTall;
	}

	f32* TileGrid::getSheet(u32 x, u32 y) {
		tileValues[0] = widthFraction;
		tileValues[1] = heightFraction;
		tileValues[2] = x * widthFraction;
		tileValues[3] = y * heightFraction;

		return tileValues;
	}

	f32* TileGrid::getSheet(u32 x) {
		tileValues[0] = widthFraction;
		tileValues[1] = 1;
		tileValues[2] = x * widthFraction;
		tileValues[3] = 0;

		return tileValues;
	}

}
