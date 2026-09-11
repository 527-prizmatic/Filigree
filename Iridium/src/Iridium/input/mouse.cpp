#include "input/mouse.hpp"

namespace ir::input {
	void Mouse::update(ir::ApplicationWindow& window) {
		cursorPosition_ = ir::Vector::fromSFMLVector(window.getMouseCursorPosition());

		bool focus = window.hasFocus();
		for (unsigned int btn = 0; btn <  states_.size(); btn++) {
			bool clicked = focus && sf::Mouse::isButtonPressed(static_cast<sf::Mouse::Button>(btn));

			if (clicked) {
				if (states_[btn] == State::IDLE) {
					states_[btn] = State::PRESSED;
				} else {
					states_[btn] = State::ACTIVE;
				}
			} else {
				if (states_[btn] == State::ACTIVE) {
					states_[btn] = State::RELEASED;
				} else {
					states_[btn] = State::IDLE;
				}
			}
		}
	}

	bool Mouse::isIdle(sf::Mouse::Button button) const {
		return states_[static_cast<unsigned int>(button)] == State::IDLE;
	}

	bool Mouse::isPressed(sf::Mouse::Button button) const {
		return states_[static_cast<unsigned int>(button)] == State::PRESSED;
	}

	bool Mouse::isActive(sf::Mouse::Button button) const {
		return  states_[static_cast<unsigned int>(button)] == State::ACTIVE;
	}

	bool Mouse::isReleased(sf::Mouse::Button button) const {
		return  states_[static_cast<unsigned int>(button)] == State::RELEASED;
	}

	ir::input::State Mouse::getState(sf::Mouse::Button button) const {
		return  states_[static_cast<unsigned int>(button)];
	}

	bool Mouse::isWithinArea(ir::Vector topLeft, ir::Vector size) const {
		sf::IntRect area {sf::Vector2i{topLeft}, sf::Vector2i{size}};
		return area.contains(sf::Vector2i{cursorPosition_});
	}

	[[nodiscard]] ir::Vector Mouse::cursorPosition() const {
		return cursorPosition_;
	}
}