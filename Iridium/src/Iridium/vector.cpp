#include "vector.hpp"
#include "math.hpp"

namespace ir {
	const ir::Vector ir::Vector::kZero = ir::Vector{ 0.f, 0.f };
	const ir::Vector ir::Vector::kUnit = ir::Vector{ 1.f, 0.f };

	Vector::Vector() {
		x = 0.f;
		y = 0.f;
	}

	Vector::Vector(float x, float y) {
		this->x = x;
		this->y = y;
	}

	Vector::Vector(const ir::Vector& other) {
		x = other.x;
		y = other.y;
	}

	ir::Vector& Vector::operator=(const ir::Vector& other) {
		x = other.x;
		y = other.y;
		return *this;
	}

	bool Vector::isZero() const {
		return false;
	}

	ir::Vector Vector::polar(float radius, float angle) {
		Expects(radius >= 0.f);
		return ir::Vector(radius, angle).polToRec();
	}
	
	float Vector::magnitude() const {
		return std::sqrtf(magnitudeSquare());
	}

	float Vector::magnitudeSquare() const {
		return x * x + y * y;
	}
	
	float Vector::angle(ir::Vector reference) const {
		if (isZero()) {
			return 0.f;
		}
		return std::atan2(y, x) - std::atan2(reference.y, reference.x);
	}
	
	float Vector::distance(ir::Vector other) const {
		return (*this - other).magnitude();
	}
	
	ir::Vector Vector::normalize() const {
		if (isZero()) {
			return *this;
		}
		return *this / magnitude();
	}
	
	ir::Vector Vector::clampMagnitude(float max) const {
		Expects(max >= 0.f);

		float mag { magnitude() };
		if (mag > max) {
			return *this / mag * max;
		}
		return *this;
	}
	
	ir::Vector Vector::clampMagnitude(float min, float max) const {
		Expects(max >= min);

		float mag { magnitude() };
		if (mag < min) {
			return *this / mag * min;
		}
		if (mag > max) {
			return *this / mag * max;
		}
		return *this;
	}
	
	ir::Vector Vector::recToPol() const {
		return ir::Vector{ magnitude(), angle() };
	}
	
	ir::Vector Vector::polToRec() const {
		return ir::Vector{ x * std::cosf(y), x * std::sinf(y) };
	}
	
	ir::Vector Vector::rotate(float angle) const {
		if (ir::math::isZero(angle) || isZero()) {
			return *this;
		}
		ir::Vector v { recToPol() };
		v.y += angle;
		return v.polToRec();
	}
	
	ir::Vector Vector::rotate(float angle, ir::Vector reference) const {
		return reference + (*this - reference).rotate(angle);
	}
	
	ir::Vector Vector::mirror(ir::Vector axis) const {
		if (isZero() || axis.isZero()) {
			return *this;
		}
		float currentAngle { angle(axis) };
		if (ir::math::isZero(currentAngle)) {
			return *this;
		}
		return rotate(currentAngle * 2.f);
	}
	
	ir::Vector Vector::mirrorX() const {
		return ir::Vector{ -x, y };
	}
	
	ir::Vector Vector::mirrorY() const {
		return ir::Vector{ x, -y };
	}
	
	ir::Vector Vector::negate() const {
		return ir::Vector{ -x, -y };
	}
}