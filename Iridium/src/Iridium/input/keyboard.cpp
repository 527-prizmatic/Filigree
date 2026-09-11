#include "input/keyboard.hpp"

namespace ir::input {
	void Keyboard::update(ir::ApplicationWindow& window) {
		bool focus = window.hasFocus();
		
		for (auto& [key, kb] : states_) {
			bool pressed = focus && sf::Keyboard::isKeyPressed(kb.key);

			if (pressed) {
				if (kb.state == State::IDLE) {
					kb.state = State::PRESSED;
				}
				else {
					kb.state = State::ACTIVE;
				}
			}
			else {
				if (kb.state == State::ACTIVE) {
					kb.state = State::RELEASED;
				}
				else {
					kb.state = State::IDLE;
				}
			}
		}
	}

	bool Keyboard::registerKeybind(std::string name, sf::Keyboard::Key key) {
		if (keybindExists(name)) {
			// Error
			LOG_WARN(std::string { "Error registering keybind: keybind " } + name + std::string { "already exists!" });
			return false;
		}

		states_[name] = ir::input::Keybind { key, ir::input::State::IDLE };
		return true;
	}

	bool Keyboard::remapKeybind(std::string name, sf::Keyboard::Key key) {
		if (!keybindExists(name)) {
			// Error
			LOG_WARN(std::string { "Error remapping keybind: keybind " } + name + std::string { "does not exist!" });
			return false;
		}

		states_[name].key = key;
		return true;
	}

	bool Keyboard::removeKeybind(std::string name) {
		if (!keybindExists(name)) {
			// Error
			LOG_WARN(std::string { "Error removing keybind: keybind " } + name + std::string { "does not exist!" });
			return false;
		}

		states_.erase(name);
		return true;
	}

	bool Keyboard::keybindExists(std::string name) const { return states_.contains(name); }

	bool Keyboard::isIdle(std::string name) const {
		if (!keybindExists(name)) {
			// Error
			return false;
		}

		return states_.at(name).state == State::IDLE;
	}

	bool Keyboard::isPressed(std::string name) const {
		if (!keybindExists(name)) {
			// Error
			return false;
		}

		return states_.at(name).state == State::PRESSED;
	}

	bool Keyboard::isActive(std::string name) const {
		if (!keybindExists(name)) {
			// Error
			return false;
		}

		return states_.at(name).state == State::ACTIVE;
	}

	bool Keyboard::isReleased(std::string name) const {
		if (!keybindExists(name)) {
			// Error
			return false;
		}

		return states_.at(name).state == State::RELEASED;
	}

	std::optional<ir::input::State> Keyboard::getState(std::string name) const {
		if (!keybindExists(name)) {
			// Error
			return {};
		}

		return states_.at(name).state;
	}
}