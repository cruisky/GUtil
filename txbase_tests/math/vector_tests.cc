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