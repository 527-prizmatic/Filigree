#include "rendering/circle.hpp"
#include "rendering/vertex_renderer.hpp"

namespace ir {
	namespace render {
		Circle& Circle::setRadius(float rds) {
			radius_ = rds;
			return *this;
		}

		Circle& Circle::setVertexCount(unsigned int count) {
			vertexCount_ = count;
			vertexAngle_ = ir::math::tau / static_cast<float>(vertexCount_);
			return *this;
		}

		void Circle::render(ir::render::VertexRenderer& renderer) const {
			renderer.reset(sf::PrimitiveType::LineStrip);

			ir::Vector center = position_ - anchor_;

			for (unsigned int i = 0; i <= vertexCount_; i++) {
				ir::Vector vertexPos { ir::Vector::polar(radius_, angle_ + vertexAngle_ * static_cast<float>(i)) };
				renderer.addPoint(center + vertexPos, color_);
			}

			renderer.flush();
		}
	}
}