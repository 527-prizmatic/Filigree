#ifndef IRIDIUM_INPUT_KEYBOARD_HPP_
#define IRIDIUM_INPUT_KEYBOARD_HPP_

#include "libraries.hpp"
#include "input/state.hpp"
#include "application_window.hpp"

#include <unordered_map>

namespace ir::input {
	struct Keybind {
		sf::Keyboard::Key key;
		ir::input::State state;
	};

	class Keyboard {
	public:
		void update(ir::ApplicationWindow& window);

		bool registerKeybind(std::string name, sf::Keyboard::Key key);
		bool remapKeybind(std::string name, sf::Keyboard::Key key);
		bool removeKeybind(std::string name);
		[[nodiscard]] bool keybindExists(std::string name) const;

		[[nodiscard]] bool isIdle(std::string name) const;
		[[nodiscard]] bool isPressed(std::string name) const;
		[[nodiscard]] bool isActive(std::string name) const;
		[[nodiscard]] bool isReleased(std::string name) const;
		[[nodiscard]] std::optional<ir::input::State> getState(std::string name) const;

	private:

		std::unordered_map<std::string, ir::input::Keybind> states_ {};
	};
}

#endif // IRIDIUM_INPUT_KEYBOARD_HPP_