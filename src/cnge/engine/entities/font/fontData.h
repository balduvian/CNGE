//
// Created by Emmet on 1/10/2021.
//

#ifndef CNGE_8_2_FONTDATA_H
#define CNGE_8_2_FONTDATA_H

#include <vector>
#include <memory>

#include "cnge/types.h"

namespace CNGE {
	class FontData {
	public:
		FontData();

		i32 imageWidth;
		i32 imageHeight;
		i32 tileWidth;
		i32 tileHeight;
		char startCharacter;
		char characterWidths[256];

		static auto fromFile(const char *) -> std::unique_ptr<FontData>;
	};
}

#endif //CNGE_8_2_FONTDATA_H
