
#ifndef GAME_GAME_SCENE 
#define GAME_GAME_SCENE

#include "cnge8/engine/camera.h"
#include "cnge8/util/aspect.h"
#include "cnge8/engine/loop/timing.h"
#include "cnge8/scene/scene.h"
#include "cnge8/util/color.h"

#include <memory>
#include <random>

#include "cnge8/util/timer.h"
#include "tetris/tetrisBoard.h"
#include "tetris/piece.h"

namespace Game {
	class GameScene : public CNGE::Scene {
	private:
		f32 angle = 0;
		const static CNGE::Color skekColor;

		CNGE::Camera camera;
		CNGE::Aspect aspect;

		/* tetris */

		std::random_device randomDevice;

		CNGE::Timer tickTimer;
		std::vector<Piece> referencePieces;

		std::unique_ptr<TetrisBoard> board;
		std::unique_ptr<Piece> currentPiece;

		static CNGE::Color colors[];
		static CNGE::Color boardColor;
		static CNGE::Color gridColor;

	public:
		GameScene();

		auto start() -> void override;

		auto resized(u32, u32) -> void override;

		auto update(CNGE::Input*, CNGE::Timing*) -> void override;

		auto render() -> void override;

		auto switchScene() -> SceneSwitchReturn override;

		/* tetris */

		auto renderBoard(TetrisBoard*, f32, f32, f32) -> void;
		auto renderPiece(Piece*, f32, f32, f32) -> void;
		//auto renderPieceArbitrary(Piece*, f32, f32, f32) -> void;

		//auto doTick() -> void;
	};
}

#endif
