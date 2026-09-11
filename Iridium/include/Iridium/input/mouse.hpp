#ifndef IRIDIUM_INPUT_MOUSE_HPP_
#define IRIDIUM_INPUT_MOUSE_HPP_

#include "libraries.hpp"
#include "input/state.hpp"
#include "application_window.hpp"

namespace ir::input {
	/// @brief Wraps functions for handling mouse input.
	/// Iridium provides a simple wrapper around SFML's utilities to facilitate detection of button states and easier handling of cursor position.
	class Mouse {
	public:
		void update(ir::ApplicationWindow& window);

		[[nodiscard]] bool isIdle(sf::Mouse::Button button) const; ///< @return Whether the specified button is not being pressed (continuous)
		[[nodiscard]] bool isPressed(sf::Mouse::Button button) const; ///< @return Whether the specified button was just pressed (1-frame pulse)
		[[nodiscard]] bool isActive(sf::Mouse::Button button) const; ///< @return Whether the specified button is being held down (continuous)
		[[nodiscard]] bool isReleased(sf::Mouse::Button button) const; ///< @return Whether the specified button was just released (1-frame pulse)
		[[nodiscard]] ir::input::State getState(sf::Mouse::Button button) const; ///< @return Current state of the specified button

		/// @return Whether the mouse cursor is within the specified region of the linked application window
		[[nodiscard]] bool isWithinArea(ir::Vector topLeft, ir::Vector size) const;

		/// @return Mouse position relative to the linked window, or to the desktop if there is none
		[[nodiscard]] ir::Vector cursorPosition() const;

	private:
		std::array<ir::input::State, sf::Mouse::ButtonCount> states_ { IDLE };
		ir::Vector cursorPosition_ { ir::Vector::kZero };
	};
}

#endif // IRIDIUM_INPUT_MOUSE_HPP_