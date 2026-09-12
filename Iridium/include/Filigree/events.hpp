#ifndef FILIGREE_EVENTS_HPP_
#define FILIGREE_EVENTS_HPP_

#include <optional>
#include <queue>

namespace filigree {
	enum class Event : unsigned char {
		DEBUG, // Testing purposes

		MOVE_TO_PARENT_FOLDER,

		MINIMIZE, // Minimize window
		EXIT, // Exit application
	};

	class EventQueue {
	public:
		void add(filigree::Event evt) {
			queue.push(evt);
		}

		[[nodiscard]] std::optional<filigree::Event> pop() {
			if (queue.size() == 0) {
				return {};
			}

			filigree::Event evt { queue.front() };
			queue.pop();
			return evt;
		}

		[[nodiscard]] bool isEmpty() { return queue.size() == 0; }

	private:
		std::queue<filigree::Event> queue;
	};
}

#endif // FILIGREE_EVENTS_HPP_