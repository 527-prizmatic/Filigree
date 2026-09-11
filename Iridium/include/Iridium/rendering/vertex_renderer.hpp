#ifndef IRIDIUM_RENDERING_VERTEX_RENDERER_HPP_
#define IRIDIUM_RENDERING_VERTEX_RENDERER_HPP_

#include "vector.hpp"

namespace ir {
	class RenderTarget;

	namespace render {
		class Vertex;

		/// @brief Utility class to interface between shapes and windows.
		///
		/// Vertex renderers wrap a sf::VertexArray and include various interfacing functions for shapes to draw on render targets.
		/// They enable for batch rendering, which is especially useful with particularly large or complex VMFs such as text.
		/// All objects attempting to draw on a window should go through this subsystem.
		class VertexRenderer {
		public:
			VertexRenderer() = delete;
			VertexRenderer(ir::RenderTarget* target);

			void reset(sf::PrimitiveType type = sf::PrimitiveType::Lines);

			void flush(const sf::Texture* texture = nullptr);

			void addPoint(ir::Vector position, sf::Color color = sf::Color::White);
			void addPoint(ir::Vector position, ir::Vector textureCoord);

			ir::RenderTarget* getTarget() { return target_; }

		private:
			ir::RenderTarget* target_;
			std::unique_ptr<sf::VertexArray> buffer_;
		};
	}
}

#endif /// IRIDIUM_RENDERING_VERTEX_RENDERER_HPP_