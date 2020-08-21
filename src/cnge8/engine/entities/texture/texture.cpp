
#include "GL/glew.h"
#include "texture.h"

namespace CNGE {
	float Texture::tileValues[4]{ 1, 1, 0, 0 };
	const float Texture::DEFAULT_TILE_VALUES[4]{ 1, 1, 0, 0 };

	/* loading */

	/// regular texture constructor
	/// without texture params, will set to default params
	Texture::Texture(const char* path, TextureParams params)
		: Resource(true), assetPath(path), assetImage(Image::makeEmpty()), width(), height(), texture(),
		horzWrap(params.horzWrap), vertWrap(params.vertWrap),
		minFilter(params.minFilter), magFilter(params.magFilter) {}

	auto Texture::customGather() -> LoadError {
		auto filename = std::filesystem::path(assetPath);

		assetImage = std::move(Image::fromPNG(filename));

		if (!assetImage.isValid())
			return LoadError(std::string("image file ") + assetPath + " could not be loaded");

		width = assetImage.getWidth();
		height = assetImage.getHeight();

		return LoadError::none();
	}

	auto Texture::customProcess() -> LoadError {
		glCreateTextures(GL_TEXTURE_2D, 1, &texture);

		bind();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, horzWrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, vertWrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, assetImage.getPixels());

		return LoadError::none();
	}

	auto Texture::customDiscard() -> LoadError {
		assetImage.invalidate();
		return LoadError::none();
	}

	auto Texture::customUnload() -> LoadError {
		glDeleteTextures(1, &texture);
		return LoadError::none();
	}

	/* use */

	auto Texture::bind(i32 slot) -> void {
		glActiveTexture(slot);
		glBindTexture(GL_TEXTURE_2D, texture);
	}

	auto Texture::get() -> u32 {
		return texture;
	}

	Texture::~Texture() {
		destroy();
	}
}
