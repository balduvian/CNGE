//
// Created by Emmet on 1/10/2021.
//

#include <fstream>

#include "fontData.h"

namespace CNGE {
	FontData::FontData(): characterWidths() {}

	auto toInt(const char *fourBytes) -> i32 {
		return (u32)fourBytes[0] | ((u32)fourBytes[1] << 8_u32) | ((u32)fourBytes[2] << 16_u32) | ((u32)fourBytes[3] << 24_u32);
	}

	/**
	 * @return nullptr if the input file was invalid
	 */
	auto FontData::fromFile(const char *filepath) -> std::unique_ptr<FontData> {
		auto fileStream = std::fstream(filepath, std::fstream::in | std::fstream::binary);

		auto ret = std::make_unique<FontData>();

		char fourBytes[4];

		fileStream.read(fourBytes, 4);
		if (fileStream.fail()) return nullptr;
		ret->imageWidth = toInt(fourBytes);

		fileStream.read(fourBytes, 4);
		if (fileStream.fail()) return nullptr;
		ret->imageHeight = toInt(fourBytes);

		fileStream.read(fourBytes, 4);
		if (fileStream.fail()) return nullptr;
		ret->tileWidth = toInt(fourBytes);

		fileStream.read(fourBytes, 4);
		if (fileStream.fail()) return nullptr;
		ret->tileHeight = toInt(fourBytes);

		fileStream.read(&ret->startCharacter, 1);
		if (fileStream.fail()) return nullptr;
		fileStream.read(ret->characterWidths, 256);
		if (fileStream.fail()) return nullptr;

		return ret;
	}
}
