#include "rendering/rectangle.hpp"
#include "rendering/vertex_renderer.hpp"

namespace ir {
	namespace render {
		Rectangle &Rectangle::setSize(ir::Vector size) {
			size_ = size;
			return *this;
		}

		Rectangle &Rectangle::setSize(float x, float y) {
			size_ = ir::Vector { x, y };
			return *this;
		}

		Rectangle& Rectangle::setCorners(ir::Vector topLeft, ir::Vector bottomRight) {
			setPosition(topLeft);
			setSize(bottomRight - topLeft);
			return *this;
		}

		void Rectangle::render(ir::render::VertexRenderer& renderer) const {
			ir::Vector point1 { ir::Vector(anchor_).rotate(angle_) };
			ir::Vector point2 { ir::Vector(point1 + ir::Vector{ size_.x, 0.f }).rotate(angle_) };
			ir::Vector point3 { ir::Vector(point1 + size_).rotate(angle_) };
			ir::Vector point4 { ir::Vector(point1 + ir::Vector{ 0.f, size_.y }).rotate(angle_) };

			if (mode() == ir::render::Mode::WIREFRAME) {
				renderer.reset(sf::PrimitiveType::LineStrip);
				renderer.addPoint(position_ + point1, color_);
				renderer.addPoint(position_ + point2, color_);
				renderer.addPoint(position_ + point3, color_);
				renderer.addPoint(position_ + point4, color_);
				renderer.addPoint(position_ + point1, color_);
			}
			
			else if (mode() == ir::render::Mode::SOLID) {
				renderer.reset(sf::PrimitiveType::TriangleStrip);
				renderer.addPoint(position_ + point1, color_);
				renderer.addPoint(position_ + point2, color_);
				renderer.addPoint(position_ + point4, color_);
				renderer.addPoint(position_ + point3, color_);
			}

			renderer.flush();
		}
	}
}