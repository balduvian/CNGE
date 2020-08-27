//
// Created by Emmet on 8/25/2020.
//

#ifndef CNGE_8_2_FONT_H
#define CNGE_8_2_FONT_H

#include <msdfgen/msdfgen.h>
#include <msdfgen/msdfgen-ext.h>

#include "../../../load/resource.h"

namespace CNGE {
	class Font : public Resource {
	public:
		std::vector<f32> widths;

		f32 offsetX, offsetY, lineHeight;

	protected:
		virtual auto  customGather() -> LoadError override;
		virtual auto customProcess() -> LoadError override;
		virtual auto customDiscard() -> LoadError override;
		virtual auto  customUnload() -> LoadError override;

	private:

	};
}

#endif //CNGE_8_2_FONT_H
