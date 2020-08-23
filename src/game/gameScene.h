
#ifndef GAME_GAME_SCENE 
#define GAME_GAME_SCENE

#include "cnge8/engine/camera.h"
#include "cnge8/util/aspect.h"
#include "cnge8/engine/loop/timing.h"
#include "cnge8/scene/scene.h"
#include "cnge8/util/color.h"
#include "cnge8/util/timer.h"

#include <memory>
#include <random>

#include "tetris/destroyParticle.h"
#include "tetris/tetrisBoard.h"
#include "tetris/piece.h"
#include "tetris/pieceList.h"
#include "tetris/animation.h"

namespace Game {
	class GameScene : public CNGE::Scene {
	private:
		const static CNGE::Color skekColor;

		CNGE::Camera camera;
		CNGE::Aspect aspect;

		/* tetris */

		std::random_device randomDevice;

		CNGE::Timer tickTimer;

		std::unique_ptr<TetrisBoard> board;
		std::unique_ptr<PieceList> pieceList;
		std::unique_ptr<Piece> currentPiece;
		i32 ghostX, ghostY;

		std::unique_ptr<CNGE::Timer> rowAnimation;
		std::vector<DestroyParticle> destroyParticles;
		i32 startDestroyRow, numDestroyRows;

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

		auto renderBoardBack(TetrisBoard*, f32, f32, f32) -> void;
		auto renderBoard(TetrisBoard*, i32, i32, f32, f32, f32) -> void;
		auto renderPiece(Piece*, f32, f32, f32, f32 = 1.0_f32) -> void;
		auto renderPiece(Piece*, i32, i32, f32, f32, f32, f32 = 1.0_f32) -> void;

		auto piecePlaceRoutine() -> void;

		static auto movePiece(Piece*, TetrisBoard*, i32, i32) -> bool;
		static auto movePiece(Piece*, TetrisBoard*, i32, i32, i32, i32) -> bool;
		static auto placePiece(Piece*, TetrisBoard*) -> void;

		static auto calculateGhost(Piece*, TetrisBoard*, i32&, i32&) -> void;

		static auto checkForRows(TetrisBoard*, i32&, i32&) -> void;
		static auto removeRows(TetrisBoard*, i32, i32) -> void;
	};
}

#endif
