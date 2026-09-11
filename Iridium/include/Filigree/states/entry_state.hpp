#ifndef FILIGREE_STATE_ENTRY_HPP_
#define FILIGREE_STATE_ENTRY_HPP_

#include <state.hpp>

#include "events.hpp"
#include "gui/title_bar.hpp"

class EntryState : public ir::StateBase<EntryState> {
public:
	void onInitialize() {
		titleBar_ = std::make_unique<filigree::gui::TitleBar>(evtQueue_);
	}

	void onReceiveEvent(const sf::Event& event) {

	}

	void onUpdate() {
		if (titleBar_) {
			titleBar_->update(*context_->mouse);
		}

		processEvents();
	}

	void onRender() {
		if (titleBar_) {
			titleBar_->render(*context_->vertexRenderer);
		}
	}

	void onEnd() {

	}

	void processEvents() {
		while (!evtQueue_.isEmpty()) {
			auto evt { evtQueue_.pop() };
			if (evt.has_value()) {
				switch (evt.value()) {
					case filigree::Event::DEBUG: {
						LOG_INFO("Hello from event debug");
						break;
					}

					case filigree::Event::MINIMIZE: {
						context_->appWindow->minimize();

						break;
					}

					case filigree::Event::EXIT: {
						meta_exit();
						break;
					}
				}
			}
		}
	}

private:
	filigree::EventQueue evtQueue_;

	std::unique_ptr<filigree::gui::TitleBar> titleBar_;

};

#endif // FILIGREE_STATE_ENTRY_HPP_