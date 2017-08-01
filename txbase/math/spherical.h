#pragma once

#include "txbase/math/base.h"
#include "txbase/math/vector.h"

namespace TX {
	class Spherical {
	private:
		union {
			Vec3 v;
			struct {
				float rho; 		// radial distance
				float theta;	// polar angle
				float phi; 		// azimuthal angle
			};
		};
	public:
		Spherical(): rho(1.0f), theta(0.f), phi(0.f) {}
		Spherical(const Spherical& ot): v(ot.v) {}
		Spherical(float rho, float theta, float phi)
		{
			Radial(rho);
			Polar(theta);
			Azimuthal(phi);
		}
		Spherical& operator = (const Spherical& ot) { v = ot.v; return *this; }
		explicit Spherical(const Vec3& v) : Spherical(v.x, v.y, v.z) {}

		inline bool operator == (const Spherical& ot) const { return v == ot.v; }
		inline bool operator != (const Spherical& ot) const { return v != ot.v; }

		inline float Radial() const { return this->rho; }
		inline void Radial(float rho) { this->rho = Math::Max(0.f, rho); }

		inline float Polar() const { return this->theta; }
		inline void Polar(float theta) { this->theta = Math::Clamp(theta, 0.f, float(Math::PI)); }

		inline float Azimuthal() const { return this->phi; }
		inline void Azimuthal(float phi) { this->phi = fmod(phi, 2 * Math::PI); }

		Vec3 Cartesian_Z() const;
		static Spherical Cartesian_Z(Vec3 cartesian);
		Vec3 Cartesian_Y() const;
		static Spherical Cartesian_Y(Vec3 cartesian);
	};

	inline std::ostream& operator << (std::ostream& os, const Spherical& s) {
		return os << "(r=" << s.rho << ", th=" << s.theta << ", ph=" << s.phi << ")";
	}
}