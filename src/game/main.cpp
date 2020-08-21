
#include <functional>
#include <cnge8/scene/sceneManager.h>

#include "cnge8/audio/audioDevice.h"

#include "gameResources.h"
#include "gameScene.h"
#include "gameLoadScreen.h"

auto main(int argc, char** argv) -> int {
	/* init glfw and opengl */
	CNGE::Window::init();
	auto window = CNGE::Window(3, 3, true, true, "CNGE Test Demo", CNGE::Window::getPrimaryMonitor(), false, true);
	CNGE::Window::wrangle();

	/* init openal */
	CNGE::AudioDevices::init();
	CNGE::AudioDevices::getDefaultOutputDevice()->setActive();

	/* open enables */
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CLIP_DISTANCE0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// get load screen ready
	Game::GameResources::loadScreenResources.setup().quickUpdateLoad(true);

	auto sceneManager = CNGE::SceneManager();

	sceneManager.start(window.getInput(), std::make_unique<Game::GameScene>(), std::make_unique<Game::GameLoadScreen>());

	// start the gameloop
	auto loop = CNGE::Loop();

	loop.begin([&window]() { return window.getShouldClose(); }, [&](CNGE::Timing* timing) {
		window.poll();

		sceneManager.update(window.getInput(), timing);

		window.swap();
	});

	return 0;
}
