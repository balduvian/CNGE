//
// Created by Emmet on 8/21/2020.
//

#include "piece.h"
#include "rotation.h"

namespace Game {
	auto Piece::copyLayout(i32 size, i32 *layout) -> i32* {
		auto ret = new i32[size * size];

		for (auto i = 0; i < size * size; ++i)
			ret[i] = layout[i];

		return ret;
	}

	Piece::Piece(i32 x, i32 y, i32 boundingSize, i32 *layout)
		: x(x), y(y), boundingSize(boundingSize), layout(layout) {}

	Piece::Piece(const Piece &other)
		: x(0), y(0), boundingSize(other.boundingSize), layout(copyLayout(boundingSize, other.layout)) {}

	Piece::Piece(Piece &&other)
		: x(other.x), y(other.y), boundingSize(other.boundingSize), layout(other.layout) {
		other.layout = nullptr;
	}

	auto Piece::operator=(const Piece &other) -> Piece& {
		this->boundingSize = other.boundingSize;

		this->layout = copyLayout(boundingSize, other.layout);

		return *this;
	}

	auto Piece::operator=(Piece &&other) -> Piece& {
		this->x = other.x;
		this->y = other.y;
		this->boundingSize = other.boundingSize;

		this->layout = other.layout;
		other.layout = nullptr;

		return *this;
	}

	Piece::~Piece() {
		delete[] layout;
	}

	auto Piece::setPosition(i32 x, i32 y) -> void {
		this->x = x;
		this->y = y;
	}

	auto Piece::getTile(i32 x, i32 y) -> i32 {
		return layout[y * boundingSize + x];
	}

	auto Piece::getX() -> i32 {
		return x;
	}

	auto Piece::getY() -> i32 {
		return y;
	}

	auto Piece::setX(i32 x) -> void {
		this->x = x;
	}

	auto Piece::setY(i32 y) -> void {
		this->y = y;
	}

	auto Piece::setXY(i32 x, i32 y) -> void {
		this->x = x;
		this->y = y;
	}

	auto Piece::moveX(i32 offset) -> void {
		x += offset;
	}

	auto Piece::moveY(i32 offset) -> void {
		y += offset;
	}

	auto Piece::rotate(Rotation::RotateFunc rotateFunction) -> i32* {
		auto ret = new i32[boundingSize * boundingSize];

		for (auto y = 0; y < boundingSize; ++y) {
			for (auto x = 0; x < boundingSize; ++x) {
				ret[y * boundingSize + x] = layout[rotateFunction(x, y, boundingSize)];
			}
		}

		return ret;
	}

	auto Piece::setRotated(i32* newLayout) -> void {
		delete[] layout;

		layout = newLayout;
	}

	auto Piece::getBoundingSize() -> i32 {
		return boundingSize;
	}

	auto Piece::getLayout() -> i32* {
		return layout;
	}
}
