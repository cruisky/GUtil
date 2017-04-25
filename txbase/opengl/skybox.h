#pragma once

#include <memory>
#include "opengl.h"

namespace TX {
	namespace GL {
		class Skybox : public Program {
		private:
			struct Impl;
			const std::unique_ptr<Impl> p;
		public:
			Skybox(const Image faces[6], float distance = 1e3f);
			~Skybox();

			void Draw(const Camera& camera) const;
		};
	}
}