#ifndef FILIGREE_EVENTS_HPP_
#define FILIGREE_EVENTS_HPP_

#include <optional>
#include <queue>

namespace filigree {
	enum class Event : unsigned char {
		DEBUG, // Testing purposes

		SELECT_FILE,
		MOVE_TO_PARENT_FOLDER,
		CLEAR_QUEUE,

		QUEUE_FILE_PROCESSING,
		START_FILE_PROCESSING,

		HISTORY_PREV,
		HISTORY_NEXT,

		CONTEXT_MENU_OPEN,
		CONTEXT_MENU_NAVIGATE,
		CONTEXT_MENU_SET_OUTPUT,
		CONTEXT_MENU_QUEUE_FILE,
		CONTEXT_MENU_SET_FILIGREE,
		CONTEXT_MENU_SET_STAMP,
		CONTEXT_MENU_CLOSE,

		MINIMIZE, // Minimize window
		EXIT, // Exit application
	};

	class EventQueue {
	public:
		void add(filigree::Event evt) {
			if (isOpen_) {	
				queue.push(evt);
			}
		}

		[[nodiscard]] std::optional<filigree::Event> pop() {
			if (queue.size() == 0) {
				return {};
			}

			filigree::Event evt { queue.front() };
			queue.pop();
			return evt;
		}

		[[nodiscard]] bool empty() { return queue.size() == 0; }

		void setOpen(bool isOpen) { isOpen_ = isOpen; }
		[[nodiscard]] bool isOpen() { return isOpen_; }

	private:
		std::queue<filigree::Event> queue;
		bool isOpen_ { true };
	};
}

#endif // FILIGREE_EVENTS_HPP_