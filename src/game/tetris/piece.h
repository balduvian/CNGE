//
// Created by Emmet on 8/21/2020.
//

#ifndef CNGE_8_2_PIECE_H
#define CNGE_8_2_PIECE_H

#include "cnge8/types.h"

#include "rotation.h"

namespace Game {
	class Piece {
	private:
		i32 x, y, boundingSize, *layout;

	public:
		static auto copyLayout(i32, i32*) -> i32*;

		Piece(i32, i32, i32, i32*);

		Piece(const Piece&);
		Piece(Piece&&);

		auto operator=(const Piece&) -> Piece&;
		auto operator=(Piece&&) -> Piece&;

		~Piece();

		auto setPosition(i32, i32) -> void;

		auto getTile(i32, i32) -> i32;

		auto getX() -> i32;
		auto getY() -> i32;
		auto setX(i32) -> void;
		auto setY(i32) -> void;
		auto setXY(i32, i32) -> void;
		auto moveX(i32) -> void;
		auto moveY(i32) -> void;

		auto rotate(Rotation::RotateFunc) -> i32*;

		auto setRotated(i32*) -> void;

		auto getBoundingSize() -> i32;
		auto getLayout() -> i32*;
	};
}

#endif //CNGE_8_2_PIECE_H
