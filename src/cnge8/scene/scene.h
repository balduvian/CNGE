
#ifndef CNGE_SCENE
#define CNGE_SCENE

#include "../types.h"
#include "../load/resource.h"
#include "../engine/loop/loop.h"
#include "../engine/window.h"
#include "../load/loadScreen.h"
#include "cnge8/load/resourceBundle.h"

namespace CNGE {
	class Scene {
	private:
        ResourceBundle* bundle;

	protected:
		void switchScene(i32);

	public:
        Scene(ResourceBundle* bundle);

        virtual auto start() -> void = 0;
        virtual auto resized(u32, u32) -> void = 0;
        virtual auto update(Input*, Timing*) -> void = 0;
        virtual auto render() -> void = 0;

		struct SceneSwitchReturn {
			std::unique_ptr<Scene>&& scene;
			std::unique_ptr<LoadScreen>&& loadScreen;
		};

		static auto dontSwitch() -> SceneSwitchReturn;
        virtual auto switchScene() -> SceneSwitchReturn = 0;

        auto getBundle() -> ResourceBundle*;
	};
}

#endif
