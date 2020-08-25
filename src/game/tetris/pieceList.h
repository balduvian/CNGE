//
// Created by Emmet on 8/22/2020.
//

#ifndef CNGE_8_2_PIECELIST_H
#define CNGE_8_2_PIECELIST_H

#include <memory>
#include <random>

#include "pieceReference.h"

namespace Game {
	class PieceList {
	private:
		std::unique_ptr<PieceReference[]> referencePieces;
		i32 numReferencePieces;

		std::vector<PieceReference*> firstQueue;
		std::vector<PieceReference*> nextQueue;

		auto generateQueue(std::vector<PieceReference*>&) -> void;

		std::default_random_engine engine;
		std::uniform_int_distribution<i32> dist;

	public:
		PieceList(i32, PieceReference[]);

		auto getPiece(i32) -> PieceReference*;

		auto dequeue() -> PieceReference*;

		auto reset() -> void;

		auto size() -> i32;

		auto begin() -> PieceReference*;
		auto end() -> PieceReference*;
	};
}

#endif //CNGE_8_2_PIECELIST_H
