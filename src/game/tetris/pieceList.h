//
// Created by Emmet on 8/22/2020.
//

#ifndef CNGE_8_2_PIECELIST_H
#define CNGE_8_2_PIECELIST_H

#include <memory>
#include <random>

#include "piece.h"

namespace Game {
	class PieceList {
	private:
		std::vector<Piece> referencePieces;

		std::vector<Piece*> firstQueue;
		std::vector<Piece*> nextQueue;

		auto generateQueue(std::vector<Piece*>&) -> void;

		std::default_random_engine engine;
		std::uniform_int_distribution<i32> dist;

	public:
		PieceList(std::initializer_list<Piece>&&);

		auto getPiece(i32) -> Piece*;

		auto dequeue() -> Piece*;

		auto reset() -> void;
	};
}

#endif //CNGE_8_2_PIECELIST_H
