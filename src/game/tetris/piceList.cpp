//
// Created by Emmet on 8/22/2020.
//

#include "pieceList.h"

namespace Game {
	PieceList::PieceList(std::initializer_list<Piece> &&referencePieces)
		: referencePieces(referencePieces), firstQueue(), nextQueue(), engine(std::random_device()()), dist(0, referencePieces.size() - 1) {}

	auto PieceList::generateQueue(std::vector<Piece*> &queue) -> void {
		queue.resize(referencePieces.size());

		auto alreadyGotten = new bool[referencePieces.size()]();

		for (auto i = 0; i < referencePieces.size(); ++i) {
			auto getIndex = dist(engine);

			while (alreadyGotten[getIndex]) getIndex = (getIndex + 1) % referencePieces.size();

			alreadyGotten[getIndex] = true;
			queue.at(i) = &referencePieces.at(getIndex);
		}

		delete[] alreadyGotten;
	}

	/**
	 * @param index
	 * @return nullptr if you are getting a piece out of range
	 */
	auto PieceList::getPiece(i32 index) -> Piece* {
		if (index < firstQueue.size())
			return firstQueue.at(index);
		else if (index < firstQueue.size() + nextQueue.size())
			return nextQueue.at(index - firstQueue.size());
		else
			return nullptr;
	}

	auto PieceList::dequeue() -> Piece* {
		/* remove first element of queue */
		auto ret = firstQueue.at(0);
		firstQueue.erase(firstQueue.begin());

		if (firstQueue.empty()) {
			firstQueue = nextQueue;
			generateQueue(nextQueue);
		}

		return ret;
	}

	auto PieceList::reset() -> void {
		generateQueue(firstQueue);
		generateQueue(nextQueue);
	}
}
