#include "spherical.h"

namespace TX {

	static Vec3 YUp2ZUp(Vec3 yUp) {
		return Vec3(yUp.z, yUp.x, yUp.y);
	}
	static Vec3 ZUp2YUp(Vec3 zUp) {
		return Vec3(zUp.y, zUp.z, zUp.x);
	}

	Vec3 Spherical::Cartesian_Z() const {
		float st = Math::Sin(theta);
		return Vec3(
			rho * st * Math::Cos(phi),
			rho * st * Math::Sin(phi),
			rho * Math::Cos(theta)
		);
	}

	Spherical Spherical::Cartesian_Z(Vec3 cartesian) {
		float rho = Math::Length(cartesian);
		return Spherical(
			rho,
			Math::Acos(cartesian.z / rho),
			Math::Atan2(cartesian.y, cartesian.x)
		);
	}

	Vec3 Spherical::Cartesian_Y() const {
		return ZUp2YUp(Cartesian_Z());
	}

	Spherical Spherical::Cartesian_Y(Vec3 cartesian) {
		return Spherical::Cartesian_Z(YUp2ZUp(cartesian));
	}
}