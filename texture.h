#pragma once

namespace CNGE7
{
	class Texture
	{
	public:

		Texture(const char*);

		Texture(const char*, int);

		void bind();

		int get();

		float* get_sheet(int);

	private:

		using pixel = unsigned char;

		static float tile_values[4];

		GLuint texture;

		int width;
		int height;

		float width_fraction;

		void init(const char*);

	};
}
