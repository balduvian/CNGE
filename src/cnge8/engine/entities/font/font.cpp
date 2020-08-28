//
// Created by Emmet on 8/25/2020.
//

#include <GL/glew.h>
#include <iostream>
#include <cnge8/math/math.h>
#include "font.h"

#include "msdfgen/msdfgen.h"
#include "msdfgen/msdfgen-ext.h"

namespace CNGE {
	Font::Font(const char* filepath) :
		Resource(true), filepath(filepath), image(Image::makeEmpty()),
		widths(), offsetX(), offsetY(), lineHeight(), texture(nullptr) {}

	Font::~Font() {
		destroy();
	}

	auto Font::customGather() -> LoadError {
		auto *ft = msdfgen::initializeFreetype();
		if (!ft) return LoadError("Freetype could not be initialized");

		auto *font = loadFont(ft, filepath);
		if (!font) return LoadError(std::string("Font ") + filepath + " could not be loaded");

		msdfgen::FontMetrics fontMetrics;
		msdfgen::getFontMetrics(fontMetrics, font);

		auto scale = 1.25;
		auto range = 4.0;
		offsetX = range / 2;
		offsetY = -(fontMetrics.descenderY / 2) + (range / 4);
		lineHeight = (fontMetrics.lineHeight * 0.75 + range / 2) * scale;
		auto boxSize = u32(ceil(lineHeight));

		widths.resize(CHAR_END - CHAR_START);
		image = Image::makeSheet(boxSize * CHARS_WIDE + (CHARS_WIDE - 1) * GAP, boxSize * CHARS_TALL + (CHARS_TALL - 1) * GAP);
		auto pixels = image.getPixels();

		// fill in black into the image
		for (auto i = 0; i < image.getWidth() * image.getHeight(); ++i)
			pixels[i * 4 + 3] = 0xff;

		msdfgen::Shape shape;
		double advance;
		msdfgen::Bitmap<float, 3> tempBitmap(boxSize, boxSize);

		for (auto j = 0; j < CHARS_TALL; ++j) {
			for (auto i = 0; i < CHARS_WIDE; ++i) {
				auto index = j * CHARS_WIDE + i;

				if (loadGlyph(shape, font, CHAR_START + index, &advance)) {
					widths[index] = advance;

					if (shape.contours.size() > 0) {
						shape.normalize();
						edgeColoringSimple(shape, 3.0);
						generateMSDF(tempBitmap, shape, range, scale, msdfgen::Vector2(offsetX, offsetY));

						// put generated msdf into the main image
						for (auto y = 0; y < boxSize; ++y) {
							for (auto x = 0; x < boxSize; ++x) {
								auto imageY = j * (boxSize + GAP) + y;
								auto imageX = i * (boxSize + GAP) + x;

								pixels[(imageY * image.getWidth() + imageX) * 4] = u8(
									msdfgen::pixelFloatToByte(tempBitmap(x, boxSize - y - 1)[0]));
								pixels[(imageY * image.getWidth() + imageX) * 4 + 1] = u8(
									msdfgen::pixelFloatToByte(tempBitmap(x, boxSize - y - 1)[1]));
								pixels[(imageY * image.getWidth() + imageX) * 4 + 2] = u8(
									msdfgen::pixelFloatToByte(tempBitmap(x, boxSize - y - 1)[2]));
							}
						}
					}
				}
			}
		}

		msdfgen::destroyFont(font);
		msdfgen::deinitializeFreetype(ft);

		auto path = std::filesystem::path("output.png");
		image.write(path);

		return LoadError::none();
	}

	auto Font::customProcess() -> LoadError {
		texture = std::make_unique<TileGrid>(image.getWidth(), image.getHeight(), image.getPixels(), TextureParams().setWrap(GL_CLAMP_TO_BORDER).setFilter(GL_LINEAR), CHARS_WIDE, CHARS_TALL, GAP);

		return LoadError::none();
	}

	auto Font::customDiscard() -> LoadError {
		image.invalidate();

		return LoadError::none();
	}

	auto Font::customUnload() -> LoadError {
		texture = nullptr;

		return LoadError::none();
	}

	auto Font::getLineHeight() -> f32{
		return lineHeight;
	}

	auto Font::getTexture() -> TileGrid* {
		return texture.get();
	}

	auto Font::charX(char current) -> u32 {
		return (current - CHAR_START) % CHARS_WIDE;
	}

	auto Font::charY(char current) -> u32 {
		return (current - CHAR_START) / CHARS_WIDE;
	}

	auto Font::width(char current) -> f32 {
		return widths[current - CHAR_START];
	}

	auto Font::render(char *string, u32 length, f32 along, f32 x, f32 y, f32 size, f32 spacing, std::function<void(TileGrid*, u32, f32, u32, u32, f32, f32, f32, f32)> &&onRender) -> void {
		auto renderLength = length * along;
		auto maxLength = u32(ceil(renderLength));
		if (maxLength > length) maxLength = length;

		auto scale = size / lineHeight;
		auto space = size * spacing;

		for (auto i = 0; i < maxLength; ++i) {
			auto current = string[i];

			auto along = renderLength - i;
			if (along > 1) along = 1;

			onRender(texture.get(), i, along, charX(current), charY(current), x - offsetX * scale, y - offsetY * scale, size, size);

			x += width(current) * scale + space;
		}
	}
}