#include "gui/file_explorer.hpp"

#include <filesystem>
#include <vgui/input_field.hpp>
#include <vgui/label.hpp>
#include <vgui/icon.hpp>
#include <vgui/factory.hpp>

namespace filigree::gui {
	namespace {
		auto isImage = [](std::filesystem::path file) {
			if (std::filesystem::is_directory(file)) {
				return false;
			}
			std::string ext { file.extension().string() };
			std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
			return ext == ".png" || ext == ".jpg" || ext == ".bmp";
		};
	}

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
		if (fileExplorer_) {
			auto buttonParent { fileExplorer_->getChild("PathBar")->getChild<ir::vgui::FramedElement>("ButtonParent") };
			if (buttonParent) {
				if (activeDir_.has_relative_path()) {
					buttonParent->setColors(sf::Color(64u, 224u, 128u), sf::Color(64u, 224u, 128u, 32u));
					buttonParent->getChild<ir::vgui::Icon>("Icon")->setFrameColor(sf::Color(64u, 224u, 128u));
				}
				else {
					buttonParent->setColors(sf::Color(128u, 128u, 128u), sf::Color(128u, 128u, 128u, 32u));
					buttonParent->getChild<ir::vgui::Icon>("Icon")->setFrameColor(sf::Color(128u, 128u, 128u));
				}
			}
		}

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
	}

	void FileExplorer::render(ir::render::VertexRenderer& renderer) const {
		if (fileExplorer_) {
			fileExplorer_->render(renderer);
		}
		if (selectionRect_) {
			selectionRect_->render(renderer);
		}
	}

	void FileExplorer::moveToParent() {
		if (activeDir_.has_relative_path()) {
			setPath(activeDir_.parent_path());
		}
	}

	void FileExplorer::setPath(std::filesystem::path path) {
		activeDir_ = path;

		auto label { fileExplorer_->getChild("PathBar")->getChild<ir::vgui::Label>("PathLabel") };
		if (label) {
			label->setLabel(concisePath(activeDir_));
		}

		listOffset_ = 0;
		populateFileList();
		updateFileList();
	}

	void FileExplorer::processFileSelection() {
		if (selectedPath_.has_value()) {
			if (std::filesystem::is_directory(*selectedPath_)) {
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
		auto bar { std::make_unique<ir::vgui::FramedElement>() };
		bar->setColors(sf::Color::White, sf::Color::Transparent)
			.setPosition(ir::Vector { 0.f, 0.f })
			.setSize(ir::Vector { 480.f, 30.f });
		
		auto pathLabel { std::make_unique<ir::vgui::Label>(concisePath(activeDir_)) };
		pathLabel->setPosition(ir::Vector { 6.f, 6.f });
		
		/// TO BE IMPLEMENTED LATER
		auto buttonPrev { ir::vgui::makeIconButton(ir::Vector { 26.f, 26.f }, "arrow_left", sf::Color(128u, 128u, 128u)) };
		buttonPrev->setPosition(ir::Vector { 396.f, 2.f })
			.registerClickEvent([&]() { evtQueue_->add(filigree::Event::DEBUG); });

		/// TO BE IMPLEMENTED LATER
		auto buttonNext { ir::vgui::makeIconButton(ir::Vector { 26.f, 26.f }, "arrow_right", sf::Color(128u, 128u, 128u)) };
		buttonNext->setPosition(ir::Vector { 424.f, 2.f })
			.registerClickEvent([&]() { evtQueue_->add(filigree::Event::DEBUG); });

		auto buttonParent { ir::vgui::makeIconButton(ir::Vector { 26.f, 26.f }, "folder_exit", sf::Color(64u, 224u, 128u)) };
		buttonParent->setPosition(ir::Vector { 452.f, 2.f })
			.registerClickEvent([&]() { evtQueue_->add(filigree::Event::MOVE_TO_PARENT_FOLDER); });
		
		bar->addChildElement("PathLabel", std::move(pathLabel));
		bar->addChildElement("ButtonPrev", std::move(buttonPrev));
		bar->addChildElement("ButtonNext", std::move(buttonNext));
		bar->addChildElement("ButtonParent", std::move(buttonParent));

		fileExplorer_->addChildElement("PathBar", std::move(bar));
	}

	void FileExplorer::createFileFields() {
		for (size_t i = 0; i < 22; i++) { /// 690 px / 30 px per field
			auto el { std::make_unique<ir::vgui::FramedElement>() };
			el->setPosition(ir::Vector { 0.f, static_cast<float>(i + 1) * 30.f })
				.setSize(ir::Vector { 480.f, 30.f })
				.setColors(sf::Color { 0u, 128u, 255u, 32u }, i % 2 ? sf::Color::Transparent :  sf::Color { 255u, 255u, 255u, 8u });

			auto label { std::make_unique<ir::vgui::Label>() };
			label->setPosition(ir::Vector { 6.f, 5.f });

			el->addChildElement("Label", std::move(label));

			el->registerClickEvent([&]() { evtQueue_->add(filigree::Event::SELECT_FILE); });
			
			fileExplorer_->addChildElement(std::string { "Field" } + std::to_string(i), std::move(el));
		}
	}

	std::string FileExplorer::concisePath(std::filesystem::path& path) {
		constexpr static std::string separator { " / " };
		std::string concise { };

		auto nameOrRoot = [] (std::filesystem::path path) {
			if (path.has_relative_path()) {
				return path.relative_path().filename().string();
			}
			else {
				return path.root_name().string();
			}
		};

		if (path.parent_path().parent_path().has_relative_path()) {
			concise += "..." + separator;
		}
		if (path.parent_path().has_relative_path()) {
			concise += nameOrRoot(path.parent_path().parent_path()) + separator;
		}
		if (path.has_relative_path()) {
			concise += nameOrRoot(path.parent_path()) + separator;
		}
		concise += nameOrRoot(path) + separator;

		return concise;
	}
}