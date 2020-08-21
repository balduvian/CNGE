
#include "gameScene.h"

#include "cnge8/math/math.h"
#include "cnge8/engine/transform.h"
#include "gameResources.h"
#include "cnge8/math/math.h"

#include <iostream>

namespace Game {
	const CNGE::Color GameScene::skekColor = CNGE::Color(0x31d433);

	GameScene::GameScene() : aspect(16, 9, 16, 9), Scene(&GameResources::gameResources) {}

	auto GameScene::start() -> void {
		GameResources::testSound.loop(true);
		GameResources::testSound.play();
	}

	auto GameScene::resized(u32 width, u32 height) -> void {
		aspect.update(width, height);
		camera.setOrtho(aspect.getGameWidth(), aspect.getGameHeight());
		glViewport(aspect.getLeft(), aspect.getTop(), aspect.getWidth(), aspect.getHeight());
	}

	auto GameScene::update(CNGE::Input* input, CNGE::Timing* timing) -> void {
		angle += CNGE::PI<f32> * timing->time;

		camera.update();
	}

	auto GameScene::render() -> void {
		GameResources::colorShader.enable();
		GameResources::colorShader.giveColor(skekColor);
		GameResources::rect.render();

		auto invColor = skekColor.invert();

		GameResources::colorShader.enable(CNGE::Transform::toModelCenterRotate(2, 2, 1, 1, angle), camera.getProjview());
		GameResources::colorShader.giveColor(invColor);
		GameResources::rect.render();
	}

	auto GameScene::switchScene() -> SceneSwitchReturn {
		return dontSwitch();
	}
}
