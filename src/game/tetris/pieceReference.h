//
// Created by Emmet on 8/24/2020.
//

#ifndef CNGE_8_2_PIECEREFERENCE_H
#define CNGE_8_2_PIECEREFERENCE_H

#include <memory>

#include "cnge8/types.h"
#include "piece.h"

namespace Game {
	class PieceReference {
	private:
		i32 boundingSize, *layout;
		i32 boundingX, boundingY, boundingWidth, boundingHeight;

	public:
		auto createPiece(i32, i32) -> std::unique_ptr<Piece>;

		PieceReference();
		PieceReference(i32, i32*);
		~PieceReference();

		PieceReference(const PieceReference&) = delete;
		auto operator=(const PieceReference&) -> PieceReference& = delete;

		PieceReference(PieceReference&&);
		auto operator=(PieceReference&&) -> PieceReference&;

		auto getBoundingSize() -> i32;
		auto getLayout() -> i32*;

		auto getBoundingX() -> i32;
		auto getBoundingY() -> i32;
		auto getBoundingWidth() -> i32;
		auto getBoundingHeight() -> i32;
	};
}

#endif //CNGE_8_2_PIECEREFERENCE_H
