//
// Created by Emmet on 8/26/2020.
//

#include "testScene.h"
#include "../gameResources.h"
#include "../gameLoadScreen.h"
#include "gameScene.h"

#include "cnge8/engine/transform.h"

namespace Game {
	TestScene::TestScene() :
		Scene(&GameResources::testResources),
		pressedSpace(false),
		aspect(16, 9, 16, 9),
		camera() {}

	auto TestScene::start() -> void {

	}

	auto TestScene::resized(u32 width, u32 height) -> void {
		aspect.update(width, height);
		camera.setOrtho(aspect.getGameWidth(), aspect.getGameHeight());
		glViewport(aspect.getLeft(), aspect.getTop(), aspect.getWidth(), aspect.getHeight());
	}

	auto TestScene::update(CNGE::Input *input, CNGE::Timing *timing) -> void {
		if (input->getKeyPressed(GLFW_KEY_SPACE)) pressedSpace = true;
		camera.update();
	}

	auto TestScene::render() -> void {
		glClearColor(0, 0.5, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		GameResources::testTexture.get()->bind();
		GameResources::textureShader.enable(CNGE::Transform::toModel(1, 1, 6, 6), camera.getProjview());
		GameResources::textureShader.giveParams(1, 1, 1, 1, CNGE::Texture::DEFAULT_TILE_VALUES);

		GameResources::rect.render();
	}

	auto TestScene::switchScene() -> SceneSwitchReturn {
		if (pressedSpace) {
			return { std::make_unique<GameScene>(), std::make_unique<GameLoadScreen>() };
		}

		return dontSwitch();
	}
}
