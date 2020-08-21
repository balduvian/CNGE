
#include "scene.h"

namespace CNGE {
	Scene::Scene(ResourceBundle* bundle)
		: bundle(bundle) {}

	auto Scene::dontSwitch() -> SceneSwitchReturn {
		return { nullptr, nullptr };
	}

	auto Scene::getBundle() -> ResourceBundle* {
		return bundle;
	}
}
