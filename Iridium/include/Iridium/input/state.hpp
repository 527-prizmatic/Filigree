#ifndef IRIDIUM_INPUT_STATE_HPP_
#define IRIDIUM_INPUT_STATE_HPP_

namespace ir::input {
	enum State : unsigned char {
		IDLE = 0,
		PRESSED = 1,
		ACTIVE = 2,
		RELEASED = 3
	};
}

#endif // IRIDIUM_INPUT_STATE_HPP_