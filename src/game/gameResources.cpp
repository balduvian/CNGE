
#include "gameResources.h"

namespace Game {
	Rect GameResources::rect = Rect();
	TextureShader GameResources::textureShader = TextureShader();
	ColorShader GameResources::colorShader = ColorShader();
	SDFShader GameResources::sdfShader = SDFShader();
	CNGE::ImageTexture<CNGE::Texture> GameResources::testTexture = CNGE::ImageTexture<CNGE::Texture>("res/test.png", CNGE::TextureParams());
	CNGE::ImageTexture<CNGE::TileGrid, u32, u32, u32> GameResources::testTileGrid = CNGE::ImageTexture<CNGE::TileGrid, u32, u32, u32>("res/gridTest.png", CNGE::TextureParams().setWrap(GL_CLAMP_TO_BORDER), 4, 3, 3);
	CNGE::Sound GameResources::testSound = CNGE::Sound("res/sound/test.wav");
	CNGE::Font GameResources::cuteFont = CNGE::Font("res/font/BalooTamma2-Medium.ttf");

	CNGE::ResourceBundle GameResources::loadScreenResources = CNGE::ResourceBundle({
		&rect, &colorShader
	});

	CNGE::ResourceBundle GameResources::gameResources = CNGE::ResourceBundle({
		&rect, &colorShader, &textureShader
	});

	CNGE::ResourceBundle GameResources::testResources = CNGE::ResourceBundle({
		&rect, &colorShader, &textureShader, &testTexture, &testTileGrid, &cuteFont, &sdfShader
	});
}
