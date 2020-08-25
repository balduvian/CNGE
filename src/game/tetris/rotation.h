//
// Created by Emmet on 8/21/2020.
//

#ifndef CNGE_8_2_ROTATION_H
#define CNGE_8_2_ROTATION_H

#include <cnge8/types.h>

namespace Game {
	class Rotation {
	public:
		using RotateFunc = i32(*)(i32, i32, i32);

		static auto rotatePositive(i32, i32, i32) -> i32;
		static auto rotateNegative(i32, i32, i32) -> i32;
		static auto rotateTwice(i32, i32, i32) -> i32;
	};
}

#endif //CNGE_8_2_ROTATION_H
