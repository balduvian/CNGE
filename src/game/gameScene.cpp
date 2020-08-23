
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
        pieceList(std::make_unique<PieceList>(std::initializer_list<Piece>{
	          Piece(3, new i32[] {
	              1, 0, 0,
	              1, 1, 1,
	              0, 0, 0
	          }),
	          Piece(3, new i32[] {
	              0, 0, 2,
	              2, 2, 2,
	              0, 0, 0
	          }),
	          Piece(3, new i32[] {
	              0, 0, 3,
	              0, 3, 3,
	              0, 3, 0
	          }),
	          Piece(3, new i32[] {
	              4, 0, 0,
	              4, 4, 0,
	              0, 4, 0
	          }),
	          Piece(2, new i32[] {
	              5, 5,
	              5, 5
	          }),
	          Piece(4, new i32[] {
	              0, 0, 0, 0,
	              6, 6, 6, 6,
	              0, 0, 0, 0,
	              0, 0, 0, 0
	          })
        })),
        currentPiece(nullptr),
        rowAnimation(nullptr),
        destroyParticles()
	{
		pieceList->reset();
	}

	CNGE::Color GameScene::colors[6] {
		CNGE::Color(252, 186, 3),
		CNGE::Color(63, 212, 55),
		CNGE::Color(36, 242, 242),
		CNGE::Color(29, 66, 196),
		CNGE::Color(149, 24, 222),
		CNGE::Color(227, 130, 27)
	};

	CNGE::Color GameScene::boardColor = CNGE::Color(2, 25, 41);
	CNGE::Color GameScene::gridColor = CNGE::Color(95, 104, 110);

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
				removeRows(board.get(), startDestroyRow, numDestroyRows);
				rowAnimation = nullptr;
			}
		} else {
			if (tickTimer.updateContinual(timing->time)) {
				if (currentPiece == nullptr) {
					currentPiece = std::make_unique<Piece>(*pieceList->dequeue());
					currentPiece->setPosition((board->getWidth() / 2) - (currentPiece->getBoundingSize() / 2), board->getHeight() - 1);
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
				currentPiece->setRotated(currentPiece->rotate(Rotation::rotatePositive));
				calculateGhost(currentPiece.get(), board.get(), ghostX, ghostY);
			} else if (input->getKeyDown(GLFW_KEY_DOWN)) {
				if (movePiece(currentPiece.get(), board.get(), 0, -1))
					currentPiece->moveY(-1);
				else
					piecePlaceRoutine();
			} else if (input->getKeyPressed(GLFW_KEY_SPACE)) {
				currentPiece->setXY(ghostX, ghostY);
				piecePlaceRoutine();
			}
		}

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

		/* render board */
		renderBoardBack(board.get(), boardX, 16, tileSize);

		if (rowAnimation) {
			/* stationary blocks below the destroyed */
			renderBoard(board.get(), 0, startDestroyRow - 1, boardX, boardY, tileSize);

			/* falling blocks above the destroyed */
			auto offsetY = CNGE::interpSquared<f32>(0, -tileSize * numDestroyRows, rowAnimation->getAlong());
			renderBoard(board.get(), startDestroyRow + numDestroyRows, board->getHeight() - 1, boardX, boardY + offsetY, tileSize);

			for (auto &particle : destroyParticles) {
				particle.render(camera, boardX, boardY, tileSize, rowAnimation->getAlong(), colors);
			}

		} else {
			renderBoard(board.get(), 0, board->getHeight() - 1, boardX, boardY, tileSize);
		}

		/* render piece and ghost */
		if (currentPiece != nullptr) {
			renderPiece(currentPiece.get(), boardX, 16, tileSize);

			renderPiece(currentPiece.get(), ghostX, ghostY, boardX, boardY, tileSize, 0.5_f32);
		}
	}

	auto GameScene::switchScene() -> SceneSwitchReturn {
		return dontSwitch();
	}

	/* tetris */
	auto GameScene::renderBoardBack(TetrisBoard *board, f32 x, f32 y, f32 tileSize) -> void {
		/* render board outline */
		GameResources::colorShader.enable(CNGE::Transform::toModel(x - tileSize / 8, y - tileSize / 8, tileSize * board->getWidth() + tileSize / 4, tileSize * board->getHeight() + tileSize / 4), camera.getProjview());
		GameResources::colorShader.giveColor(gridColor);
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
		for (auto j = startRow; j <= endRow; ++j) {
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

	auto GameScene::renderPiece(Piece *piece, f32 x, f32 y, f32 tileSize, f32 opacity) -> void {
		renderPiece(piece, piece->getX(), piece->getY(), x, y, tileSize, opacity);
	}

	auto GameScene::renderPiece(Piece *piece, i32 overrideX, i32 overrideY, f32 x, f32 y, f32 tileSize, f32 opacity) -> void {
		for (auto j = 0; j < piece->getBoundingSize(); ++j) {
			for (auto i = 0; i < piece->getBoundingSize(); ++i) {
				auto tile = piece->getTile(i, j);

				if (tile != 0) {
					GameResources::colorShader.enable(
						CNGE::Transform::toModel(
							x + tileSize * f32(i + overrideX),
							y + tileSize * f32(j + overrideY),
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

		checkForRows(board.get(), startDestroyRow, numDestroyRows);
		if (numDestroyRows != 0) {
			rowAnimation = std::make_unique<CNGE::Timer>(0.5, true);

			destroyParticles.clear();
			destroyParticles.reserve(board->getWidth() * numDestroyRows);

			for (auto j = startDestroyRow; j < startDestroyRow + numDestroyRows; ++j) {
				for (auto i = 0; i < board->getWidth(); ++i) {
					destroyParticles.emplace_back(i, j, board->get(i, j));
				}
			}
		}
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
		for (auto j = 0; j < piece->getBoundingSize(); ++j)
			for (auto i = 0; i < piece->getBoundingSize(); ++i) {
				auto x = overrideX + i;
				auto y = overrideY + j;

				auto afterX = x + offsetX;
				auto afterY = y + offsetY;

				if (
					x >= 0 && x < board->getWidth() &&
					y >= 0 && y < board->getHeight() &&
					piece->getTile(i, j) &&
					(afterY == -1 || afterX == -1 || afterX == board->getWidth() || board->get(afterX, afterY))
				)
					return false;
			}

		return true;
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

	auto GameScene::checkForRows(TetrisBoard *board, i32& startRow, i32& numRows) -> void {
		auto hasFound = false;
		startRow = 0;
		numRows = 0;

		for (auto j = 0; j < board->getHeight(); ++j) {
			auto row = true;

			for (auto i = 0; i < board->getWidth(); ++i) {
				if (!board->get(i, j)) {
					row = false;
					break;
				}
			}

			if (row) {
				if (!hasFound) {
					hasFound = true;
					startRow = j;
				}

				++numRows;
			}
			else if (hasFound) return;
		}
	}

	auto GameScene::removeRows(TetrisBoard *board, i32 startRow, i32 numRows) -> void {
		for (auto j = startRow; j < board->getHeight() - numRows; ++j)
			for (auto i = 0; i < board->getWidth(); ++i)
				board->set(i, j, board->get(i, j + numRows));
	}
}
