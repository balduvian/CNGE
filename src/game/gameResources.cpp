
#include "gameResources.h"

namespace Game {
	Rect GameResources::rect = Rect();
	TextureShader GameResources::textureShader = TextureShader();
	FontTextureShader GameResources::fontTextureShader = FontTextureShader();
	ColorShader GameResources::colorShader = ColorShader();
	SDFShader GameResources::sdfShader = SDFShader();
	CNGE::ImageTexture<CNGE::Texture> GameResources::testTexture = CNGE::ImageTexture<CNGE::Texture>("res/test.png", CNGE::TextureParams());
	CNGE::ImageTexture<CNGE::TileGrid, u32, u32, u32> GameResources::testTileGrid = CNGE::ImageTexture<CNGE::TileGrid, u32, u32, u32>("res/gridTest.png", CNGE::TextureParams().setWrap(GL_CLAMP_TO_BORDER), 4, 3, 3);
	CNGE::Sound GameResources::testSound = CNGE::Sound("res/sound/test.wav");
	CNGE::Font GameResources::testFont = CNGE::Font("res/font.png", "res/fontData.dat", CNGE::TextureParams());

	std::vector<CNLL::Resource*> GameResources::loadScreenResources = {
		&rect, &colorShader
	};

	std::vector<CNLL::Resource*> GameResources::gameResources = {
		&testSound, &rect, &colorShader, &textureShader, &testTexture, &testFont, &fontTextureShader
	};
}
