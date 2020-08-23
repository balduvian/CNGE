//
// Created by Emmet on 8/22/2020.
//

#include "cnge8/math/math.h"

#include "destroyParticle.h"
#include "vector.h"

#include "../gameResources.h"
#include <random>


namespace Game {
	auto DestroyParticle::psuedoRandom(u32 a, u32 b, u32 c, u32 d, u32 e) -> f32 {
		return CNGE::mod<f32>(sin(a * 12.9898 + b * 78.233 + c * 83.827 + d * 29.6104 + e * 13.672) * 43758.5453, 1);
	}

	DestroyParticle::DestroyParticle() {}

	DestroyParticle::DestroyParticle(i32 x, i32 y, i32 color)
		: x(x), y(y), color(color) {}

	auto DestroyParticle::render(CNGE::Camera &camera, f32 boardX, f32 boardY, f32 tileSize, f32 along, CNGE::Color *colorList) -> void {
		for (auto i = 0; i < 4; ++i) {
			for (auto j = 0; j < 4; ++j) {
				auto velocity = (Vector((i - 2_f32) + 0.5f, (j - 2_f32) + 0.5f) / 1.5 * tileSize).rotate((psuedoRandom(x, y, i, j, color) * CNGE::PI<f32> / 6) - CNGE::PI<f32> / 12);

				auto position = Vector(boardX + (x * tileSize) + (i * tileSize / 4), boardY + (y * tileSize) + (j * tileSize / 4)) + (velocity * along * along);

				auto rotation = CNGE::interpSquared(0_f32, (psuedoRandom(x + 1, y - 1, i + 1, j - 1, color + 1) * CNGE::PI<f32> * 2) - CNGE::PI<f32>, along);
				auto size = CNGE::interpSquared(tileSize / 4, 0_f32, along);

				GameResources::colorShader.enable(CNGE::Transform::toModelCenterRotate(position.getX(), position.getY(), size, size, rotation), camera.getProjview());
				GameResources::colorShader.giveColor(colorList[color - 1]);
				GameResources::rect.render();
			}
		}
	}
}