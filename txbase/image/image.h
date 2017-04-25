#pragma once

#include "txbase/math/color.h"
#include "txbase/math/vector.h"
#include <string>

namespace TX{
	class Image {
	protected:
		std::vector<Color> data;
		Vec2i dimension;
	public:
		enum Format {
			BMP, PNG
		};
		Image(){}
		Image(int width, int height):
			dimension(width, height),
			data(width * height)
			{}
		Image(const Color *data, int width, int height):
			dimension(width, height),
			data(data, data + width * height)
			{}
		Image(const Image& ot):
			dimension(ot.dimension),
			data(ot.data)
			{}
		Image(const Image&& ot):
			dimension(ot.dimension),
			data(std::move(ot.data))
			{}

		inline Vec2i 		Dimension() const { return dimension; }
		inline int 			Size() const { return data.size(); }
		inline int 			Width() const { return dimension.x; }
		inline int 			Height() const { return dimension.y; }
		inline const Color* Data() const { return data.data(); }
		inline Color* 		Data() { return const_cast<Color *>(static_cast<const Image&>(*this).Data()); }

		void LoadCheckerboard(int width, int height, int squareSize);

		static Image Read(const std::string& filename);

		static void Write(
			const std::string& filename,
			const Image& image,
			bool flip_y = false,
			Format format = BMP,
			Color::Channel channel = Color::Channel::RGB);

	};
}
