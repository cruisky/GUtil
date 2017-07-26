#include "txbase_tests/helper.h"
#include "txbase/math/ray.h"

namespace TX
{
	using namespace Math;
	namespace Tests
	{

		class RayTests: public ::testing::Test
		{
		};

		TEST(RayTests, SetSegment) {
			Ray ray;
			{
				SCOPED_TRACE("Normalized direction");
				ray.SetSegment(Vec3::ZERO, Vec3(1, 0, 0));
				Assertions::Near(
					Ray(Vec3::ZERO, Vec3(1, 0, 0), 1 - Ray::EPSILON),
					ray);
			}
			{
				SCOPED_TRACE("Non-normalized direction");
				ray.SetSegment(Vec3::ZERO, Vec3(5, 0, 0));
				Assertions::Near(
					Ray(Vec3::ZERO, Vec3(1, 0, 0), 5 - Ray::EPSILON),
					ray);
			}
			{
				SCOPED_TRACE("Non-normalized direction, arbitrary origin");
				ray.SetSegment(Vec3(1, 0, 0), Vec3(5, 0, 0));
				Assertions::Near(
					Ray(Vec3(1, 0, 0), Vec3(1, 0, 0), 4 - Ray::EPSILON),
					ray);
			}
			{
				SCOPED_TRACE("Normalized direction, with epsilon");
				ray.SetSegment(Vec3::ZERO, Vec3(1, 0, 0), 0.1f, 0.2f);
				Assertions::Near(
					Ray(Vec3::ZERO, Vec3(1, 0, 0), 0.9f, 0.2f),
					ray);
			}
			{
				SCOPED_TRACE("Non-normalized direction, arbitrary origin, with epsilon");
				ray.SetSegment(Vec3(1, 2, 0), Vec3(5, 2, 0), 0.1f, 0.2f);
				Assertions::Near(
					Ray(Vec3(1, 2, 0), Vec3(1, 0, 0), 3.9f, 0.2f),
					ray);
			}
		}
	}
}