//
// Created by Emmet on 8/24/2020.
//

#include "pieceReference.h"

namespace Game {
	auto PieceReference::createPiece(i32 x, i32 y) -> std::unique_ptr<Piece> {
		return std::make_unique<Piece>(x, y, boundingSize, Piece::copyLayout(boundingSize, layout));
	}

	PieceReference::PieceReference() {}

	PieceReference::PieceReference(i32 boundingSize, i32 *layout)
		: boundingSize(boundingSize), layout(layout), boundingX(boundingSize), boundingY(boundingSize), boundingWidth(-1), boundingHeight(-1)
	{
		/* find minimum bounding box */
		for (auto j = 0; j < boundingSize; ++j) {
			for (auto i = 0; i < boundingSize; ++i) {
				if (layout[j * boundingSize + i]) {
					if (j < boundingY) boundingY = j;
					if (i < boundingX) boundingX = i;
					/* height and width are treated as indices of greatest farthest point */
					if (j > boundingHeight) boundingHeight = j;
					if (i > boundingWidth) boundingWidth = i;
				}
			}
		}

		/* convert farthest point to a dimension using nearest point */
		boundingHeight = boundingHeight - boundingY + 1;
		boundingWidth = boundingWidth - boundingX + 1;
	}

	PieceReference::~PieceReference() {
		delete[] layout;
	}

	PieceReference::PieceReference(PieceReference &&old) :
		boundingSize(old.boundingSize),
		layout(old.layout),
		boundingX(old.boundingX),
		boundingY(old.boundingY),
		boundingWidth(old.boundingWidth),
		boundingHeight(old.boundingHeight)
	{
		old.layout = nullptr;
	}

	auto PieceReference::operator=(PieceReference &&old) -> PieceReference& {
		this->boundingSize = old.boundingSize;
		this->layout = old.layout;
		this->boundingX = old.boundingX;
		this->boundingY = old.boundingY;
		this->boundingWidth = old.boundingWidth;
		this->boundingHeight = old.boundingHeight;

		old.layout = nullptr;
	}

	auto PieceReference::getBoundingSize() -> i32 {
		return boundingSize;
	}

	auto PieceReference::getLayout() -> i32* {
		return layout;
	}

	auto PieceReference::getBoundingX() -> i32 {
		return boundingX;
	}

	auto PieceReference::getBoundingY() -> i32 {
		return boundingY;
	}

	auto PieceReference::getBoundingWidth() -> i32 {
		return boundingWidth;
	}

	auto PieceReference::getBoundingHeight() -> i32 {
		return boundingHeight;
	}
}