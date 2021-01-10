//
// Created by Emmet on 8/22/2020.
//

#ifndef CNGE_8_2_DESTROYPARTICLE_H
#define CNGE_8_2_DESTROYPARTICLE_H

#include "cnge/engine/camera.h"
#include "cnge/util/color.h"
#include "cnge/types.h"

namespace Game {
	class DestroyParticle {
	private:
		i32 x, y, color;

		static auto psuedoRandom(u32, u32, u32, u32, u32) -> f32;

	public:
		DestroyParticle();
		DestroyParticle(i32, i32, i32);

		auto render(CNGE::Camera&, f32, f32, f32, f32, CNGE::Color*) -> void;
	};
}

#endif //CNGE_8_2_DESTROYPARTICLE_H
