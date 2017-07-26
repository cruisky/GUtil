#include "gtest/gtest.h"
#include "txbase/math/vector.h"

#define FLOAT_ERROR 1e-5f
#define EXPECT_VEC3_EQ(expected, actual) { \
	EXPECT_NEAR(expected.x, actual.x, FLOAT_ERROR);\
	EXPECT_NEAR(expected.y, actual.y, FLOAT_ERROR);\
	EXPECT_NEAR(expected.z, actual.z, FLOAT_ERROR);\
}

namespace TX {
	using namespace Math;
	namespace Tests {
		class Vec3Tests : public ::testing::Test
		{
		};

		TEST(Vec3Tests, Operator_Add) {
			EXPECT_EQ(Vec3(1, 2, 3), Vec3(1, 2, 3) + Vec3(0, 0, 0));
			EXPECT_EQ(Vec3(3, 6, 9), Vec3(1, 2, 3) + Vec3(2, 4, 6));
			EXPECT_EQ(Vec3(-1, -2, -3), Vec3(1, 2, 3) + Vec3(-2, -4, -6));
		}

		TEST(Vec3Tests, Operator_Minus) {
			EXPECT_EQ(Vec3(1, 2, 3), Vec3(1, 2, 3) - Vec3(0, 0, 0));
			EXPECT_EQ(Vec3(-1, -2, -3), Vec3(1, 2, 3) - Vec3(2, 4, 6));
			EXPECT_EQ(Vec3(3, 6, 9), Vec3(1, 2, 3) - Vec3(-2, -4, -6));
		}

		TEST(Vec3Tests, Operator_Multiply) {
			EXPECT_EQ(Vec3(0, 0, 0), Vec3(1, 2, 3) * Vec3(0, 0, 0));
			EXPECT_EQ(Vec3(2, 8, 18), Vec3(1, 2, 3) * Vec3(2, 4, 6));
			EXPECT_EQ(Vec3(-2, -8, -18), Vec3(1, 2, 3) * Vec3(-2, -4, -6));
		}

		TEST(Vec3Tests, Operator_Divide) {
			EXPECT_EQ(Vec3(1, 2, 3), Vec3(1, 2, 3) / Vec3(1, 1, 1));
			EXPECT_EQ(Vec3(.5f, .5f, .5f), Vec3(1, 2, 3) / Vec3(2, 4, 6));
			EXPECT_EQ(Vec3(-.5f, -.5f, -.5f), Vec3(1, 2, 3) / Vec3(-2, -4, -6));
		}

		TEST(Vec3Tests, Normalize) {
			EXPECT_VEC3_EQ(Vec3(1, 0, 0), Normalize(Vec3(5, 0, 0)))

			Vec3 v1(3, 4, 5);
			Vec3 v2 = Normalize(v1);
			EXPECT_EQ(Vec3(3, 4, 5), v1);
			EXPECT_VEC3_EQ(Vec3(0.42426395f, 0.56568527f, 0.70710659f), v2);
			EXPECT_NEAR(1, Length(v2), FLOAT_ERROR);
		}

		TEST(Vec3Tests, Dot) {
			EXPECT_EQ(3, Dot(Vec3(5, 2, 1), Vec3(0, 0.5, 2)));
			EXPECT_EQ(1, Dot(Vec3(0, 1, 2), Vec3(2, 1, 0)));
			EXPECT_EQ(-4, Dot(Vec3(1, 2, -3), Vec3(3, -2, 1)));
			EXPECT_EQ(2, Dot(Vec3(1, 2, -3), Vec3(3, -2, -1)));
		}

		TEST(Vec3Tests, AbsDot) {
			EXPECT_EQ(3, AbsDot(Vec3(5, 2, 1), Vec3(0, 0.5, 2)));
			EXPECT_EQ(1, AbsDot(Vec3(0, 1, 2), Vec3(2, 1, 0)));
			EXPECT_EQ(4, AbsDot(Vec3(1, 2, -3), Vec3(3, -2, 1)));
			EXPECT_EQ(2, AbsDot(Vec3(1, 2, -3), Vec3(3, -2, -1)));
		}

		TEST(Vec3Tests, Cross) {
			EXPECT_EQ(+Vec3::Z, Cross(Vec3::X, Vec3::Y));
			EXPECT_EQ(-Vec3::Y, Cross(Vec3::X, Vec3::Z));
			EXPECT_EQ(-Vec3::Z, Cross(Vec3::Y, Vec3::X));
			EXPECT_EQ(+Vec3::X, Cross(Vec3::Y, Vec3::Z));
			EXPECT_EQ(+Vec3::Y, Cross(Vec3::Z, Vec3::X));
			EXPECT_EQ(-Vec3::X, Cross(Vec3::Z, Vec3::Y));
		}

		TEST(Vec3Tests, Length) {
			EXPECT_EQ(1, Length(Vec3(1, 0, 0)));
			EXPECT_EQ(1, Length(Vec3(0, 1, 0)));
			EXPECT_EQ(1, Length(Vec3(0, 0, 1)));
			EXPECT_FLOAT_EQ(1.7320508f, Length(Vec3(1, 1, 1)));
			EXPECT_FLOAT_EQ(3.7416573f, Length(Vec3(1, 2, 3)));
			EXPECT_FLOAT_EQ(3.7416573f, Length(Vec3(-1, 2, -3)));
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