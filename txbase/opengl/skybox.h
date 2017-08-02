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
			Skybox();
			~Skybox();

			Cubemap& GetCubemap();

			void Draw(const Camera& camera) const;
		};
	}
}