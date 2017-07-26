#include "gtest/gtest.h"
#include "txbase/math/base.h"

namespace TX {
	using namespace Math;
	namespace Tests {
		class MathUtilTests : public ::testing::Test
		{
		};

		TEST(MathUtilTests, RsqrtTest){
			EXPECT_NEAR(0.025532f, Rsqrt(1534.f), 1e-5f);
			EXPECT_NEAR(0.1f, Rsqrt(100.f), 1e-5f);
			EXPECT_NEAR(1.f, Rsqrt(1.f), 1e-5f);
		}
		TEST(MathUtilTests, FloorTest){
			EXPECT_EQ(0, Floor(0.f));
			EXPECT_EQ(1, Floor(1.f));
			EXPECT_EQ(-1, Floor(-1.f));
			EXPECT_EQ(1, Floor(1.2f));
			EXPECT_EQ(1, Floor(1.8f));
			EXPECT_EQ(-1, Floor(-0.2f));
			EXPECT_EQ(-1, Floor(-0.8f));
		}
		TEST(MathUtilTests, CeilTest){
			EXPECT_EQ(0, Ceil(0.f));
			EXPECT_EQ(1, Ceil(1.f));
			EXPECT_EQ(-1, Ceil(-1.f));
			EXPECT_EQ(2, Ceil(1.2f));
			EXPECT_EQ(2, Ceil(1.8f));
			EXPECT_EQ(0, Ceil(-0.2f));
			EXPECT_EQ(0, Ceil(-0.8f));
		}
		TEST(MathUtilTests, RoundTest){
			EXPECT_EQ(0, Round(0.f));
			EXPECT_EQ(1, Round(1.f));
			EXPECT_EQ(-1, Round(-1.f));
			EXPECT_EQ(1, Round(1.2f));
			EXPECT_EQ(2, Round(1.8f));
			EXPECT_EQ(0, Round(-0.2f));
			EXPECT_EQ(-1, Round(-0.8f));
			EXPECT_EQ(0, Round(0.49f));
			EXPECT_EQ(1, Round(0.51f));
		}
	}
}
