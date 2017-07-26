// NOTE: Do not include this header in math/*.h

#pragma once
#pragma warning(disable: 4800)
#pragma warning(disable: 4005)

#include <cassert>
#include <memory>
#include <vector>
#include <cstring>
#include <iostream>
#include <cstdint>

#include "txbase/math/base.h"
#include "txbase/math/bbox.h"
#include "txbase/math/color.h"
#include "txbase/math/geometry.h"
#include "txbase/math/matrix.h"
#include "txbase/math/quaternion.h"
#include "txbase/math/random.h"
#include "txbase/math/ray.h"
#include "txbase/math/sample.h"
#include "txbase/math/transform.h"
#include "txbase/math/vector.h"

namespace TX
{
	class Filter;
	class Image;
	class Film;

	class Camera;
	class Shape; class Mesh;
	class ObjMaterial; class ObjMesh; class ObjShape;
	class FontMap;

	// SSE
	namespace SSE{
		struct V4Bool;
		struct V4Float;
		struct V4Int;
	}

	// GUI
	namespace UI{
		class Application;
		struct Input;
	}
	class Timer;
}
