#include "txbase_tests/helper.h"
#include "txbase/math/base.h"

namespace TX {
	using namespace Math;
	namespace Tests {
		TEST(MathUtilTests, RsqrtTest){
			Assertions::Near(0.025532f, Rsqrt(1534.f));
			Assertions::Near(0.1f, Rsqrt(100.f));
			Assertions::Near(1.f, Rsqrt(1.f));
		}
		TEST(MathUtilTests, FloorTest){
			Assertions::Equal(0, Floor(0.f));
			Assertions::Equal(1, Floor(1.f));
			Assertions::Equal(-1, Floor(-1.f));
			Assertions::Equal(1, Floor(1.2f));
			Assertions::Equal(1, Floor(1.8f));
			Assertions::Equal(-1, Floor(-0.2f));
			Assertions::Equal(-1, Floor(-0.8f));
		}
		TEST(MathUtilTests, CeilTest){
			Assertions::Equal(0, Ceil(0.f));
			Assertions::Equal(1, Ceil(1.f));
			Assertions::Equal(-1, Ceil(-1.f));
			Assertions::Equal(2, Ceil(1.2f));
			Assertions::Equal(2, Ceil(1.8f));
			Assertions::Equal(0, Ceil(-0.2f));
			Assertions::Equal(0, Ceil(-0.8f));
		}
		TEST(MathUtilTests, RoundTest){
			Assertions::Equal(0, Round(0.f));
			Assertions::Equal(1, Round(1.f));
			Assertions::Equal(-1, Round(-1.f));
			Assertions::Equal(1, Round(1.2f));
			Assertions::Equal(2, Round(1.8f));
			Assertions::Equal(0, Round(-0.2f));
			Assertions::Equal(-1, Round(-0.8f));
			Assertions::Equal(0, Round(0.49f));
			Assertions::Equal(1, Round(0.51f));
		}
	}
}
