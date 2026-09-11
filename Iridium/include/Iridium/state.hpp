#ifndef IRIDIUM_STATE_HPP_
#define IRIDIUM_STATE_HPP_

#include <typeinfo>

#include "libraries.hpp"
#include "application_window.hpp"
#include "state_machine.hpp"

#include "detail/state.inl"

namespace ir {
	class ApplicationContext;

	/// @brief CRTP core for state classes.
	/// All states in use must derive from this one, using themselves as template argument.
	/// Ideally, no resources should be allocated during construction; everything should be done in onInitialize() instead.
	template <ir::detail::states::NoRttiReserved T>
	class StateBase {
	private:
		ir::StateMachine* stateMachine_;

	protected:
		ApplicationContext* context_;

	public:
		/// @brief Executed once upon state loading. Best used to initialize resources and set default values.
		void onInitialize() {
			if constexpr (requires() { onInitialize(); }) {
				static_cast<T*>(this)->onInitialize();
			}
		}
		
		/// @brief Executed once at the beginning of each frame. Best used to process user input.
		void onReceiveEvent(const sf::Event& event) {
			if constexpr (requires(const sf::Event& event) { onReceiveEvent(event); }) {
				static_cast<T*>(this)->onReceiveEvent(event);
			}
		}
		
		/// @brief Executed once during each frame, after onReceiveEvent(). Best used to process internal logic.
		void onUpdate() {
			if constexpr (requires() { onUpdate(); }) {
				static_cast<T*>(this)->onUpdate();
			}
		}
		
		/// @brief Executed once during each frame, after onUpdate(). Best used for on-screen rendering. Ideally, no logic should happen at all here.
		void onRender() {
			if constexpr (requires() { onRender(); }) {
				static_cast<T*>(this)->onRender();
			}
		}
		
		/// @brief Executed once upon state unloading. Best used to deallocate resources and shut down subsystems.
		void onEnd() {
			if constexpr (requires() { onEnd(); }) {
				static_cast<T*>(this)->onEnd();
			}
		}

		/// @return Internal name of this game state. Used for internal engine purposes only.
		static std::string meta_name() {
			return typeid(T).name();
		}

		/// @brief Requests state change at the end of the current frame.
		/// @tparam U Type of the state to load
		template <typename U>
		void meta_load() {
			stateMachine_->loadState<U>();
		}

		/// @brief Requests program exit at the end of the current frame.
		void meta_exit() {
			stateMachine_->requestExit();
		}

		/// @brief Links an instance of ir::StateMachine to this state instance.
		/// @attention For internal engine use only. Do not use under any circumstances.
		void meta_setStateMachine(ir::StateMachine* sm) {
			stateMachine_ = sm;
		}
	
		/// @brief Links an instance of ir::ApplicationContext to this state instance.
		/// @attention For internal engine use only. Do not use under any circumstances.
		void meta_setContext(ApplicationContext* context) {
			context_ = context;
		}
	};
}

#endif // IRIDIUM_STATE_HPP_