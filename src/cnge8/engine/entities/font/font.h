//
// Created by Emmet on 8/25/2020.
//

#ifndef CNGE_8_2_FONT_H
#define CNGE_8_2_FONT_H

#include <functional>
#include <msdfgen/msdfgen.h>
#include <msdfgen/msdfgen-ext.h>
#include <cnge8/engine/camera.h>
#include <cnge8/engine/entities/vao/vao.h>

#include "../texture/tileGrid.h"
#include "../../../load/resource.h"

namespace CNGE {
	class Font : public Resource {
	public:
		constexpr static u32 CHAR_START = 32;
		constexpr static u32 CHAR_END = 128;

		constexpr static u32 CHARS_WIDE = 16;
		constexpr static u32 CHARS_TALL = 6;

		constexpr static u32 GAP = 1;

		Font(const char*);
		~Font();

		auto getLineHeight() -> f32;
		auto getTexture() -> TileGrid*;

		auto charX(char) -> u32;
		auto charY(char) -> u32;

		auto width(char) -> f32;

		auto render(char*, u32, f32, f32, f32, f32, f32, std::function<void(TileGrid*, u32, f32, u32, u32, f32, f32, f32, f32)>&&) -> void;

	protected:
		auto  customGather() -> LoadError override;
		auto customProcess() -> LoadError override;
		auto customDiscard() -> LoadError override;
		auto  customUnload() -> LoadError override;

	private:
		const char* filepath;
		Image image;

		std::vector<f32> widths;

		f32 offsetX, offsetY, lineHeight;

		std::unique_ptr<TileGrid> texture;
	};
}

#endif //CNGE_8_2_FONT_H
