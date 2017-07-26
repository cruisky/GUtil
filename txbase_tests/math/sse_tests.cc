#include "txbase_tests/helper.h"
#include "txbase/math/quaternion.h"

namespace TX
{
	using namespace SSE;
	namespace Tests
	{
		TEST(V4FloatTests, Constructor) {
			V4Float f4(1, 2, 1, 2);
			Assertions::Near(1, f4[0]);
			Assertions::Near(2, f4[1]);
			Assertions::Near(1, f4[2]);
			Assertions::Near(2, f4[3]);
			Assertions::Near(V4Float(_mm_setzero_ps()), V4Float());
			Assertions::Near(V4Float(1), V4Float(1, 1, 1, 1));
			Assertions::Near(V4Float(1, 2), f4);
			Assertions::Near(V4Float(f4), f4);
			EXPECT_TRUE(All(Shuffle<3, 2, 1, 0>(f4) == _mm_set_ps(1, 2, 1, 2)));
		}

		TEST(V4FloatTests, OperatorEquality) {
			EXPECT_TRUE(Any(V4Float(1, 2, 3, 5) != V4Float(1, 2, 3, 4)));
			EXPECT_TRUE(All(V4Float(_mm_set_ps(4, 3, 2, 1)) == V4Float(1, 2, 3, 4)));
			EXPECT_TRUE(All(V4Float(32) == 32));
			EXPECT_TRUE(All(V4Float(100) != 0));
		}

		TEST(V4FloatTests, Min) {
			Assertions::Equal(V4Float(1, 2, 2, 1), Min(V4Float(1, 2, 3, 4), V4Float(4, 3, 2, 1)));
			Assertions::Equal(V4Float(-4, -3, 2, 1), Min(V4Float(-1, -2, 3, 4), V4Float(-4, -3, 2, 1)));
		}

		TEST(V4FloatTests, Max) {
			Assertions::Equal(V4Float(4, 3, 3, 4), Max(V4Float(1, 2, 3, 4), V4Float(4, 3, 2, 1)));
			Assertions::Equal(V4Float(-1, -2, 3, 4), Max(V4Float(-1, -2, 3, 4), V4Float(-4, -3, 2, 1)));
		}

		TEST(V4FloatTests, Operator_Unary) {
			Assertions::Equal(V4Float(1, 2, 3, 4), +V4Float(1, 2, 3, 4));
			Assertions::Equal(V4Float(-1, -2, -3, -4), -V4Float(1, 2, 3, 4));
		}

		TEST(V4FloatTests, Operator_Add) {
			V4Float a = V4Float(2, 4, 6, 8);
			V4Float b = V4Float(-1, -2, -3, -4);
			V4Float c = V4Float(1, 2, 3, 4);
			Assertions::Near(c, a + b);

			Assertions::Equal(V4Float(2, 4, 6, 8), a);
			a += b;
			Assertions::Near(V4Float(1, 2, 3, 4), a);
		}

		TEST(V4FloatTests, Operator_Subtract) {
			V4Float a = V4Float(2, 4, 6, 8);
			V4Float b = V4Float(-1, -2, -3, -4);
			V4Float c = V4Float(3, 6, 9, 12);
			Assertions::Near(c, a - b);

			Assertions::Equal(V4Float(2, 4, 6, 8), a);
			a -= b;
			Assertions::Near(c, a);
		}

		TEST(V4FloatTests, Operator_Multiply) {
			V4Float a = V4Float(2, 4, 6, 8);
			V4Float b = V4Float(-1, -2, -3, -4);
			V4Float c = V4Float(-2, -8, -18, -32);
			Assertions::Near(c, a * b);

			Assertions::Equal(V4Float(2, 4, 6, 8), a);
			a *= b;
			Assertions::Near(c, a);
		}

		TEST(V4FloatTests, Operator_Divide) {
			V4Float a = V4Float(2, 4, 6, 8);
			V4Float b = V4Float(-1, -2, -3, -4);
			V4Float c = V4Float(-2, -2, -2, -2);
			Assertions::Near(V4Float(-2, -2, -2, -2), a / b);

			Assertions::Equal(V4Float(2, 4, 6, 8), a);
			a /= b;
			Assertions::Near(c, a);
		}

		TEST(V4FloatTests, Math_Abs) {
			Assertions::Equal(V4Float(1, 2, 3, 4), Abs(V4Float(-1, -2, 3, 4)));
			Assertions::Equal(V4Float(1, 2, 3, 4), Abs(V4Float(1, 2, -3, -4)));
		}

		TEST(V4FloatTests, Math_Exp) {
			Assertions::Near(V4Float(0.367879f, 1, 2.71828f, 7.38905f), Exp(V4Float(-1, 0, 1, 2)));
		}

		TEST(V4FloatTests, Math_Log) {
			Assertions::Near(V4Float(0, 1, 1.09861f, 1.38629f), Log(V4Float(1, 2.71828f, 3, 4)));
		}

		TEST(V4FloatTests, Math_Log2) {
			Assertions::Near(V4Float(0, 1, 1.58496f, 2), Log2(V4Float(1, 2, 3, 4)));
		}

		TEST(V4FloatTests, Math_Log10) {
			Assertions::Near(V4Float(0, 0.30103f, 0.477121f, 0.602060f), Log10(V4Float(1, 2, 3, 4)));
		}

		TEST(V4FloatTests, Math_ToRad) {
			V4Float rad(0, 1.570796f, 3.141592f, 4.188790f);
			V4Float deg(0, 90, 180, 240);
			Assertions::Near(rad, ToRad(V4Float(deg)));
		}

		TEST(V4FloatTests, Math_ToDeg) {
			V4Float rad(0, 1.570796f, 3.141592f, 4.188790f);
			V4Float deg(0, 90, 180, 240);
			Assertions::Near(deg, ToDeg(V4Float(rad)));
		}

		TEST(V4FloatTests, Math_Sin) {
			V4Float rad(0, 1.570796f, 3.141592f, 4.188790f);
			Assertions::Near(V4Float(0, 1, 0, -0.866025f), Sin(rad));
		}

		TEST(V4FloatTests, Math_Cos) {
			V4Float rad(0, 1.570796f, 3.141592f, 4.188790f);
			Assertions::Near(V4Float(1, 0, -1, -0.5f), Cos(rad));
		}

		TEST(V4FloatTests, Math_Tan) {
			V4Float rad(0, 1.308997f, 3.141592f, 4.188790f);
			Assertions::Near(V4Float(0, 3.731643f, 0, 1.731627f), Tan(rad));
		}

		TEST(V4FloatTests, Math_Floor) {
			Assertions::Equal(V4Float(+0, +0, +0, +1), Floor(V4Float(+0, +0.1f, +0.9f, +1)));
			Assertions::Equal(V4Float(-0, -1, -1, -1), Floor(V4Float(-0, -0.1f, -0.9f, -1)));
		}

		TEST(V4FloatTests, Math_Ceil) {
			Assertions::Equal(V4Float(+0, +1, +1, +1), Ceil(V4Float(+0, +0.1f, +0.9f, +1)));
			Assertions::Equal(V4Float(-0, -0, -0, -1), Ceil(V4Float(-0, -0.1f, -0.9f, -1)));
		}

		TEST(V4FloatTests, Math_Round) {
			Assertions::Equal(V4Float(+0, +0, +1, +1), Round(V4Float(+0, +0.1f, +0.9f, +1)));
			Assertions::Equal(V4Float(-0, -0, -1, -1), Round(V4Float(-0, -0.1f, -0.9f, -1)));
		}

		TEST(V4FloatTests, SelectMin) {
			EXPECT_EQ(0, V4Float(0, 0, 0, 0)[SelectMin(V4Float(0, 0, 0, 0))]);
			EXPECT_EQ(0, SelectMin(V4Float(1, 2, 3, 4)));
			EXPECT_EQ(3, SelectMin(V4Float(4, 3, 2, 1)));
			EXPECT_EQ(0, SelectMin(V4Float(-1, 0, 1, 2)));
		}

		TEST(V4FloatTests, SelectMax) {
			EXPECT_EQ(0, V4Float(0, 0, 0, 0)[SelectMax(V4Float(0, 0, 0, 0))]);
			EXPECT_EQ(3, SelectMax(V4Float(1, 2, 3, 4)));
			EXPECT_EQ(1, SelectMax(V4Float(-1, 2, 0, 1)));
		}
	}
}
