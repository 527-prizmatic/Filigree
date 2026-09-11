#ifndef IRIDIUM_APPWINDOW_HPP_
#define IRIDIUM_APPWINDOW_HPP_

#include "render_target.hpp"
#include "rendering/quad.hpp"

namespace ir {
	class ApplicationWindow: public ir::RenderTarget {
	public:
		ApplicationWindow(ir::Vector size);

		/// @brief Deletes all internal resources and regenerates them with the provided size in pixels.
		/// Checking if the window is in an invalid state before calling this function is not necessary, as it will attempt to reallocate them anyway.
		/// @param size New size of the internal window and render texture, in pixels.
		void setSize(ir::Vector size);

		/// @return Size of the internal resources, in pixels
		/// @attention Attempting to call this function while the window is in an invalid state will throw an exception.
		ir::Vector size();

		/// @brief Clears the rendering buffer and fills it with the given color.
		/// @attention Attempting to call this function while the window is in an invalid state will throw an exception.
		void clear(sf::Color fillColor = sf::Color::Transparent);

		/// @brief Draws a SFML drawable object into the rendering buffer.
		/// @attention Attempting to call this function while the window is in an invalid state will throw an exception.
		void render(sf::Drawable& drawable, const sf::Texture* texture = nullptr);

		/// @attention Attempting to call this function while the window is in an invalid state will throw an exception.
		void render(ir::RenderTarget& renderTarget);

		/// @brief Flushes the render texture onto the render window.
		/// This function will most likely be the last one called in the game loop.
		void flush();

		/// @brief Minimizes the window.
		/// Exact implementation differs based on the OS.
		/// @future Add support for other OSes
		void minimize();

		/// @brief Reduces framerate when running in background to save on resources.
		void reduceBackgroundResourceUsage();

		/// @brief An application window contains two dynamically-allocated resources: a sf::renderWindow and a sf::RenderTexture.
		/// The object is considered valid if and only if both have been correctly allocated.
		/// @return Whether the application window has been properly initialized and is ready for rendering
		bool isValid() {
			return renderWindow_ != nullptr && renderTexture_ != nullptr && rect_ != nullptr;
		}

		/// @brief Sets target FPS.
		/// The application window will force its thread to sleep to match the requested framerate,
		/// so use with caution in time-critical applications.
		void setFPS(unsigned int fps);

		/// @return Current target FPS
		[[nodiscard]] unsigned int getFPS() { return fps_; }

		/// @return Whether the application window has system focus
		[[nodiscard]] bool hasFocus() const { return renderWindow_->hasFocus(); }

		/// @return Current position of the mouse cursor within the application window
		[[nodiscard]] sf::Vector2i getMouseCursorPosition() const;

		/// @return Next queued SFML event
		[[nodiscard]] const std::optional<sf::Event> pollNextEvent();

		void setTitle(std::string title);

		std::string getTitle();

		/// @brief Enables or disables mouse cursor visibility.
		void enableMouseCursor(bool val);

	private:
		std::shared_ptr<sf::RenderWindow> renderWindow_;
		std::unique_ptr<sf::RenderTexture> renderTexture_;
		std::unique_ptr<sf::RectangleShape> rect_; ///< Rectangle shape used for rendering the texture onto the window upon calling Flush()
		std::string windowTitle_ {};
		unsigned int fps_ { 60u }; ///< Target FPS
		
		/// @brief Reallocates internal resources, deleting the previous ones if they existed.
		/// @attention This overload reuses the previously-allocated resources' size in pixels.
		/// Attempting to call this function while the texture is in an invalid state will throw an exception.
		void allocateResources();

		/// @brief Reallocates internal resources, deleting the previous ones if they existed.
		/// @param size Size of the newly-allocated resources, in pixels.
		void allocateResources(sf::Vector2u size);

		void configureRect();
	};
}

#endif // IRIDIUM_APPWINDOW_HPP_