#include "txbase_tests/helper.h"
#include "txbase/math/quaternion.h"

namespace TX
{
	using namespace Math;
	namespace Tests
	{
		TEST(QuaternionTests, Norm) {
			Assertions::Near(0, Quaternion(0, 0, 0, 0).Norm());
			Assertions::Near(2, Quaternion(1, 1, 1, 1).Norm());
			Assertions::Near(5.47725f, Quaternion(1, 2, 3, 4).Norm());
		}

		TEST(QuaternionTests, Conjugate) {
			Assertions::Near(Quaternion(0, 0, 0, 0), Quaternion(0, 0, 0, 0).Conjugate());
			Assertions::Near(Quaternion(-1, -1, -1, 1), Quaternion(1, 1, 1, 1).Conjugate());
			Assertions::Near(Quaternion(-1, -2, -3, 4), Quaternion(1, 2, 3, 4).Conjugate());
		}

		TEST(QuaternionTests, Inverse) {
			Assertions::Near(Quaternion(-0.5, -0.5, -0.5, 0.5), Quaternion(1, 1, 1, 1).Inverse());
		}

		TEST(QuaternionTests, Operator_Multiply) {
			Assertions::Near(Quaternion(28, -4.5f, 21.5f, 44),
				Quaternion(0.5f, -3, 4, 1) *
				Quaternion(2, 1, -9, 6));

			Assertions::Near(Quaternion(0.388127f, 0.803418f, -0.433317f, -0.126429f),
				Quaternion(0.18260f, 0.54770f, 0.73030f, 0.36510f) *
				Quaternion(0.9889f, 0.0f, 0.0f, 0.14834f));

			Assertions::Near(Quaternion(0.388127f, -0.64097f, 0.649924f, -0.126429f),
				Quaternion(0.9889f, 0.0f, 0.0f, 0.14834f) *
				Quaternion(0.18260f, 0.54770f, 0.73030f, 0.36510f));
		}

		TEST(QuaternionTests, Rotate) {
			Vec3 inputVec[] = {
				Vec3(1, 1, 1),
				Vec3(1, 2, 3),
				Vec3(1, 0, 0),
				Vec3(1, 0, 0),
				Vec3(0, 1, 0),
				Vec3(0, 0, 1),
				Vec3(1, 0, 0),
				Vec3(0, 1, 0),
				Vec3(0, 0, 1),
				Vec3(1, 0, 0),
				Vec3(0, 1, 0),
				Vec3(0, 0, 1)
			};
			Quaternion inputQuat[] = {
				Quaternion(0, 1, 0, 1),
				Quaternion::IDENTITY,
				Quaternion(0.18260f, 0.54770f, 0.73030f, 0.36510f),
				Quaternion(1, 0, 0, 1),
				Quaternion(1, 0, 0, 1),
				Quaternion(1, 0, 0, 1),
				Quaternion(0, 1, 0, 1),
				Quaternion(0, 1, 0, 1),
				Quaternion(0, 1, 0, 1),
				Quaternion(0, 0, 1, 1),
				Quaternion(0, 0, 1, 1),
				Quaternion(0, 0, 1, 1)
			};
			Vec3 output[] = {
				Vec3(1, 1, -1),
				Vec3(1, 2, 3),
				Vec3(-0.6667f, 0.7333f, -0.1332f),
				Vec3(1, 0, 0),
				Vec3(0, 0, 1),
				Vec3(0, -1, 0),
				Vec3(0, 0, -1),
				Vec3(0, 1, 0),
				Vec3(1, 0, 0),
				Vec3(0, 1, 0),
				Vec3(-1, 0, 0),
				Vec3(0, 0, 1)
			};
			for (size_t i = 0; i < sizeof(output) / sizeof(Vec3); i++) {
				SCOPED_TRACE(::testing::Message() << "Case " << i << std::endl);
				inputQuat[i] = Normalize(inputQuat[i]);
				Assertions::Near(output[i], inputQuat[i].Rotate(inputVec[i]));
			}
		}

		TEST(QuaternionTests, RotationMatrix) {
			{
				SCOPED_TRACE("90 degree around (1,1,1)");
				Assertions::Near(
					Matrix4x4(
						0.333333f, -0.244017f, 0.910682f, 0,
						0.910682f, 0.333333f, -0.244017f, 0,
						-0.244017f, 0.910682f, 0.333333f, 0,
						0, 0, 0, 1),
					Quaternion(0.408248f, 0.408248f, 0.408248f, 0.707106f).RotationMatrix4x4());
			}
			{
				SCOPED_TRACE("90 degree around Z");
				Assertions::Near(
					Matrix4x4(
						0, -1, 0, 0,
						1, 0, 0, 0,
						0, 0, 1, 0,
						0, 0, 0, 1),
					Quaternion(0, 0, 0.70711f, 0.70711f).RotationMatrix4x4());
			}
		}

		TEST(QuaternionTests, EulerAngle) {
			Vec3 eulers[] = {
				Vec3::ZERO,
				Vec3(0, 0, PI / 2),
				Vec3(PI, PI / 6, 0),
				Vec3(0, ToRad(70),ToRad(50)),

			};
			Quaternion quats[] = {
				Quaternion::IDENTITY,
				Quaternion(0, 0, 0.70711f, 0.70711f),
				Quaternion(0.965926f, 0, -0.25882f, 0),
				Quaternion(-0.2424f, 0.519837f, 0.346189f, 0.7424f)
			};

			for (size_t i = 0; i < sizeof(eulers) / sizeof(Vec3); i++) {
				SCOPED_TRACE(::testing::Message() << "Case " << i);
				Assertions::Near(quats[i], Quaternion::Euler(eulers[i]));
				Assertions::Near(eulers[i], quats[i].Euler());
			}
		}

		TEST(QuaternionTests, AngleAxis) {
			{
				SCOPED_TRACE("90 degree around (1,1,1)");
				Assertions::Near(
					Quaternion(0.408248f, 0.408248f, 0.408248f, 0.707106f),
					Quaternion::AngleAxis(Math::PI / 2, Vec3(0.57735f, 0.57735f, 0.57735f)));
			}
			{
				SCOPED_TRACE("90 degree around Z");
				Assertions::Near(
					Quaternion(0, 0, 0.70711f, 0.70711f),
					Quaternion::AngleAxis(Math::PI / 2, Vec3::Z));
			}

		}
		TEST(QuaternionTests, FromToRotationTest) {
			Assertions::Near(
				Quaternion(0, 0, 0.707107f, 0.707107f),
				Quaternion::FromToRotation(Vec3(1, 0, 0), Vec3(0, 1, 0)));
			Assertions::Near(
				Quaternion(0, -0.707107f, 0, 0.707107f),
				Quaternion::FromToRotation(Vec3(1, 0, 0), Vec3(0, 0, 1)));
			Assertions::Near(
				Quaternion(0.707107f, 0, 0, 0.707107f),
				Quaternion::FromToRotation(Vec3(0, 1, 0), Vec3(0, 0, 1)));
			Assertions::Near(
				Quaternion(0, -0.325057f, 0.325057f, 0.888073f),
				Quaternion::FromToRotation(Vec3(1, 0, 0), Vec3(0.57735f, 0.57735f, 0.57735f)));
			Assertions::Near(
				Quaternion(0, -0.408246f, 0.408246f, 0.816493f),
				Quaternion::FromToRotation(Vec3(0.57735f, 0.57735f, 0.57735f), Vec3(-0.57735f, 0.57735f, 0.57735f)));
		}

		TEST(QuaternionTests, LookRotation) {
			Assertions::Near(
				Quaternion::IDENTITY,
				Quaternion::LookRotation(Vec3::FORWARD));
			Assertions::Near(
				Quaternion(0, 0, 1, 0),
				Quaternion::LookRotation(Vec3::FORWARD, Vec3::DOWN));
			Assertions::Near(
				Quaternion(0.382682f, 0, 0, 0.923878f),
				Quaternion::LookRotation(Vec3(0, 0.707107f, -0.707107f)));
			Assertions::Near(
				Quaternion(0, -0.382682f, 0, 0.923878f),
				Quaternion::LookRotation(Vec3(0.707107f, 0, -0.707107f)));
			Assertions::Near(
				Quaternion(0.279846f, -0.364704f, 0.115916f, 0.880474f),
				Quaternion::LookRotation(Vec3(0.57735f, 0.57735f, -0.57735f)));
		}
	}
}
