//
// Created by Emmet on 8/26/2020.
//

#include "testScene.h"
#include "../gameResources.h"
#include "../gameLoadScreen.h"
#include "gameScene.h"

namespace Game {
	TestScene::TestScene() :
		Scene(&GameResources::testResources),
		pressedSpace(false),
		aspect(16, 9, 16, 9),
		camera(), timer(2, true) {}

	auto TestScene::start() -> void {

	}

	auto TestScene::resized(u32 width, u32 height) -> void {
		aspect.update(width, height);
		camera.setOrtho(aspect.getGameWidth(), aspect.getGameHeight());
		glViewport(aspect.getLeft(), aspect.getTop(), aspect.getWidth(), aspect.getHeight());
	}

	auto TestScene::update(CNGE::Input *input, CNGE::Timing *timing) -> void {
		if (input->getKeyPressed(GLFW_KEY_SPACE)) pressedSpace = true;

		timer.updateContinual(timing->time);

		camera.update();
	}

	auto TestScene::render() -> void {
		glClearColor(0, 0.5, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		GameResources::testTileGrid.get()->bind();
		GameResources::textureShader.enable(CNGE::Transform::toModel(1, 1, 3, 3), camera.getProjview());
		GameResources::textureShader.giveParams(GameResources::testTileGrid.get()->getSheet(1, 0));
		GameResources::rect.render();

		auto str = std::string("hello there");

		GameResources::cuteFont.getTexture()->bind();
		GameResources::cuteFont.render(str.data(), str.length(), timer.getAlong(), 1, 1, 2, 0.05, [this](CNGE::TileGrid* texture, u32 index, f32 along, u32 sheetX, u32 sheetY, f32 x, f32 y, f32 width, f32 height) {
			GameResources::sdfShader.enable(CNGE::Transform::toModel(x, y, width, height), camera.getProjview());
			GameResources::sdfShader.giveParams(1, 1, 1, along, texture->getSheet(sheetX, sheetY));
			GameResources::rect.render();
		});
	}

	auto TestScene::switchScene() -> SceneSwitchReturn {
		if (pressedSpace) {
			return { std::make_unique<GameScene>(), std::make_unique<GameLoadScreen>() };
		}

		return dontSwitch();
	}
}
