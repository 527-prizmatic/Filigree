#include "rendering/shape.hpp"

namespace ir {
	namespace render {
		Shape& Shape::setPosition(ir::Vector pos) {
			position_ = pos;
			return *this;
		}

		Shape& Shape::setAngle(float angle) {
			angle_ = angle;
			if (angle_ < -ir::math::pi) {
				angle_ += ir::math::tau;
			}
			if (angle_ >= ir::math::pi) {
				angle_ -= ir::math::tau;
			}
			if (abs(angle_) < 1e-6) {
				angle_ = 0.f;
			}
			return *this;
		}

		Shape& Shape::setAnchor(ir::Vector anchor) {
			anchor_ = anchor;
			return *this;
		}

		Shape& Shape::setAnchor(float x, float y) {
			anchor_ = ir::Vector { x, y };
			return *this;
		}

		Shape& Shape::setColor(sf::Color color) {
			color_ = color;
			return *this;
		}

		Shape &Shape::setMode(ir::render::Mode mode) {
			mode_ = mode;
			return *this;
		}
	}
}