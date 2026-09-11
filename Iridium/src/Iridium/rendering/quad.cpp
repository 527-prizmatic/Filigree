#include "rendering/quad.hpp"
#include "rendering/vertex_renderer.hpp"

namespace ir {
	namespace render {
		Quad& Quad::setUVs(ir::Vector topLeft, ir::Vector size) {
			uv_ = ir::render::UV(topLeft, size);
			return *this;
		}

		Quad& Quad::setUVs(ir::render::UV uv) {
			uv_ = uv;
			return *this;
		}

		Quad& Quad::setTexture(const sf::Texture& texture) {
			texture_ = &texture;
			setSize(ir::Vector::fromSFMLVector(texture.getSize()));
			uv_ = ir::render::UV{ ir::Vector::kZero, size() };

			return *this;
		}

		void Quad::render(ir::render::VertexRenderer& renderer) const {
			if (!texture_) {
				return;
			}
			
			renderer.reset(sf::PrimitiveType::TriangleStrip);

			ir::Vector pointTL { ir::Vector(-anchor_).rotate(angle_) };
			ir::Vector pointTR { ir::Vector(pointTL + ir::Vector{ size().x, 0.f }).rotate(angle_) };
			ir::Vector pointBR { ir::Vector(pointTL + ir::Vector{ size().x, size().y }).rotate(angle_) };
			ir::Vector pointBL { ir::Vector(pointTL + ir::Vector{ 0.f, size().y }).rotate(angle_) };

			renderer.addPoint(position_ + pointTL, uv_.topLeftCorner());
			renderer.addPoint(position_ + pointTR, uv_.topRightCorner());
			renderer.addPoint(position_ + pointBL, uv_.bottomLeftCorner());
			renderer.addPoint(position_ + pointBR, uv_.bottomRightCorner());

			renderer.flush(texture_);
		}
	}
}