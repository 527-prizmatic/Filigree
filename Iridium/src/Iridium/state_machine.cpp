#include "state_machine.hpp"
#include "state.hpp"
#include "exceptions.hpp"
#include "application.hpp"

namespace ir {
	namespace detail {
		const std::string g_nullStateName = "__none";
	}

	StateMachine::StateMachine() {
		currentState_ = ir::detail::g_nullStateName;
	}

	StateMachine::~StateMachine() = default;

	bool StateMachine::loadState(std::string _name) {
		if (availableStates_.contains(_name)) {
			nextState_ = _name;
			return true;
		} else {
			return false;
		}
	}

	void StateMachine::initialize() {
		if (nextState_.has_value()) {
			if (currentState_ != ir::detail::g_nullStateName) {
				unload();
			}
			currentState_ = nextState_.value();
			availableStates_.at(currentState_)->onInitialize();
			nextState_.reset();
		}
	}
	
	void StateMachine::handleEvents() {
		expectValidID();

		while (const std::optional event = context_->appWindow->pollNextEvent()) {
			availableStates_.at(currentState_)->onReceiveEvent(event.value());
		}
	}
	
	void StateMachine::update() {
		expectValidID();
		availableStates_.at(currentState_)->onUpdate();
	}
	
	void StateMachine::render() {
		expectValidID();

		context_->appWindow->clear(sf::Color::Black);
		availableStates_.at(currentState_)->onRender();
		context_->appWindow->flush();
	}
	
	void StateMachine::unload() {
		expectValidID();
		availableStates_.at(currentState_)->onEnd();
	}

	void StateMachine::expectValidID() {
		if (!availableStates_.contains(currentState_)) {
			throw ir::Exceptions::BadStateID(currentState_);
		}
	}

	void StateMachine::registerContext(ApplicationContext* context) {
		this->context_ = context;
	}
}