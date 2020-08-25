//
// Created by Emmet on 8/22/2020.
//

#include <iostream>
#include "pieceList.h"

namespace Game {
	PieceList::PieceList(i32 numReferencePieces, PieceReference initList[]) :
		referencePieces(initList),
		numReferencePieces(numReferencePieces),
		firstQueue(),
		nextQueue(),
		engine(std::random_device()()),
		dist(0, numReferencePieces - 1)
	{}

	auto PieceList::generateQueue(std::vector<PieceReference*> &queue) -> void {
		queue.resize(numReferencePieces);

		auto alreadyGotten = new bool[numReferencePieces]();

		for (auto i = 0; i < numReferencePieces; ++i) {
			auto getIndex = dist(engine);

			while (alreadyGotten[getIndex]) getIndex = (getIndex + 1) % numReferencePieces;

			alreadyGotten[getIndex] = true;
			queue.at(i) = (referencePieces.get() + getIndex);
		}

		delete[] alreadyGotten;
	}

	/**
	 * @param index
	 * @return nullptr if you are getting a piece out of range
	 */
	auto PieceList::getPiece(i32 index) -> PieceReference* {
		if (index < firstQueue.size())
			return firstQueue.at(index);
		else if (index < firstQueue.size() + nextQueue.size())
			return nextQueue.at(index - firstQueue.size());
		else
			return nullptr;
	}

	auto PieceList::dequeue() -> PieceReference* {
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

	auto PieceList::size() -> i32 {
		return numReferencePieces;
	}

	auto PieceList::begin() -> PieceReference* {
		return referencePieces.get();
	}

	auto PieceList::end() -> PieceReference* {
		return referencePieces.get() + numReferencePieces;
	}
}
