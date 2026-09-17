#ifndef FILIGREE_STATE_ENTRY_HPP_
#define FILIGREE_STATE_ENTRY_HPP_

#include <state.hpp>

#include "events.hpp"
#include "gui/title_bar.hpp"
#include "gui/file_explorer.hpp"
#include "gui/settings.hpp"
#include "image_processor.hpp"

class EntryState : public ir::StateBase<EntryState> {
public:
	void onInitialize() {
		titleBar_ = std::make_unique<filigree::gui::TitleBar>(evtQueue_);
		fileExplorer_ = std::make_unique<filigree::gui::FileExplorer>(evtQueue_);
		settings_ = std::make_unique<filigree::gui::SettingsUI>(evtQueue_);
		processor_ = std::make_unique<filigree::Processor>(evtQueue_);
	}

	void onReceiveEvent(const sf::Event& event) {
		if (fileExplorer_) {
			fileExplorer_->processEvent(event);
		}
		
		if (settings_) {
			settings_->processEvent(event);
		}
	}

	void onUpdate() {
		if (titleBar_) {
			titleBar_->update(*context_->mouse);
		}

		if (fileExplorer_) {
			fileExplorer_->update(*context_->mouse);
		}

		if (settings_) {
			settings_->update(*context_->mouse);
		}

		processEvents();

		if (context_->mouse->isPressed(sf::Mouse::Button::Right)) {
			evtQueue_.add(filigree::Event::START_FILE_PROCESSING);
		}
	}

	void onRender() {
		if (titleBar_) {
			titleBar_->render(*context_->vertexRenderer);
		}
		
		if (fileExplorer_) {
			fileExplorer_->render(*context_->vertexRenderer);
		}
		
		if (settings_) {
			settings_->render(*context_->vertexRenderer);
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

					case filigree::Event::QUEUE_FILE_PROCESSING: {
						auto path { fileExplorer_->selectedPath() };
						if (path.has_value()) {
							int id { -1 };
							for (size_t i = 0; i < processingQueue_.size(); i++) {
								if (processingQueue_[i] == *path) {
									id = i;
									break;
								}
							}

							if (id == -1) {
								processingQueue_.push_back(*path);
							}
							else {
								processingQueue_.erase(processingQueue_.begin() + id);
							}
						}

						for (auto& path : processingQueue_) {
							LOG_INFO(path.string());
						}

						break;
					}

					case filigree::Event::START_FILE_PROCESSING: {
						if (processor_) {
							processor_->process(processingQueue_, std::filesystem::current_path());
						}

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
	std::unique_ptr<filigree::gui::SettingsUI> settings_;

	std::unique_ptr<filigree::Processor> processor_;

	std::vector<std::filesystem::path> processingQueue_;

};

#endif // FILIGREE_STATE_ENTRY_HPP_