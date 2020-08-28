//
// Created by Emmet on 8/26/2020.
//

#ifndef CNGE_8_2_TESTSCENE_H
#define CNGE_8_2_TESTSCENE_H

#include <cnge8/util/timer.h>
#include "cnge8/scene/scene.h"
#include "cnge8/util/aspect.h"
#include "cnge8/engine/camera.h"

namespace Game {
	class TestScene : public CNGE::Scene {
	private:
		bool pressedSpace;
		CNGE::Aspect aspect;
		CNGE::Camera camera;

		CNGE::Timer timer;

	public:
		TestScene();

		auto start() -> void override;

		auto resized(u32, u32) -> void override;

		auto update(CNGE::Input*, CNGE::Timing*) -> void override;

		auto render() -> void override;

		auto switchScene() -> SceneSwitchReturn override;
	};
}

#endif //CNGE_8_2_TESTSCENE_H
