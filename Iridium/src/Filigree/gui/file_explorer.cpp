#include "gui/file_explorer.hpp"

#include <filesystem>
#include <vgui/input_field.hpp>
#include <vgui/label.hpp>
#include <vgui/icon.hpp>
#include <vgui/factory.hpp>

#include "filepath_funcs.hpp"

namespace filigree::gui {
	FileExplorer::FileExplorer(filigree::EventQueue& evtQueue) {
		evtQueue_ = &evtQueue;
		activeDir_ = std::filesystem::current_path();

		fileExplorer_ = std::make_unique<ir::vgui::FramedElement>();
		if (!fileExplorer_) {
			LOG_ERROR("Could not create file explorer!!");
		}
		else {
			fileExplorer_->setColors(sf::Color::White, sf::Color { 0u, 8u, 16u })
				.setSize(ir::Vector { 480.f, 689.f })
				.setPosition(ir::Vector { 1.f, 30.f });

			createPathBar();
			createFileFields();

			populateFileList();
			updateFileList();
		}

		selectionRect_ = std::make_unique<ir::render::Rectangle>();
		selectionRect_->setSize(ir::Vector { 475.f, 26.f })
			.setPosition(ir::Vector { 0.f, 60.f })
			.setColor(sf::Color::Transparent)
			.setMode(ir::render::Mode::WIREFRAME);
	}

	void FileExplorer::processEvent(const sf::Event& evt, ir::input::Mouse& mouseInput) {
		if (fileExplorer_) {
			
			if (mouseInput.cursorPosition().x < fileExplorer_->absolutePosition().x
				|| mouseInput.cursorPosition().x > fileExplorer_->absolutePosition().x + fileExplorer_->size().x
				|| mouseInput.cursorPosition().y < fileExplorer_->absolutePosition().y
				|| mouseInput.cursorPosition().y > fileExplorer_->absolutePosition().y + fileExplorer_->size().y) {
				return;
			}

			fileExplorer_->processEvent(evt);
		}

		if (auto scroll = evt.getIf<sf::Event::MouseWheelScrolled>()) {
			listOffset_ -= scroll->delta;
			if (listOffset_ < 0 || pathList_.size() <= 22) {
				listOffset_ = 0;
			}
			else if (listOffset_ > static_cast<int>(pathList_.size() - 22)) {
				listOffset_ = pathList_.size() - 22;
			}
			updateFileList();
		}
	}

	void FileExplorer::update(ir::input::Mouse& mouseInput) {
		auto recolorTitleBarButton = [&](std::string name, bool condition, sf::Color clrTrue, sf::Color clrFalse) {
			if (fileExplorer_) {
				auto buttonParent { fileExplorer_->getChild("PathBar")->getChild<ir::vgui::FramedElement>(name) };
				if (buttonParent) {
					if (condition) {
						buttonParent->setColors(clrTrue, sf::Color(clrTrue.r, clrTrue.g, clrTrue.b, 32u));
						buttonParent->getChild<ir::vgui::Icon>("Icon")->setFrameColor(clrTrue);
					}
					else {
						buttonParent->setColors(clrFalse, sf::Color(clrFalse.r, clrFalse.g, clrFalse.b, 32u));
						buttonParent->getChild<ir::vgui::Icon>("Icon")->setFrameColor(clrFalse);
					}
				}
			}
		};

		recolorTitleBarButton("ButtonPrev", historyPrev_.size() != 0, sf::Color(64u, 224u, 128u), sf::Color(128u, 128u, 128u));
		recolorTitleBarButton("ButtonNext", historyNext_.size() != 0, sf::Color(64u, 224u, 128u), sf::Color(128u, 128u, 128u));
		recolorTitleBarButton("ButtonParent", activeDir_.has_relative_path(), sf::Color(64u, 224u, 128u), sf::Color(128u, 128u, 128u));

		if (fileExplorer_->update(mouseInput)) {
			int id { static_cast<int>((mouseInput.cursorPosition().y - fileExplorer_->position().y) / 30.f) - 1 };
			id += listOffset_;
			if (pathList_.size() > static_cast<unsigned int>(id)) {
				selectedPath_ = pathList_[id];

				selectionRect_->setColor(sf::Color {64u, 160u, 255u })
					.setPosition(fileExplorer_->position() + ir::Vector { 2.f, static_cast<float>(id - listOffset_ + 1) * 30.f + 2.f });
			}
			else {
				selectedPath_ = {};
				selectionRect_->setColor(sf::Color::Transparent);
			}
		}
		else {
			selectedPath_ = {};
			selectionRect_->setColor(sf::Color::Transparent);
		}

		if (mouseInput.isPressed(sf::Mouse::Button::Right)) {
			evtQueue_->add(filigree::Event::CONTEXT_MENU_OPEN);
		}
	}

	void FileExplorer::render(ir::render::VertexRenderer& renderer) const {
		if (fileExplorer_) {
			fileExplorer_->render(renderer);
		}
		if (selectionRect_) {
			selectionRect_->render(renderer);
		}
	}

	void FileExplorer::moveTo(std::filesystem::path path) {
		clearHistoryNext();
		historyPrev_.push_front(activeDir_);
		setPath(path);
	}

	void FileExplorer::moveToParent() {
		if (activeDir_.has_relative_path()) {
			moveTo(activeDir_.parent_path());
		}
	}

	void FileExplorer::moveToHistoryPrev() {
		if (historyPrev_.size() == 0) {
			return;
		}
		auto path { historyPrev_.front() };
		historyPrev_.pop_front();
		historyNext_.push_front(activeDir_);
		setPath(path);
	}

	void FileExplorer::moveToHistoryNext() {
		if (historyNext_.size() == 0) {
			return;
		}

		auto path { historyNext_.front() };
		historyNext_.pop_front();
		historyPrev_.push_front(activeDir_);
		setPath(path);
	}

	void FileExplorer::clearHistoryNext() {
		historyNext_.clear();
	}

	void FileExplorer::setPath(std::filesystem::path path) {
		activeDir_ = path;

		auto label { fileExplorer_->getChild("PathBar")->getChild<ir::vgui::Label>("LabelPath") };
		if (label) {
			label->setLabel(filigree::shortenPath(activeDir_, 2));
		}

		listOffset_ = 0;
		populateFileList();
		updateFileList();
		
		LOG_INFO("prev");
		std::for_each(historyPrev_.begin(), historyPrev_.end(), [&](std::filesystem::path& p) { LOG_INFO(p.string()); });
		LOG_INFO("next");
		std::for_each(historyNext_.begin(), historyNext_.end(), [&](std::filesystem::path& p) { LOG_INFO(p.string()); });
	}

	void FileExplorer::processFileSelection() {
		if (selectedPath_.has_value()) {
			if (std::filesystem::is_directory(*selectedPath_)) {
				clearHistoryNext();
				historyPrev_.push_front(activeDir_);
				setPath(*selectedPath_);
			}
			else if (isImage(*selectedPath_)) {
				evtQueue_->add(filigree::Event::QUEUE_FILE_PROCESSING);
			}
		}
	}

	void FileExplorer::populateFileList() {
		pathList_.clear();

		for (auto file : std::filesystem::directory_iterator { activeDir_ }) {
			if (std::filesystem::is_directory(file)) {
				pathList_.push_back(file);
			}
		}
		
		for (auto file : std::filesystem::directory_iterator { activeDir_ }) {
			if (!std::filesystem::is_directory(file) && isImage(file)) {
				pathList_.push_back(file);
			}
		}
		
		for (auto file : std::filesystem::directory_iterator { activeDir_ }) {
			if (!std::filesystem::is_directory(file) && !isImage(file)) {
				pathList_.push_back(file);
			}
		}
	}

	void FileExplorer::updateFileList() {
		for (size_t i = 0; i < 22; i++) { /// 690 px / 30 px per field
			auto field { fileExplorer_->getChild(std::string { "Field" } + std::to_string(i))->getChild<ir::vgui::Label>("Label") };
			if (i + listOffset_ < pathList_.size()) {
				auto path { pathList_[i + listOffset_] };
				field->setLabel(path.filename().string());

				if (std::filesystem::is_directory(path)) {
					field->setColor(sf::Color { 128u, 160u, 224u });
				}
				else if (isImage(path)) {
					field->setColor(sf::Color::White);
				}
				else {
					field->setColor(sf::Color { 128u, 128u, 128u });
				}
			}
			else {
				field->setLabel("");
			}
		}
	}

	void FileExplorer::createPathBar() {
		/// Parent element
		auto bar { fileExplorer_->addChildElement<ir::vgui::FramedElement>("PathBar") };
		bar->setColors(sf::Color::White, sf::Color::Transparent)
			.setPosition(ir::Vector { 0.f, 0.f })
			.setSize(ir::Vector { 480.f, 30.f });

		/// Window title
		auto labelPath { bar->addChildElement<ir::vgui::Label>("LabelPath", filigree::shortenPath(activeDir_, 2)) };
		labelPath->setPosition(ir::Vector { 6.f, 6.f });
		
		/// Navigation buttons (still use the old nesting system for now, due to makeIconButton limitations)
		auto buttonPrev { ir::vgui::makeIconButton(ir::Vector { 26.f, 26.f }, "arrow_left", sf::Color(128u, 128u, 128u)) };
		buttonPrev->setPosition(ir::Vector { 396.f, 2.f })
			.registerClickEvent([&]() { evtQueue_->add(filigree::Event::HISTORY_PREV); });

		auto buttonNext { ir::vgui::makeIconButton(ir::Vector { 26.f, 26.f }, "arrow_right", sf::Color(128u, 128u, 128u)) };
		buttonNext->setPosition(ir::Vector { 424.f, 2.f })
			.registerClickEvent([&]() { evtQueue_->add(filigree::Event::HISTORY_NEXT); });

		auto buttonParent { ir::vgui::makeIconButton(ir::Vector { 26.f, 26.f }, "folder_exit", sf::Color(64u, 224u, 128u)) };
		buttonParent->setPosition(ir::Vector { 452.f, 2.f })
			.registerClickEvent([&]() { evtQueue_->add(filigree::Event::MOVE_TO_PARENT_FOLDER); });
		
		bar->setChildElement("ButtonPrev", std::move(buttonPrev));
		bar->setChildElement("ButtonNext", std::move(buttonNext));
		bar->setChildElement("ButtonParent", std::move(buttonParent));
	}

	void FileExplorer::createFileFields() {
		for (size_t i = 0; i < 22; i++) { /// 690 px / 30 px per field
			auto el { fileExplorer_->addChildElement<ir::vgui::FramedElement>(std::string { "Field" } + std::to_string(i)) };
			el->setPosition(ir::Vector { 0.f, static_cast<float>(i + 1) * 30.f })
				.setSize(ir::Vector { 480.f, 30.f })
				.setColors(sf::Color { 0u, 128u, 255u, 32u }, i % 2 ? sf::Color::Transparent :  sf::Color { 255u, 255u, 255u, 8u })
				.registerClickEvent([&]() { evtQueue_->add(filigree::Event::SELECT_FILE); });;

			auto label { el->addChildElement<ir::vgui::Label>("Label", "") };
			label->setPosition(ir::Vector { 6.f, 5.f });
		}
	}
}