#ifndef IRIDIUM_SUBWINDOW_HPP_
#define IRIDIUM_SUBWINDOW_HPP_

#include "render_target.hpp"
#include "rendering/quad.hpp"

namespace ir {
	class SubWindow: public ir::RenderTarget {
	public:
		SubWindow(ir::Vector size);

		/// @brief Deletes all internal resources and regenerates them with the provided size in pixels.
		/// Checking if the window is in an invalid state before calling this function is not necessary, as it will attempt to reallocate them anyway.
		/// @param _size New size of the internal window and render texture, in pixels.
		void setSize(ir::Vector size);

		/// @return Size of the internal resources, in pixels
		/// @attention Attempting to call this function while the window is in an invalid state will throw an exception.
		ir::Vector size();

		/// @brief Clears the rendering buffer and fills it with the given color.
		/// @attention Attempting to call this function while the buffer is in an invalid state will throw an exception.
		void clear(sf::Color fillColor = sf::Color::Transparent);

		/// @brief Draws a SFML drawable object into the rendering buffer.
		/// @attention Attempting to call this function while the buffer is in an invalid state will throw an exception.
		void render(sf::Drawable& drawable, const sf::Texture* texture = nullptr);

		/// @brief Draws the contents of another render target into this one.
		/// @attention Attempting to call this function while the buffer is in an invalid state will throw an exception.
		void render(ir::RenderTarget& renderTarget);

		/// @brief Draws the contents of the subwindow's rendering buffer into another render target.
		/// @attention Attempting to call this function while either buffer is in an invalid state will throw an exception.
		void flushToTarget(ir::RenderTarget& renderTarget);

		/// @brief Subwindows contain a sf::RenderTexture as their rendering buffer.
		/// The object is considered valid if it has been correctly allocated.
		/// @return Whether the subwindow has been properly initialized and is ready for rendering
		bool isValid() {
			return renderTexture_ != nullptr && frame_ != nullptr;
		}

		/// @brief Whether a frame should be rendered around the subwindow upon calling flushToTarget().
		void setRenderFrame(bool val) { renderFrame_ = val; }

		/// @brief Sets the color of the frame to be rendered around the subwindow.
		void setRenderFrameColor(sf::Color clr);

		/// @brief Sets the position where the subwindow should be rendered, relative to the parent.
		void setPosition(ir::Vector pos) { position_ = pos; }
		ir::Vector position() const { return position_; }

	private:
		std::unique_ptr<sf::RenderTexture> renderTexture_;
		std::unique_ptr<ir::render::Quad> rect_; ///< Rectangle shape used for rendering the subwindow upon calling flushToTarget()
		std::unique_ptr<ir::render::Rectangle> frame_; ///< Rectangle shape used to render the frame
		ir::Vector position_; ///< Where to draw the subwindow relative to its parent

		bool renderFrame_ { true }; ///< Whether to draw a frame around the subwindow

		/// @brief Reallocates internal resources, deleting the previous ones if they existed.
		/// @attention This overload reuses the previously-allocated resources' size in pixels.
		/// Attempting to call this function while the window is in an invalid state will throw an exception.
		void allocateResources();

		/// @brief Reallocates internal resources, deleting the previous ones if they existed.
		/// @param _size Size of the newly-allocated resources, in pixels.
		void allocateResources(ir::Vector size);

		void configureRect(ir::Vector size);
		void configureFrame(ir::Vector size);
	};
}

#endif // IRIDIUM_SUBWINDOW_HPP_