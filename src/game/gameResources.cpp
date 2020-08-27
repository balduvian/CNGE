
#include "gameResources.h"

namespace Game {
	Rect GameResources::rect = Rect();
	TextureShader GameResources::textureShader = TextureShader();
	ColorShader GameResources::colorShader = ColorShader();
	CNGE::ImageTexture<CNGE::Texture> GameResources::testTexture = CNGE::ImageTexture<CNGE::Texture>("res/test.png", CNGE::TextureParams());
	CNGE::Sound GameResources::testSound = CNGE::Sound("res/sound/test.wav");

	CNGE::ResourceBundle GameResources::loadScreenResources = CNGE::ResourceBundle({
		&rect, &colorShader
	});

	CNGE::ResourceBundle GameResources::gameResources = CNGE::ResourceBundle({
		&rect, &colorShader, &textureShader
	});

	CNGE::ResourceBundle GameResources::testResources = CNGE::ResourceBundle({
		&rect, &colorShader, &textureShader, &testTexture
	});
}
