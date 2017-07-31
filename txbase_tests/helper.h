#pragma once

#include <cstdarg>
#include <iostream>
#include <string>
#include "gtest/gtest.h"
#include "txbase/math/vector.h"
#include "txbase/math/matrix.h"
#include "txbase/math/quaternion.h"
#include "txbase/math/color.h"
#include "txbase/sse/sse.h"

namespace TX
{
	namespace Tests
	{
		template <typename T>
		struct TypeName
		{
			static const char* Get()
			{
				return typeid(T).name();
			}
		};

		namespace Assertions {
			static const struct {
				constexpr inline operator float() const { return 1e-3f; }
				constexpr inline operator double() const { return 1e-6; }
			} TOLERANCE;

			template<typename T, size_t N> void VEqual(const T& expected, const T& actual);
			template<typename T, size_t N> void VNear(const T& expected, const T& actual);

			inline void Near(float expected, float actual) {
				EXPECT_NEAR(expected, actual, float(TOLERANCE));
			}
			inline void Near(double expected, double actual) {
				EXPECT_NEAR(expected, actual, double(TOLERANCE));
			}
			template <typename T>
			inline void Equal(const T& expected, const T& actual) {
				SCOPED_TRACE(::testing::Message() << "type: " << TypeName<T>::Get());
				EXPECT_EQ(expected, actual);
			}

			inline void Near(const Vec4& expected, const Vec4& actual) {
				Assertions::VNear<Vec4, 4>(expected, actual);
			}
			inline void Near(const Color& expected, const Color& actual) {
				Assertions::VNear<Color, 3>(expected, actual);
			}
			inline void Near(const Vec3& expected, const Vec3& actual) {
				Assertions::VNear<Vec3, 3>(expected, actual);
			}
			inline void Near(const Ray& expected, const Ray& actual) {
				{
					SCOPED_TRACE("comparing origin");
					Assertions::Near(expected.origin, actual.origin);
				}
				{
					SCOPED_TRACE("comparing dir");
					Assertions::Near(expected.dir, actual.dir);
				}
				{
					SCOPED_TRACE("comparing t_min");
					Assertions::Near(expected.t_min, actual.t_min);
				}
				{
					SCOPED_TRACE("comparing t_max");
					Assertions::Near(expected.t_max, actual.t_max);
				}
			}
			inline void Near(const Matrix4x4& expected, const Matrix4x4& actual){
				Assertions::VNear<Matrix4x4, 4>(expected, actual);
			}
			inline void Near(const Quaternion& expected, const Quaternion& actual) {
#define signof(f) ((f) >= 0 ? 1 : -1)
				// might get a negative version, but they are still equivalent
				bool negate = false;
				for(size_t i = 0; i < 4; i++)
					if (Math::Abs(expected.q[i] - actual.q[i]) > float(TOLERANCE) && signof(expected.w) != signof(actual.w))
						negate = true;
#undef signof
				Assertions::Near(negate ? -expected.q : expected.q, actual.q);
			}

			inline void Equal(const SSE::V4Bool& expected, const SSE::V4Bool& actual) {
				Assertions::VEqual<SSE::V4Bool, 4>(expected, actual);
			}
			inline void Equal(const SSE::V4Int& expected, const SSE::V4Int& actual) {
				Assertions::VEqual<SSE::V4Int, 4>(expected, actual);
			}
			inline void Equal(const SSE::V4Float& expected, const SSE::V4Float& actual) {
				Assertions::VEqual<SSE::V4Float, 4>(expected, actual);
			}
			inline void Near(const SSE::V4Float& expected, const SSE::V4Float& actual) {
				Assertions::VNear<SSE::V4Float, 4>(expected, actual);
			}


			template<typename T, size_t N>
			void VNear(const T& expected, const T& actual) {
				SCOPED_TRACE(::testing::Message() << "comparing with tolerance: " << TypeName<T>::Get() << std::endl
					<< "[Expected] " << std::endl << expected << std::endl
					<< "[Actual] " << std::endl << actual << std::endl);
				for (size_t i = 0; i < N; i++){
					SCOPED_TRACE(::testing::Message() << "at element " << i);
					Near(expected[i], actual[i]);
				}
			}
			template<typename T, size_t N>
			void VEqual(const T& expected, const T& actual) {
				SCOPED_TRACE(::testing::Message() << "comparing: " << TypeName<T>::Get() << std::endl
					<< "[Expected]" << std::endl << expected << std::endl
					<< "[Actual]" << std::endl << actual << std::endl);
				for (size_t i = 0; i < N; i++){
					SCOPED_TRACE(::testing::Message() << "at element " << i);
					Equal(expected[i], actual[i]);
				}
			}
		}
	}
}
