#ifndef IRIDIUM_RENDERING_RECTANGLE_HPP_
#define IRIDIUM_RENDERING_RECTANGLE_HPP_

#include "rendering/shape.hpp"

namespace ir {
	namespace render {
		/// @brief Utility class for drawing wireframe rectangles.
		/// For rectangles, position is considered to be the top-left corner.
		class Rectangle : public Shape {
		public:
			/// @brief Draws the object on the given render target.
			virtual void render(ir::render::VertexRenderer& renderer) const;
			
			Rectangle& setSize(ir::Vector size); ///< @brief Sets size, in pixels
			Rectangle& setSize(float x, float y); ///< @brief Sets size, in pixels
			Rectangle& setCorners(ir::Vector topLeft, ir::Vector bottomRight); ///< @brief Sets rectangle's corners as screen positions

			[[nodiscard]] inline ir::Vector size() const { return size_; } ///< @return Rectangle size, in pixels

		private:
			ir::Vector size_ { 0.f, 0.f }; ///< Size
		};
	}
}

#endif // IRIDIUM_RENDERING_RECTANGLE_HPP_