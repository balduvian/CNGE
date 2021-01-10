//
// Created by Emmet on 8/26/2020.
//

#ifndef CNGE_8_2_IMAGETEXTURE_H
#define CNGE_8_2_IMAGETEXTURE_H

#include <type_traits>
#include <concepts>
#include <tuple>
#include "texture.h"
#include "cnge/load/resource.h"

namespace CNGE {
	template<typename T>
	concept TextureType = requires(T t) {
		std::is_base_of<Texture, T>();
	};

	template<TextureType T, typename ...Parameters>
	class ImageTexture : public CNLL::Resource {
	public:
		ImageTexture(const char* path, TextureParams textureParams, Parameters ...inputParameters)
			: Resource(true), texture(nullptr), textureParams(textureParams), assetPath(path), assetImage(Image::makeEmpty()), parameters(std::make_tuple(inputParameters...)) {}

		~ImageTexture() {
			onDestroy();
		}

		auto get() -> T* {
			return texture.get();
		}

		auto customGather() -> bool override {
			auto filename = std::filesystem::path(assetPath);

			assetImage = Image::fromPNG(filename);

			return assetImage.isValid();
		}

		auto customDiscard() -> void override {
			assetImage.invalidate();
		}

		auto customLoad() -> void override {
			std::apply([this](Parameters ...parameters) {
				texture = std::make_unique<T>(assetImage.getWidth(), assetImage.getHeight(), assetImage.getPixels(), textureParams, parameters...);
			}, parameters);
		}

		auto  customUnload() -> void override {
			texture = nullptr;
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
