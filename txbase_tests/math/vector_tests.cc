#include "txbase_tests/helper.h"
#include "txbase/math/vector.h"

namespace TX {
	using namespace Math;
	namespace Tests {
		TEST(Vec3Tests, Operator_Add) {
			Assertions::Near(Vec3(1, 2, 3), Vec3(1, 2, 3) + Vec3(0, 0, 0));
			Assertions::Near(Vec3(3, 6, 9), Vec3(1, 2, 3) + Vec3(2, 4, 6));
			Assertions::Near(Vec3(-1, -2, -3), Vec3(1, 2, 3) + Vec3(-2, -4, -6));
		}

		TEST(Vec3Tests, Operator_Minus) {
			Assertions::Near(Vec3(1, 2, 3), Vec3(1, 2, 3) - Vec3(0, 0, 0));
			Assertions::Near(Vec3(-1, -2, -3), Vec3(1, 2, 3) - Vec3(2, 4, 6));
			Assertions::Near(Vec3(3, 6, 9), Vec3(1, 2, 3) - Vec3(-2, -4, -6));
		}

		TEST(Vec3Tests, Operator_Multiply) {
			Assertions::Near(Vec3(0, 0, 0), Vec3(1, 2, 3) * Vec3(0, 0, 0));
			Assertions::Near(Vec3(2, 8, 18), Vec3(1, 2, 3) * Vec3(2, 4, 6));
			Assertions::Near(Vec3(-2, -8, -18), Vec3(1, 2, 3) * Vec3(-2, -4, -6));
		}

		TEST(Vec3Tests, Operator_Divide) {
			Assertions::Near(Vec3(1, 2, 3), Vec3(1, 2, 3) / Vec3(1, 1, 1));
			Assertions::Near(Vec3(.5f, .5f, .5f), Vec3(1, 2, 3) / Vec3(2, 4, 6));
			Assertions::Near(Vec3(-.5f, -.5f, -.5f), Vec3(1, 2, 3) / Vec3(-2, -4, -6));
		}

		TEST(Vec3Tests, Normalize) {
			Assertions::Near(Vec3(1, 0, 0), Normalize(Vec3(5, 0, 0)));

			Vec3 v1(3, 4, 5);
			Vec3 v2 = Normalize(v1);
			Assertions::Equal(Vec3(3, 4, 5), v1);
			Assertions::Near(Vec3(0.42426395f, 0.56568527f, 0.70710659f), v2);
			Assertions::Near(1, Length(v2));
		}

		TEST(Vec3Tests, Dot) {
			Assertions::Near(3, Dot(Vec3(5, 2, 1), Vec3(0, 0.5, 2)));
			Assertions::Near(1, Dot(Vec3(0, 1, 2), Vec3(2, 1, 0)));
			Assertions::Near(-4, Dot(Vec3(1, 2, -3), Vec3(3, -2, 1)));
			Assertions::Near(2, Dot(Vec3(1, 2, -3), Vec3(3, -2, -1)));
		}

		TEST(Vec3Tests, AbsDot) {
			Assertions::Near(3, AbsDot(Vec3(5, 2, 1), Vec3(0, 0.5, 2)));
			Assertions::Near(1, AbsDot(Vec3(0, 1, 2), Vec3(2, 1, 0)));
			Assertions::Near(4, AbsDot(Vec3(1, 2, -3), Vec3(3, -2, 1)));
			Assertions::Near(2, AbsDot(Vec3(1, 2, -3), Vec3(3, -2, -1)));
		}

		TEST(Vec3Tests, Cross) {
			Assertions::Near(+Vec3::Z, Cross(Vec3::X, Vec3::Y));
			Assertions::Near(-Vec3::Y, Cross(Vec3::X, Vec3::Z));
			Assertions::Near(-Vec3::Z, Cross(Vec3::Y, Vec3::X));
			Assertions::Near(+Vec3::X, Cross(Vec3::Y, Vec3::Z));
			Assertions::Near(+Vec3::Y, Cross(Vec3::Z, Vec3::X));
			Assertions::Near(-Vec3::X, Cross(Vec3::Z, Vec3::Y));
		}

		TEST(Vec3Tests, Length) {
			Assertions::Near(1, Length(Vec3(1, 0, 0)));
			Assertions::Near(1, Length(Vec3(0, 1, 0)));
			Assertions::Near(1, Length(Vec3(0, 0, 1)));
			Assertions::Near(1.7320508f, Length(Vec3(1, 1, 1)));
			Assertions::Near(3.7416573f, Length(Vec3(1, 2, 3)));
			Assertions::Near(3.7416573f, Length(Vec3(-1, 2, -3)));
		}

		TEST(Vec3Tests, Lerp) {
			Vec3 a(0, 0, 0), b(10, 20, 30);
			Assertions::Near(a, Lerp(0, a, b));
			Assertions::Near(Vec3(1, 2, 3), Lerp(0.1f, a, b));
			Assertions::Near(Vec3(5, 10, 15), Lerp(0.5f, a, b));
			Assertions::Near(Vec3(9, 18, 27), Lerp(0.9f, a, b));
			Assertions::Near(b, Lerp(1, a, b));
		}

		TEST(Vec3Tests, Slerp) {
			{
				SCOPED_TRACE("");
				Vec3 a(10, 0, 0), b(0, 20, 0);
				Assertions::Near(a, Slerp(0, a, b));
				Assertions::Near(Vec3(10.8645f, 1.72077f, 0), Slerp(0.1f, a, b));
				Assertions::Near(Vec3(10.6066f, 10.6065f, 0), Slerp(0.5f, a, b));
				Assertions::Near(Vec3(2.97225f, 18.7659f, 0), Slerp(0.9f, a, b));
				Assertions::Near(b, Slerp(1, a, b));
			}
			{
				SCOPED_TRACE("");
				Vec3 a(0, 10, 0), b(0, 0, 10);
				Assertions::Near(a, Slerp(0, a, b));
				Assertions::Near(Vec3(0, 9.87688f, 1.56433f), Slerp(0.1f, a, b));
				Assertions::Near(Vec3(0, 7.07106f, 7.07103f), Slerp(0.5f, a, b));
				Assertions::Near(Vec3(0, 1.56434f, 9.87684f), Slerp(0.9f, a, b));
				Assertions::Near(b, Slerp(1, a, b));
			}
		}

		class Vec4Tests : public ::testing::Test
		{
		};
		TEST(Vec4Tests, Operator_ArrayAccess) {
			Vec4 v(1, 2, 3, 4);
			EXPECT_EQ(1, v[0]);
			EXPECT_EQ(2, v[1]);
			EXPECT_EQ(3, v[2]);
			EXPECT_EQ(4, v[3]);
		}
	}
}