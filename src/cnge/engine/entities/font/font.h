//
// Created by Emmet on 1/10/2021.
//

#ifndef CNGE_8_2_FONT_H
#define CNGE_8_2_FONT_H

#include <functional>

#include "cnge/load/resource.h"
#include "../texture/tileGrid.h"
#include "fontData.h"

namespace CNGE {
	class Font : public CNLL::Resource {
	private:
		std::string imageFilepath;
		std::string dataFilepath;

		TextureParams textureParams;
		std::unique_ptr<Image> image;

		std::unique_ptr<TileGrid> tileGrid;
		std::unique_ptr<FontData> fontData;

	public:
		Font(const char *, const char *, TextureParams);
		~Font();

		auto customGather() -> bool override;
		auto customDiscard() -> void override;
		auto customLoad() -> void override;
		auto customUnload() -> void override;

		auto getTexture() -> TileGrid *;

		auto render(f32, f32, f32, f32, const char *, std::function<void(f32, f32, f32, f32, f32*)>&&) -> void;
	};
}

#endif //CNGE_8_2_FONT_H
