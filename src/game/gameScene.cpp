
#include "gameScene.h"

#include "cnge8/math/math.h"
#include "cnge8/engine/transform.h"
#include "gameResources.h"

#include <iostream>

namespace Game {
	const CNGE::Color GameScene::skekColor = CNGE::Color(0x31d433);

	GameScene::GameScene()
		: aspect(512, 288, 688, 288),
		Scene(&GameResources::gameResources),
		randomDevice(),
		tickTimer(0.5),
		referencePieces({
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
        }),
        board(std::make_unique<TetrisBoard>(10, 20)),
        currentPiece(nullptr)
	{}

	CNGE::Color GameScene::colors[6] {
		CNGE::Color(252, 186, 3),
		CNGE::Color(63, 212, 55),
		CNGE::Color(36, 242, 242),
		CNGE::Color(29, 66, 196),
		CNGE::Color(149, 24, 222),
		CNGE::Color(227, 130, 27)
	};

	CNGE::Color GameScene::boardColor = CNGE::Color(2, 25, 41);
	CNGE::Color GameScene::gridColor = CNGE::Color(209, 227, 240);

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
		angle += CNGE::PI<f32> * timing->time;

		if (tickTimer.updateContinual(timing->time)) {
			if (currentPiece == nullptr) {
				auto engine = std::default_random_engine(randomDevice());
				auto dist = std::uniform_int_distribution<i32>(0, referencePieces.size() - 1);

				currentPiece = std::make_unique<Piece>(referencePieces[dist(engine)]);
				currentPiece->setPosition((board->getWidth() / 2) - (currentPiece->getBoundingSize() / 2), board->getHeight() - 1);

				std::cout << currentPiece->getX() << std::endl;

			} else {
				currentPiece->moveY(-1);

				if (currentPiece->getY() == -1) {
					currentPiece = nullptr;
				}
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
		auto tileSize = boardHeight / board->getHeight();

		renderBoard(board.get(), boardX, 16, tileSize);

		if (currentPiece != nullptr) {
			renderPiece(currentPiece.get(), boardX, 16, tileSize);
		}
	}

	auto GameScene::switchScene() -> SceneSwitchReturn {
		return dontSwitch();
	}

	/* tetris */

	auto GameScene::renderBoard(TetrisBoard *board, f32 x, f32 y, f32 tileSize) -> void {
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

		/* render tiles in the board */
		for (auto j = 0; j < board->getHeight(); ++j) {
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

	auto GameScene::renderPiece(Piece *piece, f32 x, f32 y, f32 tileSize) -> void {
		for (auto j = 0; j < piece->getBoundingSize(); ++j) {
			for (auto i = 0; i < piece->getBoundingSize(); ++i) {
				auto tile = piece->getTile(i, j);

				if (tile != 0) {
					GameResources::colorShader.enable(
						CNGE::Transform::toModel(
							x + tileSize * (i + piece->getX()),
							y + tileSize * (j + piece->getY()),
							tileSize,
							tileSize
						),
					camera.getProjview());

					GameResources::colorShader.giveColor(colors[tile - 1]);

					GameResources::rect.render();
				}
			}
		}
	}
}
