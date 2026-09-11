#ifndef IRIDIUM_RENDERING_CIRCLE_HPP_
#define IRIDIUM_RENDERING_CIRCLE_HPP_

#include "rendering/shape.hpp"

namespace ir {
	namespace render {
		/// @brief Utility class for drawing wireframe circles.
		/// Also usable for regular polygons, as circles are essentially just polygons with many vertices.
		/// For such shapes, position is considered to be the center.
		class Circle : public Shape {
		public:
			/// @brief Draws the object on the given render target.
			virtual void render(ir::render::VertexRenderer& renderer) const;
			
			Circle& setRadius(float rds); ///< @brief Sets radius, in pixels
			Circle& setVertexCount(unsigned int count); ///< @brief Sets vertex count

			[[nodiscard]] inline float radius() const { return radius_; } ///< @return Radius, in pixels
			[[nodiscard]] inline unsigned int vertexCount() const { return vertexCount_; } ///< @return Vertex count

		private:
			float radius_ { 0.f }; ///< Radius
			unsigned int vertexCount_ { 32u }; ///< Vertex count
			float vertexAngle_ { ir::math::tau / 32.f }; ///< Angle between two vertices. Used internally for reducing calculations in render calls.
		};
	}
}

#endif // IRIDIUM_RENDERING_CIRCLE_HPP_