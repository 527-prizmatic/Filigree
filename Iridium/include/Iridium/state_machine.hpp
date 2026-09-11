#ifndef IRIDIUM_STATEMACHINE_HPP_
#define IRIDIUM_STATEMACHINE_HPP_

#include "libraries.hpp"

namespace ir {
	namespace detail {
		class State;
	}
	class ApplicationWindow;
	class ApplicationContext;

	/// @brief Verifies that the type is a correctly implemented derivative of Ir::State.
	template <typename T>
	concept HasMethodMetaName = requires {
		{ T::meta_name() } -> std::same_as<std::string>;
	};

	class StateMachine {
	public:
		StateMachine();
		~StateMachine();

		/// @brief Queues loading of the state with the given name, to be loaded upon the start of the next frame.
		/// @return Whether the state was successfully queued
		/// @deprecated Cumbersome and error-prone due to subtleties of std::type_info.
		/// Use the templated version instead.
		[[deprecated]] bool loadState(std::string name);

		/// @brief Queues loading of the state with the given type, to be loaded upon the start of the next frame.
		/// @brief If the state did not exist already, it is registered, allowing to perform lazy initialization if the user wishes.
		template <HasMethodMetaName T> void loadState() {
			registerState<T>();
			nextState_ = T::meta_name();
		}

		/// @brief Performs initialization for the currently active state.
		void initialize();

		/// @brief Collects events for the given application window, and sends them to the currently active state for processing.
		void handleEvents();

		/// @brief Performs frame updates for the currently active state.
		void update();

		/// @brief Performs rendering operations for the currently active state.
		void render();

		/// @brief Calls resource destruction functions for the currently active state.
		void unload();

		/// @brief Registers a state of the given type into the state machine.
		template <HasMethodMetaName T>
		bool registerState() {
			if (availableStates_.contains(T::meta_name())) {
				return false;
			}
			
			availableStates_.emplace(T::meta_name(), std::make_unique<ir::detail::State>(T()));
			availableStates_.at(T::meta_name())->meta_setStateMachine(this);
			availableStates_.at(T::meta_name())->meta_setContext(context_);
			LOG_INFO("Registered state " + T::meta_name());
			return true;
		}

		/// @return type_info name of the currently active state.
		/// @note A return value of "__none" means the state machine was not initialized yet.
		/// @attention type_info slightly obfuscates type names, pay attention to this if doing manual type name comparison.
		/// Prefer using getStateName() instead of writing type names manually in such comparisons.
		std::string getCurrentStateName() {
			return currentState_;
		}

		void requestExit() { requestedExit_ = true; }
		bool hasRequestedExit() { return requestedExit_; }

		void registerContext(ApplicationContext* context);

	private:
		std::unordered_map<std::string, std::unique_ptr<ir::detail::State>> availableStates_; ///< Storage for available states
		std::string currentState_; ///< Name of the currently active state
		std::optional<std::string> nextState_; ///< Name of the state to be loaded upon starting the next frame

		bool requestedExit_ { false };
		void expectValidID();

		ApplicationContext* context_;
	};
}

#endif // IRIDIUM_STATEMACHINE_HPP_