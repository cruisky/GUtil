#include "txbase/stdafx.h"
#include "txbase/math/matrix.h"
#include "txbase/math/vector.h"
#include "txbase/math/ray.h"

namespace TX{
	const Matrix3x3 Matrix3x3::IDENTITY = Matrix3x3(
		1.f, 0.f, 0.f,
		0.f, 1.f, 0.f,
		0.f, 0.f, 1.f);


	const Matrix4x4 Matrix4x4::IDENTITY = Matrix4x4(
		1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f);

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