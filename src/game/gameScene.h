
#ifndef GAME_GAME_SCENE 
#define GAME_GAME_SCENE

#include "cnge8/engine/camera.h"
#include "cnge8/util/aspect.h"
#include "cnge8/engine/loop/timing.h"
#include "cnge8/scene/scene.h"
#include "cnge8/util/color.h"

namespace Game {
	class GameScene : public CNGE::Scene {
	private:
		const static CNGE::Color skekColor;

		CNGE::Camera camera;
		CNGE::Aspect aspect;
		
	public:
		GameScene();

		auto start() -> void override;

		auto resized(u32, u32) -> void override;

		auto update(CNGE::Input*, CNGE::Timing*) -> void override;

		auto render() -> void override;
	};
}

#endif
