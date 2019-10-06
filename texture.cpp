
#include "GL/glew.h"
#include "GL/gl.h"

#include <iostream>

#include "png.h"

#include "texture.h"

namespace CNGE7
{

	float Texture::tile_values[4]{};

	/// creates a texture sheet
	Texture::Texture(const char* _path, int _frames)
	{
		width_fraction = 1.f / _frames;

		try
		{
			init(_path);
		}
		catch (const char* err)
		{
			std::cout << err << std::endl;
		}
	}

	/// regular texture
	Texture::Texture(const char* _path)
	{
		width_fraction = 1.f;

		try
		{
			init(_path);
		}
		catch (const char* err)
		{
			std::cout << err << std::endl;
		}
	}

	void Texture::bind()
	{
		glBindTexture(GL_TEXTURE_2D, texture);
	}

	int Texture::get()
	{
		return texture;
	}

	float* Texture::get_sheet(int _x)
	{
		tile_values[0] = width_fraction;
		tile_values[1] = 1;
		tile_values[2] = _x * width_fraction;
		tile_values[3] = 0;

		return tile_values;
	}

	/// actually creates the texture
	/// and uses libpng
	void Texture::init(const char* _path)
	{
		// start loading the texture image
		FILE* filepoint;
		auto err = fopen_s(&filepoint, _path, "rb");

		if (err)
			throw "error opening file";

		if (!filepoint)
			throw "could not read this file!";

		{
			pixel header[8];

			fread(header, 1, 8, filepoint);

			if (png_sig_cmp(header, 0, 8))
				throw "this image isn't a png";
		}

		auto png_p = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);

		if (!png_p)
			throw "png pointer couldn't be initialized!";

		auto info_p = png_create_info_struct(png_p);

		if (!info_p)
			throw "info pointer kekd!";

		if (setjmp(png_jmpbuf(png_p)))
			throw "image could not be read!";

		png_init_io(png_p, filepoint);
		png_set_sig_bytes(png_p, 8);

		png_read_info(png_p, info_p);

		/// gather information about the image 
		width = png_get_image_width(png_p, info_p);
		height = png_get_image_height(png_p, info_p);
		auto color_type = png_get_color_type(png_p, info_p);
		auto bit_depth = png_get_bit_depth(png_p, info_p);

		// make sure it's rgba
		if (color_type == PNG_COLOR_TYPE_RGB)
			png_set_filler(png_p, 0xff, PNG_FILLER_AFTER);

		auto number_of_passes = png_set_interlace_handling(png_p);
		png_read_update_info(png_p, info_p);

		/// now get the actual image data 
		if (setjmp(png_jmpbuf(png_p)))
			throw "could not be read";

		// create the 1D array
		auto row_ps = new pixel[width * height * 4llu];
		auto row_m = row_ps;

		for (int i = 0; i < height; ++i)
		{
			auto adv = png_get_rowbytes(png_p, info_p);

			png_read_row(png_p, row_m, nullptr);
		
			row_m += adv;
		}

		fclose(filepoint);

		// create the texture
		glCreateTextures(GL_TEXTURE_2D, 1, &texture);

		bind();

		// set texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// load texture data
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, row_ps);

		//delete image
		delete[] row_ps;
	}

}
