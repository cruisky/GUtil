#include "txbase/stdafx.h"
#include "txbase/image/image.h"
#include <stdexcept>

#define STB_IMAGE_IMPLEMENTATION
// #define STBI_ONLY_PNG
// #define STBI_ONLY_BMP
// #define STBI_ONLY_PNM
// #define STBI_ONLY_
#include "txbase/libs/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "txbase/libs/stb_image_write.h"

namespace TX {
	Image Image::Read(const std::string& filename) {
		int width, height, channels;
		float *data = stbi_loadf(filename.c_str(), &width, &height, &channels, 0);

		if (!data){
			throw std::runtime_error(filename + ": read failed.");
		}

		std::printf("image: %s\n", filename.c_str());
		std::printf("data: %p\n", data);
		std::printf("size: %d x %d, channels: %d\n", width, height, channels);

		if (channels > 4) {
			throw std::runtime_error("unsupported number of channels");
		}

		// convert to RGBA format
		const int size = width * height;
		Image result(width, height);
		Color *img = result.Data();

		if (channels <= static_cast<int>(Color::Channel::YA)){
			for (int i = 0, di = 0; i < size; i++, di += channels){
				img[i].r = img[i].g = img[i].b = data[di];
			}
			if (channels == static_cast<int>(Color::Channel::YA)){
				for (int i = 0, di = 1; i < size; i++, di += channels)
					img[i].a = data[di];
			}
		}
		else {
			for (int i = 0, di = 0; i < size; i++, di += channels){
				img[i].r = data[di];
				img[i].g = data[di + 1];
				img[i].b = data[di + 2];
			}
			if (channels == static_cast<int>(Color::Channel::RGBA)){
				for (int i = 0, di = 3; i < size; i++, di += channels)
					img[i].a = data[di];
			}
		}

		stbi_image_free(data);

		return result;
	}

	void Image::Write(const std::string& filename, const Image& image, bool flip_y, Format format, Color::Channel channel){
		// get bytes per pixel
		const int pixel_size = static_cast<int>(channel);
		const int image_size = image.Size();
		const Color *data = image.Data();
		const int width = image.Width();
		const int height = image.Height();

		if (width <= 0 || height <= 0)
			return;

		unsigned char *buffer = new unsigned char[image_size * pixel_size];
		int buffer_i;
		if (channel <= Color::Channel::YA){
			for (int y = 0; y < height; y++){
				buffer_i = ((flip_y ? height - y - 1 : y) * width) * pixel_size;
				for (int x = 0, data_i = y * width; x < width; x++, data_i++){
					buffer[buffer_i++] = Math::Clamp(Math::Round(data[data_i].Luminance() * 255), 0, 255);
					if (channel == Color::Channel::YA)
						buffer[buffer_i++] = Math::Clamp(Math::Round(data[data_i].a * 255), 0, 255);
				}
			}
		}
		else {
			for (int y = 0; y < height; y++){
				buffer_i = ((flip_y ? height - y - 1 : y) * width) * pixel_size;
				for (int x = 0, data_i = y * width; x < width; x++, data_i++){
					buffer[buffer_i++] = Math::Clamp(Math::Round(data[data_i].r * 255), 0, 255);
					buffer[buffer_i++] = Math::Clamp(Math::Round(data[data_i].g * 255), 0, 255);
					buffer[buffer_i++] = Math::Clamp(Math::Round(data[data_i].b * 255), 0, 255);
					if (channel == Color::Channel::RGBA)
						buffer[buffer_i++] = Math::Clamp(Math::Round(data[data_i].a * 255), 0, 255);
				}
			}
		}

		switch (format){
		case PNG:
			stbi_write_png(filename.c_str(), width, height, static_cast<int>(channel), buffer, width * pixel_size);
			break;
		case BMP:
			stbi_write_bmp(filename.c_str(), width, height, static_cast<int>(channel), buffer);
			break;
		}
		delete[] buffer;
	}

	void Image::LoadCheckerboard(int width, int height, int squareSize) {
		const int size = width * height;
		const int dblSquareSize = 2 * squareSize;

		this->data.resize(size);
		this->dimension = Vec2i(width, height);
		Color *image = this->Data();

		for (int y = 0, i = 0; y < height; y++){
			bool flag = y % dblSquareSize < squareSize;
			for (int x = 0; x < width; x++, i++){
				if (flag ^ (x % dblSquareSize < squareSize))
					image[i] = Color::BLACK;
				else
					image[i] = Color::WHITE;
			}
		}
	}

}
