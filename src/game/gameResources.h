
#ifndef GAME_GAME_RESOURCES
#define GAME_GAME_RESOURCES

#include <cnge8/engine/entities/texture/tileGrid.h>
#include <cnge8/engine/entities/font/font.h>
#include "cnge8/engine/entities/texture/imageTexture.h"
#include "cnge8/load/resourceBundle.h"
#include "cnge8/audio/sound.h"

#include "graphics/rect.h"
#include "graphics/colorShader.h"
#include "graphics/textureShader.h"
#include "graphics/sdfShader.h"

namespace Game {
	class GameResources {
	public:
		static Rect rect;
		static TextureShader textureShader;
		static ColorShader colorShader;
		static SDFShader sdfShader;
		static CNGE::ImageTexture<CNGE::Texture> testTexture;
		static CNGE::ImageTexture<CNGE::TileGrid, u32, u32, u32> testTileGrid;
		static CNGE::Sound testSound;
		static CNGE::Font cuteFont;

		static CNGE::ResourceBundle loadScreenResources;
		static CNGE::ResourceBundle gameResources;
		static CNGE::ResourceBundle testResources;
	};
}

#endif
