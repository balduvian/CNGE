//
// Created by Emmet on 8/21/2020.
//

#ifndef CNGE_8_2_TETRISBOARD_H
#define CNGE_8_2_TETRISBOARD_H

#include "cnge/types.h"

namespace Game {
	class TetrisBoard {
	private:
		i32 width, height, *board;

	public:
		TetrisBoard(i32, i32);
		~TetrisBoard();

		auto set(i32, i32, i32) -> void;
		auto get(i32, i32) -> i32;
		auto getCollision(i32, i32) -> bool;

		auto getWidth() -> i32;
		auto getHeight() -> i32;
	};
}

#endif //CNGE_8_2_TETRISBOARD_H
