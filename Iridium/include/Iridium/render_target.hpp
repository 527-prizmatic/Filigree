#ifndef IRIDIUM_RENDERTARGET_HPP_
#define IRIDIUM_RENDERTARGET_HPP_

#include "libraries.hpp"
#include "vector.hpp"

namespace ir {
	namespace render{
		class VertexRenderer;
	}

	/// @brief Base class for various rendering buffers.
	/// They are typically implemented as wrappers for a sf::RenderTexture,
	/// coming with various utilities for ease of manipulation by the end user.
	class RenderTarget {
	public:
		/// @brief Resizes the render target's internal buffer.
		virtual void setSize(ir::Vector size) = 0;

		/// @return Size of the render target's buffer
		virtual ir::Vector size() = 0;

		/// @brief Completely clears the render target's buffer and fills it with the given color.
		virtual void clear(sf::Color fillColor = sf::Color::Transparent) = 0;

		/// @brief Renders the given object onto the render target.
		virtual void render(sf::Drawable& drawable, const sf::Texture* texture = nullptr) = 0;
		
		/// @brief Renders the given object onto the render target.
	//	virtual void render(ir::render::Shape& shape) = 0;

		/// @brief Renders the contents of the given render target's buffer onto this one.
		virtual void render(ir::RenderTarget& renderTarget) = 0;

		/// @return Whether the render target has been properly initialized and is ready for rendering
		virtual bool isValid() = 0;

		void setRenderer(ir::render::VertexRenderer* renderer) {
			renderer_ = renderer;
		}

		ir::render::VertexRenderer* getRenderer() { return renderer_; }
	
	protected:
		ir::render::VertexRenderer* renderer_;

		void expectValid();
	};
};

#endif // IRIDIUM_RENDERTARGET_HPP_