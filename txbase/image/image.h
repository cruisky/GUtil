#pragma once

#include "txbase/math/color.h"
#include <string>

namespace TX{
	class Image {
	public:
		enum Format {
			BMP, PNG
		};

		static Color *Read(const std::string& filename, int *width, int *height);

		static void Write(
			const std::string& filename,
			const Color *data,
			int width,
			int height,
			bool flip_y = false,
			Format format = BMP,
			Color::Channel channel = Color::Channel::RGB);

		static Color *LoadCheckerboard(int width, int height, int squareSize);
	};
}
