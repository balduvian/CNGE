
#include <memory>

#include "gameSceneManager.h"
#include "gameScene.h"
#include "gameLoadScreen.h"

namespace Game {
	
	GameSceneManager::GameSceneManager() : SceneManager() {}

	auto GameSceneManager::onSceneSwitch(i32 code) -> SceneSwitchReturn {
		return { std::make_unique<GameScene>(), std::make_unique<GameLoadScreen>() };
	}

}
