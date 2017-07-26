#pragma once

#include <iostream>
#include "txbase/math/base.h"
#include "txbase/math/vector.h"

namespace TX{
	class Matrix3x3;
	class Matrix4x4;

	class Matrix3x3 {
	public:
		static const Matrix3x3 IDENTITY;
	private:
		Vec3 row[3];
	public:
		Matrix3x3(){
			row[0] = Vec3::X;
			row[1] = Vec3::Y;
			row[2] = Vec3::Z;
		}
		Matrix3x3(const Vec3& r0, const Vec3& r1, const Vec3& r2){
			row[0] = r0;
			row[1] = r1;
			row[2] = r2;
		}
		Matrix3x3(const Matrix3x3& ot) : Matrix3x3(ot[0], ot[1], ot[2]){}
		Matrix3x3(const Matrix4x4& ot);
		Matrix3x3(
			float m00, float m01, float m02,
			float m10, float m11, float m12,
			float m20, float m21, float m22)
			: Matrix3x3(
			Vec3(m00, m01, m02),
			Vec3(m10, m11, m12),
			Vec3(m20, m21, m22)){}
		~Matrix3x3(){}

		inline Matrix3x3& operator = (const Matrix3x3& ot){
			row[0] = ot[0];
			row[1] = ot[1];
			row[2] = ot[2];
			return *this;
		}

		inline Matrix3x3 operator + (const Matrix3x3& ot) const{
			return Matrix3x3(
				row[0] + ot[0],
				row[1] + ot[1],
				row[2] + ot[2]);
		}
		inline Matrix3x3 operator - (const Matrix3x3& ot) const{
			return Matrix3x3(
				row[0] - ot[0],
				row[1] - ot[1],
				row[2] - ot[2]);
		}
		inline Matrix3x3 operator * (const Matrix3x3& ot) const {
			Matrix3x3 result;
			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++)
					result[i][j] =
					row[i][0] * ot[0][j] +
					row[i][1] * ot[1][j] +
					row[i][2] * ot[2][j];
			return result;
		}
		inline const Matrix3x3& operator += (const Matrix3x3& ot){
			row[0] += ot[0];
			row[1] += ot[1];
			row[2] += ot[2];
			return *this;
		}
		inline const Matrix3x3& operator -= (const Matrix3x3& ot){
			row[0] -= ot[0];
			row[1] -= ot[1];
			row[2] -= ot[2];
			return *this;
		}
		inline const Matrix3x3& operator *= (const Matrix3x3& ot) {
			Vec3 temp;
			for (int i = 0; i < 3; i++){
				for (int j = 0; j < 3; j++)
					temp[j] =
					row[i][0] * ot[0][j] +
					row[i][1] * ot[1][j] +
					row[i][2] * ot[2][j];
				row[i] = temp;
			}
			return *this;
		}

		inline const Vec3& operator[](int rowi) const { return row[rowi]; }
		inline Vec3& operator[](int rowi){ return row[rowi]; }
		inline operator float*(){ return &row[0][0]; }
		inline operator const float*() const { return &row[0][0]; }

		inline Matrix3x3 Transpose() const{
			return Matrix3x3(
				row[0][0], row[1][0], row[2][0],
				row[0][1], row[1][1], row[2][1],
				row[0][2], row[1][2], row[2][2]);
		}

		inline Matrix3x3 Inverse() const {
			//TODO
			return Matrix3x3();
		}
	};

	inline std::ostream& operator << (std::ostream& os, const Matrix3x3& m){
		for (int i = 0; i < 3; i++){
			for (int j = 0; j < 3; j++)
				os << m[i][j] << "\t";
			os << std::endl;
		}
		return os;
	}


	/// <summary>
	/// 4x4 row-major matrix using right-handed coordinate system.
	/// </summary>
	class Matrix4x4 {
	public:
		static const Matrix4x4 IDENTITY;
	private:
		Vec4 row[4];
	public:
		Matrix4x4(){
			row[0] = Vec4::X;
			row[1] = Vec4::Y;
			row[2] = Vec4::Z;
			row[3] = Vec4::W;
		}
		Matrix4x4(const Vec4& r0, const Vec4& r1, const Vec4& r2, const Vec4& r3){
			row[0] = r0;
			row[1] = r1;
			row[2] = r2;
			row[3] = r3;
		}
		Matrix4x4(const Matrix4x4& ot) : Matrix4x4(ot[0], ot[1], ot[2], ot[3]){}
		Matrix4x4(const Matrix3x3& ot) :
			Matrix4x4(
				Vec4(ot[0], 0.f),
				Vec4(ot[1], 0.f),
				Vec4(ot[2], 0.f),
				Vec4::W)
			{}
		Matrix4x4(
			float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33)
			: Matrix4x4(
			Vec4(m00, m01, m02, m03),
			Vec4(m10, m11, m12, m13),
			Vec4(m20, m21, m22, m23),
			Vec4(m30, m31, m32, m33)){}
		~Matrix4x4(){}

		inline Matrix4x4& operator = (const Matrix4x4& ot){
			row[0] = ot[0];
			row[1] = ot[1];
			row[2] = ot[2];
			row[3] = ot[3];
			return *this;
		}

		inline Matrix4x4 operator + (const Matrix4x4& ot) const{
			return Matrix4x4(
				row[0] + ot[0],
				row[1] + ot[1],
				row[2] + ot[2],
				row[3] + ot[3]);
		}
		inline Matrix4x4 operator - (const Matrix4x4& ot) const{
			return Matrix4x4(
				row[0] - ot[0],
				row[1] - ot[1],
				row[2] - ot[2],
				row[3] - ot[3]);
		}
		inline Matrix4x4 operator * (const Matrix4x4& ot) const {
			Matrix4x4 result;
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
					result[i][j] =
					row[i][0] * ot[0][j] +
					row[i][1] * ot[1][j] +
					row[i][2] * ot[2][j] +
					row[i][3] * ot[3][j];
			return result;
		}
		inline const Matrix4x4& operator += (const Matrix4x4& ot){
			row[0] += ot[0];
			row[1] += ot[1];
			row[2] += ot[2];
			row[3] += ot[3];
			return *this;
		}
		inline const Matrix4x4& operator -= (const Matrix4x4& ot){
			row[0] -= ot[0];
			row[1] -= ot[1];
			row[2] -= ot[2];
			row[3] -= ot[3];
			return *this;
		}
		inline const Matrix4x4& operator *= (const Matrix4x4& ot) {
			Vec4 temp;
			for (int i = 0; i < 4; i++){
				for (int j = 0; j < 4; j++)
					temp[j] =
					row[i][0] * ot[0][j] +
					row[i][1] * ot[1][j] +
					row[i][2] * ot[2][j] +
					row[i][3] * ot[3][j];
				row[i] = temp;
			}
			return *this;
		}

		inline const Vec4& operator[](int rowi) const { return row[rowi]; }
		inline Vec4& operator[](int rowi){ return row[rowi]; }
		inline operator float*(){ return &row[0][0]; }
		inline operator const float*() const { return &row[0][0]; }

		inline Matrix4x4 Transpose() const{
			return Matrix4x4(
				row[0][0], row[1][0], row[2][0], row[3][0],
				row[0][1], row[1][1], row[2][1], row[3][1],
				row[0][2], row[1][2], row[2][2], row[3][2],
				row[0][3], row[1][3], row[2][3], row[3][3]);
		}

		Matrix4x4 Inverse() const;

		/// <summary>
		/// Transforms a point.
		/// </summary>
		static inline Vec3 TPoint(const Matrix4x4& m, const Vec3& p){
			// apply translation
			return Vec3(
				p.x * m[0][0] + p.y * m[0][1] + p.z * m[0][2] + m[0][3],
				p.x * m[1][0] + p.y * m[1][1] + p.z * m[1][2] + m[1][3],
				p.x * m[2][0] + p.y * m[2][1] + p.z * m[2][2] + m[2][3]);
		}
		/// <summary>
		/// Transforms a vector.
		/// </summary>
		static inline Vec3 TVec(const Matrix4x4& m, const Vec3& v){
			// ignore translation
			return Vec3(
				v.x * m[0][0] + v.y * m[0][1] + v.z * m[0][2],
				v.x * m[1][0] + v.y * m[1][1] + v.z * m[1][2],
				v.x * m[2][0] + v.y * m[2][1] + v.z * m[2][2]);
		}
		/// <summary>
		/// Transforms a normal vector with an already inverted matrix, the result is not normalized.
		/// </summary>
		static inline Vec3 TNormal(const Matrix4x4& m_inv, const Vec3& n){
			// multiply the transpose
			return Vec3(
				n.x * m_inv[0][0] + n.y * m_inv[1][0] + n.z * m_inv[2][0],
				n.x * m_inv[0][1] + n.y * m_inv[1][1] + n.z * m_inv[2][1],
				n.x * m_inv[0][2] + n.y * m_inv[1][2] + n.z * m_inv[2][2]);
		}

		static Matrix4x4 Translate(const Vec3& v);
		static Matrix4x4 Translate(float x, float y, float z);

		static Matrix4x4 Rotate(const Vec3& rad);
		static Matrix4x4 Rotate(float radx, float rady, float radz);
		static Matrix4x4 Rotate(float rad, const Vec3& axis);

		/// <summary>
		/// Computes the view matrix (world -> local).
		/// To rotate -z axis to target dir, use it's inverse (or transpose) instead.
		/// </summary>
		static Matrix4x4 LookAt(const Vec3& pEye, const Vec3& pTarget, const Vec3& up);

		static Matrix4x4 Scale(const Vec3& s);
		static Matrix4x4 Scale(float x, float y, float z);

		/// <summary>
		/// Orthographic projection matrix, size is a similar term to fov.
		/// </summary>
		static Matrix4x4 Orthographic(float ratio, float size, float near, float far);
		/// <summary>
		/// Perspective projection matrix, fov is vertical.
		/// </summary>
		static Matrix4x4 Perspective(float ratio, float fov, float near, float far);
		/// <summary>
		/// Viewport matrix.
		/// </summary>
		static Matrix4x4 Viewport(float resx, float resy);
	};

	inline std::ostream& operator << (std::ostream& os, const Matrix4x4& m){
		for (int i = 0; i < 4; i++){
			for (int j = 0; j < 4; j++)
				os << m[i][j] << "\t";
			os << std::endl;
		}
		return os;
	}

	inline bool SolveLinearSystem2x2(const float A[2][2], const float B[2], float *x0, float *x1){
		float det = A[0][0] * A[1][1] - A[0][1] * A[1][0];
		if (Math::Abs(det) < Math::EPSILON)
			return false;
		*x0 = (A[1][1] * B[0] - A[0][1] * B[1]) / det;
		*x1 = (A[0][0] * B[1] - A[1][0] * B[0]) / det;
		return !(std::isnan(*x0) || std::isnan(*x1));
	}
}
