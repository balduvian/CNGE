//
// Created by Emmet on 8/26/2020.
//

#ifndef CNGE_8_2_IMAGETEXTURE_H
#define CNGE_8_2_IMAGETEXTURE_H

#include <type_traits>
#include <concepts>
#include <tuple>
#include "texture.h"

namespace CNGE {
	template<typename T>
	concept TextureType = requires(T t) {
		std::is_base_of<Texture, T>();
	};

	template<TextureType T, typename ...Parameters>
	class ImageTexture : public Resource {
	public:
		ImageTexture(const char* path, TextureParams textureParams, Parameters ...inputParameters)
			: Resource(true), texture(nullptr), textureParams(textureParams), assetPath(path), assetImage(Image::makeEmpty()), parameters(std::make_tuple(inputParameters...))
	{}

	auto get() -> T* {
		return texture.get();
	}

	auto customGather() -> LoadError override {
		auto filename = std::filesystem::path(assetPath);

		assetImage = Image::fromPNG(filename);

		if (!assetImage.isValid())
			return LoadError(std::string("image file ") + assetPath + " could not be loaded");

		return LoadError::none();
	}

	auto customProcess() -> LoadError override {
		std::apply([this](Parameters ...parameters) {
			texture = std::make_unique<T>(assetImage.getWidth(), assetImage.getHeight(), assetImage.getPixels(),
			                              textureParams, parameters...);
		}, parameters);

		return LoadError::none();
	}

	auto customDiscard() -> LoadError override {
		assetImage.invalidate();
		return LoadError::none();
	}

	auto  customUnload() -> LoadError override {
		texture = nullptr;
		return LoadError::none();
	}

	private:
		std::unique_ptr<T> texture;
		TextureParams textureParams;
		std::tuple<Parameters...> parameters;

		const char* assetPath;
		Image assetImage;
	};
}

#endif //CNGE_8_2_IMAGETEXTURE_H
