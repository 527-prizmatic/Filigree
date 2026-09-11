#include "rendering/vertex_renderer.hpp"
#include "render_target.hpp"

namespace ir {
	namespace render {
		VertexRenderer::VertexRenderer(ir::RenderTarget* target) : target_ { target } {
			buffer_ = std::make_unique<sf::VertexArray>();
		}

		void VertexRenderer::reset(sf::PrimitiveType type) {
			buffer_->setPrimitiveType(type);
			buffer_->clear();
		}

		void VertexRenderer::flush(const sf::Texture* texture) {
			target_->render(*buffer_, texture);
		}

		void VertexRenderer::addPoint(ir::Vector position, sf::Color color) {
			sf::Vertex vertex { static_cast<sf::Vector2f>(position), color };
			buffer_->append(vertex);
		}

		void VertexRenderer::addPoint(ir::Vector position, ir::Vector textureCoord) {
			sf::Vertex vertex { static_cast<sf::Vector2f>(position), sf::Color::White, sf::Vector2f{ textureCoord } };
			buffer_->append(vertex);
		}
	}
}