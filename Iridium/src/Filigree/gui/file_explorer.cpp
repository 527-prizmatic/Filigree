#include "gui/file_explorer.hpp"

#include <filesystem>
#include <vgui/input_field.hpp>
#include <vgui/label.hpp>
#include <vgui/icon.hpp>
#include <vgui/factory.hpp>

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
				.setSize(ir::Vector { 479.f, 689.f })
				.setPosition(ir::Vector { 1.f, 30.f });

	//		createMinimizeButton();
	//		createExitButton();
	//		createTitle();
			createPathBar();
		}
	}

	void FileExplorer::processEvent(const sf::Event& evt) {
		if (fileExplorer_) {
			fileExplorer_->processEvent(evt);
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

			fileExplorer_->update(mouseInput);
		}
	}

	void FileExplorer::render(ir::render::VertexRenderer& renderer) const {
		if (fileExplorer_) {
			fileExplorer_->render(renderer);
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
	}

	void FileExplorer::createPathBar() {
		auto bar { std::make_unique<ir::vgui::FramedElement>() };
		bar->setColors(sf::Color::White, sf::Color::Transparent)
			.setPosition(ir::Vector { 0.f, 0.f })
			.setSize(ir::Vector { 479.f, 30.f });
		
		auto pathLabel { std::make_unique<ir::vgui::Label>(concisePath(activeDir_)) };
		pathLabel->setScale(15.f)
			.setPosition(ir::Vector { 6.f, 6.f });
		
		/// TO BE IMPLEMENTED LATER
		auto buttonPrev { ir::vgui::makeIconButton(ir::Vector { 26.f, 26.f }, "arrow_left", sf::Color(128u, 128u, 128u)) };
		buttonPrev->setPosition(ir::Vector { 395.f, 2.f })
			.registerClickEvent([&]() { evtQueue_->add(filigree::Event::EXIT); });

		/// TO BE IMPLEMENTED LATER
		auto buttonNext { ir::vgui::makeIconButton(ir::Vector { 26.f, 26.f }, "arrow_right", sf::Color(128u, 128u, 128u)) };
		buttonNext->setPosition(ir::Vector { 423.f, 2.f })
			.registerClickEvent([&]() { evtQueue_->add(filigree::Event::EXIT); });

		auto buttonParent { ir::vgui::makeIconButton(ir::Vector { 26.f, 26.f }, "folder_exit", sf::Color(64u, 224u, 128u)) };
		buttonParent->setPosition(ir::Vector { 451.f, 2.f })
			.registerClickEvent([&]() { evtQueue_->add(filigree::Event::MOVE_TO_PARENT_FOLDER); });
		
		bar->addChildElement("PathLabel", std::move(pathLabel));
		bar->addChildElement("ButtonPrev", std::move(buttonPrev));
		bar->addChildElement("ButtonNext", std::move(buttonNext));
		bar->addChildElement("ButtonParent", std::move(buttonParent));

		fileExplorer_->addChildElement("PathBar", std::move(bar));
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

		LOG_INFO(path.string());
		LOG_INFO(concise);
		return concise;
	}
}