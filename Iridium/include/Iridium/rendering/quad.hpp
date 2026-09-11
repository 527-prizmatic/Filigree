#ifndef IRIDIUM_RENDERING_QUAD_HPP_
#define IRIDIUM_RENDERING_QUAD_HPP_

#include "rendering/rectangle.hpp"

namespace ir {
	namespace render {
		/// @brief Utility class for drawing quads (textured objects).
		/// For rectangles, position is considered to be the top-left corner.
		/// @future Integrate this with a future resource manager, when there will be one
		class Quad : public Rectangle {
		public:
			/// @brief Draws the object on the given render target.
			virtual void render(ir::render::VertexRenderer& renderer) const override;

			Quad& setUVs(ir::Vector top_left, ir::Vector size); ///< Sets UVs, in texture pixels
			Quad& setUVs(ir::render::UV uv); ///< Sets UVs, in texture pixels
			Quad& setTexture(const sf::Texture& texture); ///< Sets texture resource

			[[nodiscard]] ir::render::UV UVs() const { return uv_; } ///< @return UVs, in texture pixels

		private:
			ir::render::UV uv_ {}; ///< Texture UVs
			const sf::Texture* texture_ {}; ///< Texture to use in rendering
		};
	}
}

#endif // IRIDIUM_RENDERING_QUAD_HPP_