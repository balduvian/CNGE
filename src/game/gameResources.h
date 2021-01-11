
#ifndef GAME_GAME_RESOURCES
#define GAME_GAME_RESOURCES

#include "game/graphics/fontTextureShader.h"
#include "game/graphics/textureShader.h"
#include "cnge/engine/entities/texture/tileGrid.h"
#include "cnge/engine/entities/texture/imageTexture.h"
#include "cnge/engine/entities/sound/sound.h"
#include "cnge/engine/entities/font/font.h"
#include "cnge/load/resource.h"

#include "graphics/rect.h"
#include "graphics/colorShader.h"
#include "graphics/textureShader.h"
#include "graphics/sdfShader.h"

namespace Game {
	class GameResources {
	public:
		static Rect rect;
		static TextureShader textureShader;
		static FontTextureShader fontTextureShader;
		static ColorShader colorShader;
		static SDFShader sdfShader;
		static CNGE::ImageTexture<CNGE::Texture> testTexture;
		static CNGE::ImageTexture<CNGE::TileGrid, u32, u32, u32> testTileGrid;
		static CNGE::Sound testSound;
		static CNGE::Font testFont;

		static std::vector<CNLL::Resource*> loadScreenResources;
		static std::vector<CNLL::Resource*> gameResources;
	};
}

#endif
