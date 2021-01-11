//
// Created by Emmet on 1/10/2021.
//

#include "font.h"

namespace CNGE {
	Font::Font(const char *imageFilepath, const char *dataFilepath, TextureParams textureParams): CNLL::Resource(true), imageFilepath(imageFilepath), dataFilepath(dataFilepath), textureParams(textureParams), image(), tileGrid(), fontData() {}

	Font::~Font() {
		onDestroy();
	}

	auto Font::customGather() -> bool {
		fontData = FontData::fromFile(dataFilepath.c_str());
		image = Image::fromPNG(imageFilepath.c_str());

		return image != nullptr && fontData != nullptr;
	}

	auto Font::customDiscard() -> void {
		image = nullptr;
	}

	auto Font::customLoad() -> void {
		tileGrid = std::make_unique<TileGrid>(image->getWidth(), image->getHeight(), image->getPixels(), textureParams, fontData->imageWidth / fontData->tileWidth, fontData->imageHeight / fontData->tileHeight, 0);
	}

	auto Font::customUnload() -> void {
		tileGrid = nullptr;
	}

	auto Font::getTexture() -> TileGrid * {
		return tileGrid.get();
	}

	auto Font::render(f32 x, f32 y, f32 height, f32 spacing, const char *text, std::function<void(f32, f32, f32, f32, f32*)>&& onChar) -> void {
		auto width = (fontData->tileWidth / (f32)fontData->tileHeight) * height;

		for (auto i = 0, c = (i32)(u8)*text; c != 0; ++i, c = text[i]) {
			auto xx = (c - fontData->startCharacter) % tileGrid->getTilesWide();
			auto yy = (c - fontData->startCharacter) / tileGrid->getTilesWide();
			onChar(x, y, width, height, tileGrid->getSheet((c - fontData->startCharacter) % tileGrid->getTilesWide(), (c - fontData->startCharacter) / tileGrid->getTilesWide()));

			x += (fontData->characterWidths[c] / (f32)fontData->tileWidth) * width + spacing;
		}
	}
}
