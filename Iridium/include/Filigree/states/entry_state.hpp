#ifndef FILIGREE_STATE_ENTRY_HPP_
#define FILIGREE_STATE_ENTRY_HPP_

#include <thread>

#include <state.hpp>
#include <vgui/label.hpp>

#include "events.hpp"
#include "gui/title_bar.hpp"
#include "gui/file_explorer.hpp"
#include "gui/settings.hpp"
#include "gui/file_queue.hpp"
#include "gui/image_preview.hpp"
#include "gui/context_menu.hpp"
#include "image_processor.hpp"

class EntryState : public ir::StateBase<EntryState> {
public:
	void onInitialize() {
		context_->appWindow->setTitle("Filigree -- Image Watermarker");

		ir::vgui::Checkbox::setDefaultSize(ir::Vector { 24.f, 24.f });
		ir::vgui::Label::setDefaultScale(15.f);

		titleBar_ = std::make_unique<filigree::gui::TitleBar>(evtQueue_);
		fileExplorer_ = std::make_unique<filigree::gui::FileExplorer>(evtQueue_);
		settings_ = std::make_unique<filigree::gui::SettingsUI>(evtQueue_);
		fileQueue_ = std::make_unique<filigree::gui::FileQueue>(evtQueue_);
		preview_ = std::make_unique<filigree::gui::ImagePreview>(evtQueue_);
		ctxMenu_ = std::make_unique<filigree::gui::ContextMenu>(evtQueue_);

		processor_ = std::make_unique<filigree::Processor>(evtQueue_, &*preview_);

		fileQueue_->setWatchedQueue(&processingQueue_);
	}

	void onReceiveEvent(const sf::Event& event) {
		if (ctxMenu_->active()) {
			ctxMenu_->processEvent(event, *context_->mouse);
		}
		else {
			if (fileExplorer_) {
				fileExplorer_->processEvent(event, *context_->mouse);
			}
			
			if (settings_) {
				settings_->processEvent(event);
			}

			if (fileQueue_) {
				fileQueue_->processEvent(event, *context_->mouse);
			}
		}
	}

	void onUpdate() {
		if (titleBar_) {
			titleBar_->update(*context_->mouse);
		}

		
		if (ctxMenu_->active()) {
			ctxMenu_->update(*context_->mouse);
		}
		else {
			if (fileExplorer_) {
				fileExplorer_->update(*context_->mouse);
			}

			if (settings_) {
				settings_->update(*context_->mouse);
			}

			if (fileQueue_) {
				fileQueue_->update(*context_->mouse);
			}
		}

		if (preview_) {
			preview_->updateSpinnerAngle(context_->deltaTime());
			preview_->update(*context_->mouse);
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
		
		if (settings_) {
			settings_->render(*context_->vertexRenderer);
		}
		
		if (fileQueue_) {
			fileQueue_->render(*context_->vertexRenderer);
		}

		if (preview_) {
			preview_->render(*context_->vertexRenderer);
		}

		if (ctxMenu_->active()) {
			ctxMenu_->render(*context_->vertexRenderer);
		}
	}

	void onEnd() {

	}

	void processEvents() {
		auto queueFileProcessing = [&](std::filesystem::path path) {
			/// Add if not already present
			if (std::find(processingQueue_.begin(), processingQueue_.end(), path) == processingQueue_.end()) {
				processingQueue_.push_back(path);
			}
			
			/// Sort paths alphabetically regardless of case
			std::sort(processingQueue_.begin(), processingQueue_.end(),
				[](const std::filesystem::path& a, const std::filesystem::path& b) {
					std::string aLower { a.filename().string() };
					std::transform(aLower.begin(), aLower.end(), aLower.begin(), ::tolower);
					
					std::string bLower { b.filename().string() };
					std::transform(bLower.begin(), bLower.end(), bLower.begin(), ::tolower);
					
					return aLower < bLower;
				}
			);
			
			/// Update file queue display
			fileQueue_->updateFileList();
		};

		while (!evtQueue_.empty()) {
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

					case filigree::Event::CLEAR_QUEUE: {
						LOG_INFO("Clearing processing queue");
						processingQueue_.clear();
						break;
					}

					case filigree::Event::QUEUE_FILE_PROCESSING: {
						auto path { fileExplorer_->selectedPath() };
						
						if (path.has_value()) {
							queueFileProcessing(*path);
						}

						break;
					}

					case filigree::Event::START_FILE_PROCESSING: {
						if (!evtQueue_.isOpen()) {
							LOG_WARN("Cannot start processing while another processing task is running");
							break;
						}

						if (processor_) {
							auto sentQueue { processingQueue_ };
							std::jthread thr([&, sentQueue]() {
								evtQueue_.setOpen(false);
								preview_->setProgressBarVisibility(true);
								preview_->setProgressBarCount(sentQueue.size());
								preview_->setProgressBarStatus(0);
								try {
									processor_->loadSettings(settings_->assembleSettings());
									processor_->process(sentQueue);
								}
								catch (...) {
									LOG_ERROR("Unspecified error during image processing");
								}
								evtQueue_.setOpen(true);
								preview_->setProgressBarVisibility(false);
							});

							thr.detach();
							processingQueue_.clear(); ///< Preparing for the next round
						}

						fileQueue_->updateFileList();
						break;
					}

					case filigree::Event::HISTORY_PREV: {
						fileExplorer_->moveToHistoryPrev();
						break;
					}

					case filigree::Event::HISTORY_NEXT: {
						fileExplorer_->moveToHistoryNext();
						break;
					}

					case filigree::Event::CONTEXT_MENU_OPEN: {
						if (fileExplorer_->selectedPath().has_value()) {
							ctxMenu_->create(fileExplorer_->selectedPath().value(), context_->mouse->cursorPosition());
						}
						break;
					}

					case filigree::Event::CONTEXT_MENU_CLOSE: {
						ctxMenu_->close();
						break;
					}

					case filigree::Event::CONTEXT_MENU_NAVIGATE: {
						if (std::filesystem::is_directory(ctxMenu_->path())) {
							fileExplorer_->setPath(ctxMenu_->path());
						}
						ctxMenu_->close();
						break;
					}

					case filigree::Event::CONTEXT_MENU_SET_OUTPUT: {
						settings_->setOutputPath(ctxMenu_->path());
						ctxMenu_->close();
						break;
					}

					case filigree::Event::CONTEXT_MENU_QUEUE_FILE: {
						queueFileProcessing(ctxMenu_->path());
						ctxMenu_->close();
						break;
					}

					case filigree::Event::CONTEXT_MENU_SET_FILIGREE: {
						settings_->setFiligreePath(ctxMenu_->path());
						ctxMenu_->close();
						break;
					}

					case filigree::Event::CONTEXT_MENU_SET_STAMP: {
						settings_->setStampPath(ctxMenu_->path());
						ctxMenu_->close();
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
	std::unique_ptr<filigree::gui::FileQueue> fileQueue_;
	std::unique_ptr<filigree::gui::ImagePreview> preview_;
	std::unique_ptr<filigree::gui::ContextMenu> ctxMenu_;

	std::unique_ptr<filigree::Processor> processor_;

	std::vector<std::filesystem::path> processingQueue_;
};

#endif // FILIGREE_STATE_ENTRY_HPP_