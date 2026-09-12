#ifndef FILIGREE_STATE_ENTRY_HPP_
#define FILIGREE_STATE_ENTRY_HPP_

#include <state.hpp>

#include "events.hpp"
#include "gui/title_bar.hpp"
#include "gui/file_explorer.hpp"

class EntryState : public ir::StateBase<EntryState> {
public:
	void onInitialize() {
		titleBar_ = std::make_unique<filigree::gui::TitleBar>(evtQueue_);
		fileExplorer_ = std::make_unique<filigree::gui::FileExplorer>(evtQueue_);
	}

	void onReceiveEvent(const sf::Event& event) {
		if (fileExplorer_) {
			fileExplorer_->processEvent(event);
		}
	}

	void onUpdate() {
		if (titleBar_) {
			titleBar_->update(*context_->mouse);
		}

		if (fileExplorer_) {
			fileExplorer_->update(*context_->mouse);
		}

		processEvents();
	}

	void onRender() {
		if (titleBar_) {
			titleBar_->render(*context_->vertexRenderer);
		}
		
		if (fileExplorer_) {
			fileExplorer_->render(*context_->vertexRenderer);
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

					case filigree::Event::SELECT_FILE: {
						fileExplorer_->processFileSelection();

						break;
					}

					case filigree::Event::MOVE_TO_PARENT_FOLDER: {
						LOG_INFO("Moving to parent folder");
						fileExplorer_->moveToParent();
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
	std::unique_ptr<filigree::gui::FileExplorer> fileExplorer_;

};

#endif // FILIGREE_STATE_ENTRY_HPP_