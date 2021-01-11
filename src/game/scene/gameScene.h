
#ifndef GAME_GAME_SCENE 
#define GAME_GAME_SCENE

#include "cnge/engine/camera.h"
#include "cnge/util/aspect.h"
#include "cnge/engine/loop/timing.h"
#include "cnge/scene/scene.h"
#include "cnge/util/color.h"
#include "cnge/util/timer.h"

#include <memory>
#include <random>

#include "../tetris/destroyParticle.h"
#include "../tetris/tetrisBoard.h"
#include "../tetris/pieceReference.h"
#include "../tetris/pieceList.h"
#include "../tetris/animation.h"
#include "../tetris/rotation.h"

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
		std::vector<i32> destroyedRows;

		bool downLock;

		i32 moveCounter;

		i32 score;

		std::unique_ptr<PieceReference> hold;
		bool usedHold;

		static CNGE::Color colors[];
		static CNGE::Color boardColor;
		static CNGE::Color gridColor;
		static CNGE::Color outlineColor;

	public:
		GameScene();

		auto start() -> void override;

		auto resized(u32, u32) -> void override;

		auto update(CNGE::Input*, CNGE::Timing*) -> void override;

		auto render() -> void override;

		auto switchScene() -> SceneSwitchReturn override;

		/* tetris */

		auto renderBoardBack(TetrisBoard*, f32, f32, f32, f32) -> void;
		auto renderBoard(TetrisBoard*, i32, i32, f32, f32, f32) -> void;
		auto renderSide(PieceList*, PieceReference*, f32, f32, f32, f32, f32) -> void;

		auto renderPiece(Piece*, TetrisBoard*, f32, f32, f32, f32 = 1.0_f32) -> void;
		auto renderPiece(PieceReference*, f32, f32, f32, f32 = 1.0_f32) -> void;
		auto renderPiece(Piece*, i32, i32, f32, f32, f32, f32 = 1.0_f32) -> void;
		auto renderPiece(i32*, i32, i32, i32, i32, f32, f32, f32, f32 = 1.0_f32) -> void;

		auto piecePlaceRoutine() -> void;
		auto moveTimer() -> void;
		auto newPiece(PieceReference*, bool) -> void;

		static auto movePiece(Piece*, TetrisBoard*, i32, i32) -> bool;
		static auto movePiece(Piece*, TetrisBoard*, i32, i32, i32, i32) -> bool;
		static auto rotatePiece(Piece*, TetrisBoard*, Rotation::RotateFunc, i32*&, i32&) -> bool;
		static auto placePiece(Piece*, TetrisBoard*) -> void;

		static auto testCollision(i32*, i32, i32, i32, TetrisBoard*) -> bool;
		static auto calculateGhost(Piece*, TetrisBoard*, i32&, i32&) -> void;
		static auto checkForRows(TetrisBoard*, std::vector<i32>&) -> void;

		static auto removeRows(TetrisBoard*, std::vector<i32>&) -> void;
		struct PositionReturn { i32 x, y; };
		static auto initialPosition(PieceReference*, TetrisBoard*) -> PositionReturn;
	};
}

#endif
