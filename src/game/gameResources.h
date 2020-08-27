
#ifndef GAME_GAME_RESOURCES
#define GAME_GAME_RESOURCES

#include "cnge8/engine/entities/texture/imageTexture.h"
#include "cnge8/load/resourceBundle.h"
#include "cnge8/audio/sound.h"

#include "graphics/rect.h"
#include "graphics/colorShader.h"
#include "graphics/textureShader.h"

namespace Game {
	class GameResources {
	public:
		static Rect rect;
		static TextureShader textureShader;
		static ColorShader colorShader;
		static CNGE::ImageTexture<CNGE::Texture> testTexture;
		static CNGE::Sound testSound;

		static CNGE::ResourceBundle loadScreenResources;
		static CNGE::ResourceBundle gameResources;
		static CNGE::ResourceBundle testResources;
	};
}

#endif
