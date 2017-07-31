#include "txbase_tests/helper.h"
#include "txbase/math/matrix.h"

namespace TX
{
	namespace Tests
	{
		TEST(Matrix4x4Tests, Operator_Multiply) {
			Matrix4x4 a(
				1, 2, 3, 4,
				2, 1, 2, 3,
				3, 2, 1, 2,
				4, 3, 2, 1);
			Matrix4x4 b(
				1, 0, 1, 0,
				0, 1, 0, 1,
				-1, 0, -1, 0,
				0, -1, 0, -1);
			Matrix4x4 expected(
				-2, -2, -2, -2,
				0, -2, 0, -2,
				2, 0, 2, 0,
				2, 2, 2, 2);
			Assertions::Near(expected, a * b);
		}
		TEST(Matrix4x4Tests, Inverse) {
			Matrix4x4 example(
				1, 2, 3, 4,
				1e-4f, 1, 2, 3,
				1e-4f, 1e-4f, 1, 2,
				1e-4f, 1e-4f, 1e-4f, 1);
			Matrix4x4 inverse(
				1, -2, 1, 0,
				0, 1, -2, 1,
				0, 0, 1, -2,
				0, 0, 0, 1);
			Matrix4x4 actual = example.Inverse();
			Assertions::Near(inverse, actual);
		}

		TEST(Matrix4x4Tests, Transpose) {
			Matrix4x4 example(
				1, 2, 3, 4,
				4, 3, 2, 1,
				1, 2, 3, 4,
				4, 3, 2, 1);
			Matrix4x4 transpose(
				1, 4, 1, 4,
				2, 3, 2, 3,
				3, 2, 3, 2,
				4, 1, 4, 1);
			Matrix4x4 actual = example.Transpose();
			Assertions::Near(transpose, actual);
		}

		TEST(Matrix4x4Tests, Translate) {
			Matrix4x4 translation(
				1, 0, 0, 1,
				0, 1, 0, 2,
				0, 0, 1, 3,
				0, 0, 0, 1);
			Matrix4x4 actual = Matrix4x4::Translate(Vec3(1, 2, 3));
			Assertions::Near(translation, actual);
		}

		TEST(Matrix4x4Tests, Rotate_Euler) {
			{
				SCOPED_TRACE("Identity");
				Assertions::Near(
					Matrix4x4::IDENTITY,
					Matrix4x4::Rotate(0, 0, Math::PI * 2));
			}
			{
				SCOPED_TRACE("Around X axis");
				Assertions::Near(
					Matrix4x4(
						1, 0, 0, 0,
						0, 0, -1, 0,
						0, 1, 0, 0,
						0, 0, 0, 1),
					Matrix4x4::Rotate(Math::PI / 2, 0, 0));
			}
			{
				SCOPED_TRACE("Around Y axis");
				Assertions::Near(
					Matrix4x4(
						0, 0, 1, 0,
						0, 1, 0, 0,
						-1, 0, 0, 0,
						0, 0, 0, 1),
					Matrix4x4::Rotate(0, Math::PI / 2, 0));
			}
			{
				SCOPED_TRACE("Around Z axis");
				Assertions::Near(
					Matrix4x4(
						0, -1, 0, 0,
						1, 0, 0, 0,
						0, 0, 1, 0,
						0, 0, 0, 1),
					Matrix4x4::Rotate(0, 0, Math::PI / 2));
			}
			{
				SCOPED_TRACE("Around XYZ axis");
				Assertions::Near(
					Matrix4x4(
						0, 0, 1, 0,
						0, 1, 0, 0,
						-1, 0, 0, 0,
						0, 0, 0, 1),
					Matrix4x4::Rotate(Math::PI / 2, Math::PI / 2, Math::PI / 2));
			}
		}

		TEST(Matrix4x4Tests, Rotate_Axis) {
			{
				SCOPED_TRACE("Identity");
				Assertions::Near(
					Matrix4x4::IDENTITY,
					Matrix4x4::Rotate(Math::PI * 2, Vec3::X));
			}
			{
				SCOPED_TRACE("Around X axis");
				Assertions::Near(
					Matrix4x4(
						1, 0, 0, 0,
						0, 0, -1, 0,
						0, 1, 0, 0,
						0, 0, 0, 1),
					Matrix4x4::Rotate(Math::PI / 2, Vec3::X));
			}
			{
				SCOPED_TRACE("Around arbitrary axis");
				Assertions::Near(
					Matrix4x4(
						0.333333f, -0.244017f, 0.910682f, 0,
						0.910682f, 0.333333f, -0.244017f, 0,
						-0.244017f, 0.910682f, 0.333333f, 0,
						0, 0, 0, 1),
					Matrix4x4::Rotate(Math::PI / 2, Vec3(1, 1, 1)));
			}
		}

		TEST(Matrix4x4Tests, Scale) {
			{
				SCOPED_TRACE("Identity");
				Assertions::Near(
					Matrix4x4::IDENTITY,
					Matrix4x4::Scale(1, 1, 1));
			}
			{
				SCOPED_TRACE("By 0");
				Assertions::Near(
					Matrix4x4(
						0, 0, 0, 0,
						0, 0, 0, 0,
						0, 0, 0, 0,
						0, 0, 0, 1),
					Matrix4x4::Scale(0, 0, 0));
			}
			{
				SCOPED_TRACE("By arbitrary factor");
				Assertions::Near(
					Matrix4x4(
						1, 0, 0, 0,
						0, 2, 0, 0,
						0, 0, 3, 0,
						0, 0, 0, 1),
					Matrix4x4::Scale(1, 2, 3));
			}
		}

		TEST(Matrix4x4Tests, Transformation) {
			Matrix4x4 translation(
				1, 0, 0, 1,
				0, 1, 0, 2,
				0, 0, 1, 3,
				0, 0, 0, 1);
			Matrix4x4 translation_inv(
				1, 0, 0, -1,
				0, 1, 0, -2,
				0, 0, 1, -3,
				0, 0, 0, 1);
			Matrix4x4 rotation(
				1, 0, 0, 0,
				0, 0, -1, 0,
				0, 1, 0, 0,
				0, 0, 0, 1);
			Matrix4x4 rotation_inv(
				1, 0, 0, 0,
				0, 0, 1, 0,
				0, -1, 0, 0,
				0, 0, 0, 1);
			Matrix4x4 scaling(
				1, 0, 0, 0,
				0, 2, 0, 0,
				0, 0, 4, 0,
				0, 0, 0, 1);
			Matrix4x4 scaling_inv(
				1, 0, 0, 0,
				0, 0.5f, 0, 0,
				0, 0, 0.25f, 0,
				0, 0, 0, 1);
			{
				SCOPED_TRACE("Translate point");
				Assertions::Near(Vec3(4, 8, 12), Matrix4x4::TPoint(translation, Vec3(3, 6, 9)));
			}
			{
				SCOPED_TRACE("Translate vector");
				Assertions::Near(Vec3(3, 6, 9), Matrix4x4::TVec(translation, Vec3(3, 6, 9)));
			}
			{
				SCOPED_TRACE("Translate normal");
				Assertions::Near(Vec3(3, 6, 9), Matrix4x4::TNormal(translation_inv, Vec3(3, 6, 9)));
			}
			{
				SCOPED_TRACE("Rotate point");
				Assertions::Near(Vec3(3, -9, 6), Matrix4x4::TPoint(rotation, Vec3(3, 6, 9)));
			}
			{
				SCOPED_TRACE("Rotate vector");
				Assertions::Near(Vec3(3, -9, 6), Matrix4x4::TVec(rotation, Vec3(3, 6, 9)));
			}
			{
				SCOPED_TRACE("Rotate normal");
				Assertions::Near(Vec3(3, -9, 6), Matrix4x4::TNormal(rotation_inv, Vec3(3, 6, 9)));
			}
			{
				SCOPED_TRACE("Scale point");
				Assertions::Near(Vec3(3, 12, 36), Matrix4x4::TPoint(scaling, Vec3(3, 6, 9)));
			}
			{
				SCOPED_TRACE("Scale vector");
				Assertions::Near(Vec3(3, 12, 36), Matrix4x4::TVec(scaling, Vec3(3, 6, 9)));
			}
			{
				SCOPED_TRACE("Scale normal");
				Assertions::Near(Vec3(3, 3, 2.25), Matrix4x4::TNormal(scaling_inv, Vec3(3, 6, 9)));
			}
		}
	}
}