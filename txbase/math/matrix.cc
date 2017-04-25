#include "txbase/stdafx.h"
#include "txbase/math/matrix.h"
#include "txbase/math/vector.h"
#include "txbase/math/ray.h"
#include "txbase/sse/sse.h"

namespace TX{
	const Matrix3x3 Matrix3x3::IDENTITY = Matrix3x3(
		1.f, 0.f, 0.f,
		0.f, 1.f, 0.f,
		0.f, 0.f, 1.f);
	Matrix3x3::Matrix3x3(const Matrix4x4& ot) : Matrix3x3(Vec3(ot[0]), Vec3(ot[1]), Vec3(ot[2])){}


	const Matrix4x4 Matrix4x4::IDENTITY = Matrix4x4(
		1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f);

	Matrix4x4 Matrix4x4::Inverse() const {
		Matrix4x4 result;
		const float *src = (const float *)*row;
		float *dst = (float *)result[0];
		// Intel AP-928
		__m128 minor0, minor1, minor2, minor3;
		__m128 row0, row1, row2, row3;
		__m128 det, tmp1;
		tmp1 = _mm_loadh_pi(_mm_loadl_pi(_mm_setzero_ps(), (__m64*)(src)), (__m64*)(src + 4));
		row1 = _mm_loadh_pi(_mm_loadl_pi(_mm_setzero_ps(), (__m64*)(src + 8)), (__m64*)(src + 12));
		row0 = _mm_shuffle_ps(tmp1, row1, 0x88);
		row1 = _mm_shuffle_ps(row1, tmp1, 0xDD);
		tmp1 = _mm_loadh_pi(_mm_loadl_pi(tmp1, (__m64*)(src + 2)), (__m64*)(src + 6));
		row3 = _mm_loadh_pi(_mm_loadl_pi(_mm_setzero_ps(), (__m64*)(src + 10)), (__m64*)(src + 14));
		row2 = _mm_shuffle_ps(tmp1, row3, 0x88);
		row3 = _mm_shuffle_ps(row3, tmp1, 0xDD);					//
		tmp1 = _mm_mul_ps(row2, row3);
		tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
		minor0 = _mm_mul_ps(row1, tmp1);
		minor1 = _mm_mul_ps(row0, tmp1);
		tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
		minor0 = _mm_sub_ps(_mm_mul_ps(row1, tmp1), minor0);
		minor1 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor1);
		minor1 = _mm_shuffle_ps(minor1, minor1, 0x4E);				//
		tmp1 = _mm_mul_ps(row1, row2);
		tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
		minor0 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor0);
		minor3 = _mm_mul_ps(row0, tmp1);
		tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
		minor0 = _mm_sub_ps(minor0, _mm_mul_ps(row3, tmp1));
		minor3 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor3);
		minor3 = _mm_shuffle_ps(minor3, minor3, 0x4E);				//
		tmp1 = _mm_mul_ps(_mm_shuffle_ps(row1, row1, 0x4E), row3);
		tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
		row2 = _mm_shuffle_ps(row2, row2, 0x4E);
		minor0 = _mm_add_ps(_mm_mul_ps(row2, tmp1), minor0);
		minor2 = _mm_mul_ps(row0, tmp1);
		tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
		minor0 = _mm_sub_ps(minor0, _mm_mul_ps(row2, tmp1));
		minor2 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor2);
		minor2 = _mm_shuffle_ps(minor2, minor2, 0x4E);				//
		tmp1 = _mm_mul_ps(row0, row1);
		tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
		minor2 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor2);
		minor3 = _mm_sub_ps(_mm_mul_ps(row2, tmp1), minor3);
		tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
		minor2 = _mm_sub_ps(_mm_mul_ps(row3, tmp1), minor2);
		minor3 = _mm_sub_ps(minor3, _mm_mul_ps(row2, tmp1));		//
		tmp1 = _mm_mul_ps(row0, row3);
		tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
		minor1 = _mm_sub_ps(minor1, _mm_mul_ps(row2, tmp1));
		minor2 = _mm_add_ps(_mm_mul_ps(row1, tmp1), minor2);
		tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
		minor1 = _mm_add_ps(_mm_mul_ps(row2, tmp1), minor1);
		minor2 = _mm_sub_ps(minor2, _mm_mul_ps(row1, tmp1));		//
		tmp1 = _mm_mul_ps(row0, row2);
		tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
		minor1 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor1);
		minor3 = _mm_sub_ps(minor3, _mm_mul_ps(row1, tmp1));
		tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
		minor1 = _mm_sub_ps(minor1, _mm_mul_ps(row3, tmp1));
		minor3 = _mm_add_ps(_mm_mul_ps(row1, tmp1), minor3);		//
		det = _mm_mul_ps(row0, minor0);
		det = _mm_add_ps(_mm_shuffle_ps(det, det, 0x4E), det);
		det = _mm_add_ss(_mm_shuffle_ps(det, det, 0xB1), det);
		tmp1 = _mm_rcp_ss(det);
		det = _mm_sub_ss(_mm_add_ss(tmp1, tmp1), _mm_mul_ss(det, _mm_mul_ss(tmp1, tmp1)));
		det = _mm_shuffle_ps(det, det, 0x00);
		minor0 = _mm_mul_ps(det, minor0);
		_mm_storel_pi((__m64*)(dst), minor0);
		_mm_storeh_pi((__m64*)(dst + 2), minor0);
		minor1 = _mm_mul_ps(det, minor1);
		_mm_storel_pi((__m64*)(dst + 4), minor1);
		_mm_storeh_pi((__m64*)(dst + 6), minor1);
		minor2 = _mm_mul_ps(det, minor2);
		_mm_storel_pi((__m64*)(dst + 8), minor2);
		_mm_storeh_pi((__m64*)(dst + 10), minor2);
		minor3 = _mm_mul_ps(det, minor3);
		_mm_storel_pi((__m64*)(dst + 12), minor3);
		_mm_storeh_pi((__m64*)(dst + 14), minor3);
		return result;
	}

	Matrix4x4 Matrix4x4::Translate(const Vec3& v){
		return Translate(v.x, v.y, v.z);
	}

	Matrix4x4 Matrix4x4::Translate(float x, float y, float z){
		return Matrix4x4(
			1.f, 0.f, 0.f, x,
			0.f, 1.f, 0.f, y,
			0.f, 0.f, 1.f, z,
			0.f, 0.f, 0.f, 1.f);
	}

	Matrix4x4 Matrix4x4::Rotate(float rad, const Vec3& axis){
		Vec3 a(axis);
		a = Math::Normalize(a);
		float c = Math::Cos(rad), s = Math::Sin(rad), c1 = 1 - c;
		return Matrix4x4(
			a.x*a.x*c1 + c, a.x*a.y*c1 - a.z*s, a.x*a.z*c1 + a.y*s, 0.f,
			a.x*a.y*c1 + a.z*s, a.y*a.y*c1 + c, a.y*a.z*c1 - a.x*s, 0.f,
			a.x*a.z*c1 - a.y*s, a.y*a.z*c1 + a.x*s, a.z*a.z*c1 + c, 0.f,
			0.f, 0.f, 0.f, 1.f);
	}

	Matrix4x4 Matrix4x4::Rotate(const Vec3& rad){
		return Rotate(rad.x, rad.y, rad.z);
	}

	Matrix4x4 Matrix4x4::Rotate(float radx, float rady, float radz){
		float sx = Math::Sin(radx);
		float sy = Math::Sin(rady);
		float sz = Math::Sin(radz);
		float cx = Math::Cos(radx);
		float cy = Math::Cos(rady);
		float cz = Math::Cos(radz);
		return Matrix4x4(
			cz*cy, cz*sy*sx - sz*cx, cz*sy*cx + sz*sx, 0.f,
			sz*cy, sz*sy*sx + cz*cx, sz*sy*cx - cz*sx, 0.f,
			-sy, cy*sx, cy*cx, 0.f,
			0.f, 0.f, 0.f, 1.f);
	}
	Matrix4x4 Matrix4x4::LookAt(const Vec3& pEye, const Vec3& pTarget, const Vec3& up){
		Vec3 zaxis = Math::Normalize(pEye - pTarget);	// -z is forward
		Vec3 xaxis = Math::Normalize(Math::Cross(up, zaxis));
		Vec3 yaxis = Math::Cross(zaxis, xaxis);
		return Matrix4x4(
			xaxis.x, xaxis.y, xaxis.z, -Math::Dot(xaxis, pEye),
			yaxis.x, yaxis.y, yaxis.z, -Math::Dot(yaxis, pEye),
			zaxis.x, zaxis.y, zaxis.z, -Math::Dot(zaxis, pEye),
			0.f, 0.f, 0.f, 1.f);
	}

	Matrix4x4 Matrix4x4::Scale(const Vec3& s){
		return Scale(s.x, s.y, s.z);
	}

	Matrix4x4 Matrix4x4::Scale(float x, float y, float z){
		return Matrix4x4(
			x, 0.f, 0.f, 0.f,
			0.f, y, 0.f, 0.f,
			0.f, 0.f, z, 0.f,
			0.f, 0.f, 0.f, 1.f);
	}

	Matrix4x4 Matrix4x4::Orthographic(float ratio, float size, float an, float af){
		assert(an > 0 && af > 0);
		float t = size;			// top
		float r = ratio * t;	// right
		return Matrix4x4(
			Vec4(1.f / r, 0.f, 0.f, 0.f),
			Vec4(0.f, 1.f / t, 0.f, 0.f),
			Vec4(0.f, 0.f, 1.f / (af - an), (an + af) / (af - an)),
			Vec4::W);
	}

	Matrix4x4 Matrix4x4::Perspective(float ratio, float fov, float an, float af){
		assert(an > 0 && af > 0);
		float t = Math::Tan(Math::ToRad(fov) / 2.f) * an;		// top
		float r = ratio * t;						// right
		return Matrix4x4(
			Vec4(an / r, 0.f, 0.f, 0.f),
			Vec4(0.f, an / t, 0.f, 0.f),
			Vec4(0.f, 0.f, (an + af) / (an - af), 2 * af * an / (an - af)),
			Vec4(0.f, 0.f, -1.f, 0.f));
	}

	Matrix4x4 Matrix4x4::Viewport(float resx, float resy){
		resx *= 0.5f;
		resy *= 0.5f;
		// upside down screen space
		return Matrix4x4(
			Vec4(resx, 0.f, 0.f, resx - 0.5f),
			Vec4(0.f, resy, 0.f, resy - 0.5f),
			Vec4(0.f, 0.f, 1.f, 0.f),
			Vec4::W);

		//return Matrix4x4(
		//	resx, 0.f, 0.f, resx - 0.5f,
		//	0.f, -resy, 0.f, resy - 0.5f,
		//	0.f, 0.f, 1.f, 0.f,
		//	0.f, 0.f, 0.f, 1.f);
	}
}
