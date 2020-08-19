
#include <iostream>
#include <stdio.h>
#include <memory>
#include <setjmp.h>

#include "image.h"

#include "libpng16/png.h"
#include "jpeglib.h"

#include "imageUtil.h"

namespace CNGE {
	Image::Image(u32 width, u32 height, u32* pixels)
		: width(width), height(height), pixels(pixels) {}

	Image::Image(Image&& other) : width(other.width), height(other.height), pixels(other.pixels) {
		other.pixels = nullptr;
	}

	auto Image::operator=(Image&& other) -> Image& {
		width = other.width;
		height = other.height;
		pixels = other.pixels;
		other.pixels = nullptr;
	}

	Image::~Image() {
		delete[] pixels;
	}

	auto Image::fromPNG(std::filesystem::path& path) -> Image {
		auto* file = static_cast<FILE*>(nullptr);

		/* open the file of the image */
		_wfopen_s(&file, path.c_str(), L"rb");

		if (!file) return makeEmpty();

		auto* png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
		auto* info = png_create_info_struct(png);

		png_init_io(png, file);
		png_read_info(png, info);

		auto width = png_get_image_width(png, info);
		auto height = png_get_image_height(png, info);
		auto* pixels = new u32[static_cast<unsigned long long>(width) * height];

		/* convert the image into 8 bit rgba */
		const auto colorType = png_get_color_type(png, info);
		const auto bitDepth = png_get_bit_depth(png, info);

		if (bitDepth == 16)
			png_set_strip_16(png);

		if (colorType == PNG_COLOR_TYPE_PALETTE)
			png_set_palette_to_rgb(png);

		if (colorType == PNG_COLOR_TYPE_GRAY && bitDepth < 8)
			png_set_expand_gray_1_2_4_to_8(png);

		if (png_get_valid(png, info, PNG_INFO_tRNS))
			png_set_tRNS_to_alpha(png);

		if (colorType == PNG_COLOR_TYPE_RGB || colorType == PNG_COLOR_TYPE_GRAY || colorType == PNG_COLOR_TYPE_PALETTE)
			png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

		if (colorType == PNG_COLOR_TYPE_GRAY || colorType == PNG_COLOR_TYPE_GRAY_ALPHA)
			png_set_gray_to_rgb(png);

		png_read_update_info(png, info);

		/* cache the row so we can convert it */
		auto* pngRow = new u8[width * 4llu];

		/* read each row into the 1d array */
		for (auto j = 0u; j < height; ++j) {
			png_read_row(png, pngRow, nullptr);

			/* merge bytes into ints */
			for (auto i = 0u; i < width; ++i) {
				const auto red = pngRow[i * 4];
				const auto gre = pngRow[i * 4 + 1];
				const auto blu = pngRow[i * 4 + 2];
				const auto alp = pngRow[i * 4 + 3];

				pixels[Util::pos(i, j, width)] = Util::pix(red, gre, blu, alp);
			}
		}

		delete[] pngRow;

		png_destroy_read_struct(&png, &info, nullptr);
		fclose(file);

		return Image(width, height, pixels);
	}

	struct my_error_mgr {
		struct jpeg_error_mgr pub;	/* "public" fields */

		jmp_buf setjmp_buffer;	/* for return to caller */
	};
	
	auto my_error_exit(j_common_ptr cinfo) {
		/* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
		auto* myerr = reinterpret_cast<my_error_mgr*>(cinfo->err);

		/* Always display the message. */
		/* We could postpone this until after returning, if we chose. */
		(*cinfo->err->output_message) (cinfo);

		/* Return control to the setjmp point */
		longjmp(myerr->setjmp_buffer, 1);
	}
	
	auto Image::fromJPG(std::filesystem::path& path) -> Image {
		struct jpeg_decompress_struct cinfo;
		struct my_error_mgr jerr;

		/* More stuff */
		FILE* file;      /* source file */

		_wfopen_s(&file, path.c_str(), L"rb");

		if (!file) return makeEmpty();

		/* We set up the normal JPEG error routines, then override error_exit. */
		cinfo.err = jpeg_std_error(&jerr.pub);
		jerr.pub.error_exit = my_error_exit;
		/* Establish the setjmp return context for my_error_exit to use. */
		if (setjmp(jerr.setjmp_buffer)) {

			jpeg_destroy_decompress(&cinfo);
			fclose(file);

			return makeEmpty();
		}

		jpeg_create_decompress(&cinfo);

		jpeg_stdio_src(&cinfo, file);

		(void)jpeg_read_header(&cinfo, TRUE);

		(void)jpeg_start_decompress(&cinfo);

		auto channels = cinfo.output_components;
		
		auto row_stride = cinfo.output_width * channels;

		auto width = cinfo.output_width;
		auto height = cinfo.output_height;
		auto* pixels = new u32[u64(width) * height];

		auto counter = 0llu;

		auto bufferRow = std::unique_ptr<u8>(new u8[row_stride]);
		auto* row = bufferRow.get();

		while (cinfo.output_scanline < cinfo.output_height) {
			jpeg_read_scanlines(&cinfo, &row, 1);
			
			for (auto i = 0u; i < width; ++i)
				pixels[i + counter] = Util::pix(row[i * channels], row[i * channels + 1], row[i * channels + 2], 0xff);

			counter += width;
		}

		jpeg_finish_decompress(&cinfo);

		/* cleanup */
		jpeg_destroy_decompress(&cinfo);
		fclose(file);

		return Image(width, height, pixels);
	}

	auto Image::makeSheet(u32 width, u32 height) -> Image {
		return Image(width, height, new u32[static_cast<unsigned long long>(width) * height]());
	}

	auto Image::makeEmpty() -> Image {
		return Image(0, 0, nullptr);
	}

	auto Image::resize(u32 width, u32 height) -> void {
		delete[] pixels;
		this->pixels = new u32[width * height];

		this->width = width;
		this->height = height;
	}

	auto Image::getWidth() const -> u32 {
		return width;
	}

	auto Image::getHeight() const -> u32 {
		return height;
	}

	auto Image::getPixels() const -> u32* {
		return pixels;
	}

	auto Image::write(std::filesystem::path& path) const -> void {
		auto* file = static_cast<FILE*>(nullptr);
		
		_wfopen_s(&file, path.c_str(), L"wb");

		auto* png = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
		auto* info = png_create_info_struct(png);

		png_init_io(png, file);

		png_set_IHDR(
			png,
			info,
			width, height,
			8,
			PNG_COLOR_TYPE_RGBA,
			PNG_INTERLACE_NONE,
			PNG_COMPRESSION_TYPE_DEFAULT,
			PNG_FILTER_TYPE_DEFAULT
		);

		png_write_info(png, info);

		auto* row = new u8[png_get_rowbytes(png, info)];
		
		/* convert image back into bytes */
		for (auto j = 0u; j < height; ++j) {
			for (auto i = 0u; i < width; ++i) {
				const auto pixel = pixels[Util::pos(i, j, width)];

				row[i * 4    ] = Util::red(pixel);
				row[i * 4 + 1] = Util::gre(pixel);
				row[i * 4 + 2] = Util::blu(pixel);
				row[i * 4 + 3] = Util::alp(pixel);
			}

			png_write_row(png, row);
		}
		
		delete[] row;
		
		png_write_end(png, nullptr);

		png_destroy_write_struct(&png, &info);

		fclose(file);
	}

	auto Image::isValid() -> bool {
		return pixels != nullptr;
	}

	auto Image::invalidate() -> void {
		delete[] pixels;
		pixels = nullptr;
	}
}
