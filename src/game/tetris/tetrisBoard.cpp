//
// Created by Emmet on 8/21/2020.
//

#include "tetrisBoard.h"

namespace Game {
	TetrisBoard::TetrisBoard(i32 width, i32 height)
		: width(width), height(height), board(new i32[width * height]()) {}

	TetrisBoard::~TetrisBoard() {
		delete[] board;
	}

	auto TetrisBoard::set(i32 x, i32 y, i32 piece) -> void {
		board[y * width + x] = piece;
	}

	auto TetrisBoard::get(i32 x, i32 y) -> i32 {
		return board[y * width + x];
	}

	auto TetrisBoard::getCollision(i32 x, i32 y) -> bool {
		if (x < 0) return true;
		if (x >= width) return true;
		if (y >= height) return false;
		if (y < 0) return true;

		return get(x, y);
	}

	auto TetrisBoard::getWidth() -> i32 {
		return width;
	}

	auto TetrisBoard::getHeight() -> i32 {
		return height;
	}
}
