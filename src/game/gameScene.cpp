
#include "gameScene.h"

#include "cnge8/math/math.h"
#include "cnge8/engine/transform.h"
#include "gameResources.h"

#include <iostream>
#include <game/tetris/rotation.h>

namespace Game {
	const CNGE::Color GameScene::skekColor = CNGE::Color(0x31d433);

	GameScene::GameScene()
		: aspect(512, 288, 688, 288),
		Scene(&GameResources::gameResources),
		randomDevice(),
		tickTimer(0.5),
        board(std::make_unique<TetrisBoard>(10, 20)),
        pieceList(std::make_unique<PieceList>(4, new PieceReference[] {
			PieceReference(2, new i32[] {
				0, 0,
				1, 1,
			}),
			PieceReference(1, new i32[] {
				2,
			}),
			PieceReference(2, new i32[] {
				3, 3,
				3, 0
			}),
			PieceReference(3, new i32[] {
				0, 4, 0,
				0, 4, 0,
				0, 4, 0
			})
        })),
        currentPiece(nullptr),
        rowAnimation(nullptr),
        destroyParticles(),
        destroyedRows(),
        downLock(false)
	{
		pieceList->reset();

		for (auto &ref : *pieceList) {
			std::cout << "{" << ref.getBoundingX() << ref.getBoundingY() << ref.getBoundingWidth() << ref.getBoundingHeight() << "}";
		}
	}

	CNGE::Color GameScene::colors[7] {
		CNGE::Color(252, 186, 3),
		CNGE::Color(63, 212, 55),
		CNGE::Color(36, 242, 242),
		CNGE::Color(29, 66, 196),
		CNGE::Color(149, 24, 222),
		CNGE::Color(227, 130, 27),
		CNGE::Color(232, 30, 171)
	};

	CNGE::Color GameScene::boardColor = CNGE::Color(2, 25, 41);
	CNGE::Color GameScene::gridColor = CNGE::Color(95, 104, 110);
	CNGE::Color GameScene::outlineColor = CNGE::Color(197, 220, 235);

	auto GameScene::start() -> void {
		GameResources::testSound.loop(true);
		GameResources::testSound.play();

		tickTimer.start();
	}

	auto GameScene::resized(u32 width, u32 height) -> void {
		aspect.update(width, height);
		camera.setOrtho(aspect.getGameWidth(), aspect.getGameHeight());
		glViewport(aspect.getLeft(), aspect.getTop(), aspect.getWidth(), aspect.getHeight());
	}

	auto GameScene::update(CNGE::Input* input, CNGE::Timing* timing) -> void {
		if (rowAnimation) {
			if (rowAnimation->update(timing->time)) {
				/* only when animation is done actually modify board */
				removeRows(board.get(), destroyedRows);
				rowAnimation = nullptr;
			}
		} else {
			if (tickTimer.updateContinual(timing->time)) {
				if (currentPiece == nullptr) {
					currentPiece = std::move(pieceList->dequeue(board->getWidth(), board->getHeight()));
					calculateGhost(currentPiece.get(), board.get(), ghostX, ghostY);

				} else {
					if (movePiece(currentPiece.get(), board.get(), 0, -1))
						currentPiece->moveY(-1);
					else
						piecePlaceRoutine();
				}
			}
		}

		if (currentPiece != nullptr) {
			if (input->getKeyPressed(GLFW_KEY_LEFT)) {
				if (movePiece(currentPiece.get(), board.get(), -1, 0)) {
					currentPiece->moveX(-1);
					calculateGhost(currentPiece.get(), board.get(), ghostX, ghostY);
				}
			} else if (input->getKeyPressed(GLFW_KEY_RIGHT)) {
				if (movePiece(currentPiece.get(), board.get(), 1, 0)) {
					currentPiece->moveX(1);
					calculateGhost(currentPiece.get(), board.get(), ghostX, ghostY);
				}
			} else if (input->getKeyPressed(GLFW_KEY_UP)) {
				i32* rotatedLayout;
				i32 pushX;
				if (rotatePiece(currentPiece.get(), board.get(), Rotation::rotatePositive, rotatedLayout, pushX)) {
					currentPiece->setRotated(rotatedLayout);
					calculateGhost(currentPiece.get(), board.get(), ghostX, ghostY);
				}
			} else if (input->getKeyDown(GLFW_KEY_DOWN) && !downLock) {
				if (movePiece(currentPiece.get(), board.get(), 0, -1))
					currentPiece->moveY(-1);
				else
					piecePlaceRoutine();
			} else if (input->getKeyPressed(GLFW_KEY_SPACE)) {
				currentPiece->setXY(ghostX, ghostY);
				piecePlaceRoutine();
			}
		}

		if (!input->getKeyDown(GLFW_KEY_DOWN)) downLock = false;

		camera.update();
	}

	auto GameScene::render() -> void {
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		auto boardHeight = aspect.getGameHeight() - 32;
		auto boardWidth = boardHeight * board->getWidth() / board->getHeight();
		auto boardX = aspect.getGameWidth() / 2 - boardWidth / 2;
		auto boardY = 16;
		auto tileSize = boardHeight / board->getHeight();
		auto outlineSize = tileSize / 8;

		/* render board */
		renderBoardBack(board.get(), boardX, 16, tileSize, outlineSize);

		if (rowAnimation) {
			/* stationary blocks below the destroyed */
			renderBoard(board.get(), 0, destroyedRows[0], boardX, boardY, tileSize);

			/* falling blocks above the destroyed */
			for (auto rowIndex = 0; rowIndex < destroyedRows.size(); ++rowIndex) {
				auto start = destroyedRows[rowIndex] + 1;
				auto end = rowIndex == destroyedRows.size() - 1 ? board->getHeight() : destroyedRows[rowIndex + 1];

				auto offsetY = CNGE::interpSquared<f32>(0, -tileSize * (rowIndex + 1), rowAnimation->getAlong());
				renderBoard(board.get(), start, end, boardX, boardY + offsetY, tileSize);
			}

			for (auto &particle : destroyParticles) {
				particle.render(camera, boardX, boardY, tileSize, rowAnimation->getAlong(), colors);
			}

		} else {
			renderBoard(board.get(), 0, board->getHeight(), boardX, boardY, tileSize);
		}

		/* render pieces to the side of board */
		if (pieceList) {
			auto upcomingX = boardX + boardWidth + 8;
			auto numUpcoming = 6;

			/* board height minus gaps divided by num upcoming */
			auto upcomingSize = (boardHeight - (8 * (numUpcoming - 1))) / numUpcoming;

			for (auto i = 0; i < numUpcoming; ++i) {
				auto upcomingY = boardY + i * (upcomingSize + 8);

				GameResources::colorShader.enable(CNGE::Transform::toModel(upcomingX - outlineSize, upcomingY - outlineSize, upcomingSize + outlineSize * 2, upcomingSize + outlineSize * 2), camera.getProjview());
				GameResources::colorShader.giveColor(outlineColor);
				GameResources::rect.render();

				GameResources::colorShader.enable(CNGE::Transform::toModel(upcomingX, upcomingY, upcomingSize, upcomingSize), camera.getProjview());
				GameResources::colorShader.giveColor(boardColor);
				GameResources::rect.render();

				auto piece = pieceList->getPiece(numUpcoming - i - 1);
				if (piece != nullptr) {
					auto pieceRatio = f32(piece->getBoundingWidth()) / piece->getBoundingHeight();
					f32 displayWidth, displayHeight, offsetX, offsetY;

					/* width is greater than height */
					if (pieceRatio > 1) {
						displayWidth = upcomingSize - (outlineSize * 2);
						displayHeight = (1 / pieceRatio) * displayWidth;
						offsetX = outlineSize;
						offsetY = (upcomingSize - displayHeight) / 2;
						/* height is greater than width */
					} else {
						displayHeight = upcomingSize - (outlineSize * 2);
						displayWidth = pieceRatio * displayHeight;
						offsetY = outlineSize;
						offsetX = (upcomingSize - displayWidth) / 2;
					}

					auto displayTilesize = displayWidth / piece->getBoundingWidth();

					renderPiece(piece, upcomingX + offsetX - (displayTilesize * piece->getBoundingX()),
					            upcomingY + offsetY - (displayTilesize * piece->getBoundingY()), displayTilesize);
				}
			}
		}

		/* render piece and ghost */
		if (currentPiece != nullptr) {
			renderPiece(currentPiece.get(), board.get(), boardX, 16, tileSize);

			renderPiece(currentPiece.get(), ghostX, ghostY, boardX, boardY, tileSize, 0.5_f32);
		}
	}

	auto GameScene::switchScene() -> SceneSwitchReturn {
		return dontSwitch();
	}

	/* tetris */

	auto GameScene::renderBoardBack(TetrisBoard *board, f32 x, f32 y, f32 tileSize, f32 outlineSize) -> void {
		/* render board outline */
		GameResources::colorShader.enable(CNGE::Transform::toModel(x - outlineSize, y - outlineSize, tileSize * board->getWidth() + outlineSize * 2, tileSize * board->getHeight() + outlineSize * 2), camera.getProjview());
		GameResources::colorShader.giveColor(outlineColor);
		GameResources::rect.render();

		GameResources::colorShader.enable(CNGE::Transform::toModel(x, y, tileSize * board->getWidth(), tileSize * board->getHeight()), camera.getProjview());
		GameResources::colorShader.giveColor(boardColor);
		GameResources::rect.render();

		/* render horizontal gridlines */
		GameResources::colorShader.giveColor(gridColor);

		for (auto j = 1; j < board->getHeight(); ++j) {
			GameResources::colorShader.enable(CNGE::Transform::toModel(x, y + j * tileSize - tileSize / 32, tileSize * board->getWidth(), tileSize / 16), camera.getProjview());
			GameResources::rect.render();
		}

		/* render vertical gridlines */
		for (auto i = 1; i < board->getWidth(); ++i) {
			GameResources::colorShader.enable(CNGE::Transform::toModel(x + i * tileSize - tileSize / 32, y, tileSize / 16, tileSize * board->getHeight()), camera.getProjview());
			GameResources::rect.render();
		}
	}

	auto GameScene::renderBoard(TetrisBoard *board, i32 startRow, i32 endRow, f32 x, f32 y, f32 tileSize) -> void {
		/* render tiles in the board */
		for (auto j = startRow; j < endRow; ++j) {
			for (auto i = 0; i < board->getWidth(); ++i) {
				auto tile = board->get(i, j);

				if (tile != 0) {
					GameResources::colorShader.enable(CNGE::Transform::toModel(x + tileSize * i, y + tileSize * j, tileSize, tileSize), camera.getProjview());
					GameResources::colorShader.giveColor(colors[tile - 1]);

					GameResources::rect.render();
				}
			}
		}
	}

	auto GameScene::renderPiece(Piece *piece, TetrisBoard *board, f32 offsetX, f32 offsetY, f32 tileSize, f32 opacity) -> void {
		renderPiece(piece->getLayout(), piece->getBoundingSize(), board->getHeight(), piece->getX(), piece->getY(), offsetX, offsetY, tileSize, opacity);
	}

	auto GameScene::renderPiece(PieceReference *piece, f32 offsetX, f32 offsetY, f32 tileSize, f32 opacity) -> void {
		renderPiece(piece->getLayout(), piece->getBoundingSize(), 0x7fffffff, 0, 0, offsetX, offsetY, tileSize, opacity);
	}

	auto GameScene::renderPiece(Piece *piece, i32 pieceX, i32 pieceY, f32 offsetX, f32 offsetY, f32 tileSize, f32 opacity) -> void {
		renderPiece(piece->getLayout(), piece->getBoundingSize(), 0x7fffffff, pieceX, pieceY, offsetX, offsetY, tileSize, opacity);
	}

	auto GameScene::renderPiece(i32* layout, i32 boundingSize, i32 maxHeight, i32 pieceX, i32 pieceY, f32 offsetX, f32 offsetY, f32 tileSize, f32 opacity) -> void {
		for (auto j = 0; j < boundingSize; ++j) {
			for (auto i = 0; i < boundingSize; ++i) {
				auto tile = layout[j * boundingSize + i];

				if (tile != 0 && j + pieceY < maxHeight) {
					GameResources::colorShader.enable(
						CNGE::Transform::toModel(
							offsetX + tileSize * f32(i + pieceX),
							offsetY + tileSize * f32(j + pieceY),
							tileSize,
							tileSize
						),
						camera.getProjview());

					auto& color = colors[tile - 1];
					GameResources::colorShader.giveColor(color.r, color.g, color.b, opacity);

					GameResources::rect.render();
				}
			}
		}
	}

	auto GameScene::piecePlaceRoutine() -> void {
		placePiece(currentPiece.get(), board.get());
		currentPiece = nullptr;

		checkForRows(board.get(), destroyedRows);
		if (!destroyedRows.empty()) {
			rowAnimation = std::make_unique<CNGE::Timer>(0.5, true);

			destroyParticles.clear();
			destroyParticles.reserve(board->getWidth() * destroyedRows.size());

			for (auto &row : destroyedRows) {
				for (auto i = 0; i < board->getWidth(); ++i) {
					auto tile = board->get(i, row);

					if (tile) destroyParticles.emplace_back(i, row, tile);
				}
			}
		}

		downLock = true;
	}

	/**
	 * @param piece
	 * @param board
	 * @return if the piece should move down or not
	 */
	auto GameScene::movePiece(Piece *piece, TetrisBoard *board, i32 offsetX, i32 offsetY) -> bool {
		return movePiece(piece, board, piece->getX(), piece->getY(), offsetX, offsetY);
	}

	auto GameScene::movePiece(Piece *piece, TetrisBoard *board, i32 overrideX, i32 overrideY, i32 offsetX, i32 offsetY) -> bool {
		return !testCollision(piece->getLayout(), piece->getBoundingSize(), overrideX + offsetX, overrideY + offsetY, board);
	}

	auto GameScene::rotatePiece(Piece *piece, TetrisBoard *board, Rotation::RotateFunc rotateFunc, i32 *&rotatedLayout, i32 &pushX) -> bool {
		rotatedLayout = piece->rotate(rotateFunc);

		if (testCollision(rotatedLayout, piece->getBoundingSize(), piece->getX(), piece->getY(), board)) {
			delete[] rotatedLayout;
			return false;
		} else {
			return true;
		}
	}

	auto GameScene::testCollision(i32 *layout, i32 size, i32 pieceX, i32 pieceY, TetrisBoard *board) -> bool {
		for (auto j = 0; j < size; ++j)
			for (auto i = 0; i < size; ++i) {
				auto x = pieceX + i;
				auto y = pieceY + j;

				if (layout[j * size + i] && board->getCollision(x, y))
					return true;
			}

		return false;
	}

	auto GameScene::placePiece(Piece *piece, TetrisBoard *board) -> void {
		for (auto j = 0; j < piece->getBoundingSize(); ++j)
			for (auto i = 0; i < piece->getBoundingSize(); ++i)
				if (piece->getTile(i, j))
					board->set(piece->getX() + i, piece->getY() + j, piece->getTile(i, j));
	}

	auto GameScene::calculateGhost(Piece *piece, TetrisBoard *board, i32 &ghostX, i32 &ghostY) -> void {
		ghostX = piece->getX();
		ghostY = piece->getY();

		while (movePiece(piece, board, ghostX, ghostY, 0, -1)) --ghostY;
	}

	auto GameScene::checkForRows(TetrisBoard *board, std::vector<i32>& destroyedRows) -> void {
		destroyedRows.clear();

		for (auto j = 0; j < board->getHeight(); ++j) {
			auto row = true;

			for (auto i = 0; i < board->getWidth(); ++i) {
				if (!board->get(i, j)) {
					row = false;
					break;
				}
			}

			if (row) destroyedRows.push_back(j);
		}
	}

	auto GameScene::removeRows(TetrisBoard *board, std::vector<i32> &destroyRows) -> void {
		for (auto rowIndex = 0; rowIndex < destroyRows.size(); ++rowIndex) {
			auto currentRow = destroyRows[rowIndex];
			auto nextRow = rowIndex == destroyRows.size() - 1 ? board->getHeight() + 1 : destroyRows[rowIndex + 1];

			auto chunkSize = nextRow - currentRow - 1;
			auto fallDown = rowIndex + 1;

			auto start = currentRow - rowIndex;

			for (auto j = start; j < start + chunkSize; ++j)
				for (auto i = 0; i < board->getWidth(); ++i)
					board->set(i, j, (j + fallDown) >= board->getHeight() ? 0 : board->get(i, j + fallDown));
		}
	}
}
