//
// Created by Emmet on 8/21/2020.
//

#include "rotation.h"
#include "cnge8/math/math.h"

namespace Game {
	/* 2d coordinates flattened into a 1d index */
	/* created by: { x + y * width } */

	auto Rotation::rotatePositive(i32 x, i32 y, i32 size) -> i32 {
		return (-y + size - 1) + (x) * size;
	}

	auto Rotation::rotateNegative(i32 x, i32 y, i32 size) ->  i32 {
		return (y) + (-x + size - 1) * size;
	}

	auto Rotation::rotateTwice(i32 x, i32 y, i32 size) ->  i32 {
		return (-x + size - 1) + (-y + size - 1) * size;
	}
}
