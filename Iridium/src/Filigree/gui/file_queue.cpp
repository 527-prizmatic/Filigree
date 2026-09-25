#include "gui/file_queue.hpp"

#include <algorithm>

#include <vgui/label.hpp>
#include <vgui/factory.hpp>

#include "filepath_funcs.hpp"

namespace filigree::gui {
	FileQueue::FileQueue(filigree::EventQueue& evtQueue) {
		evtQueue_ = &evtQueue;

		fileQueue_ = std::make_unique<ir::vgui::FramedElement>();
		if (!fileQueue_) {
			LOG_ERROR("Could not create processing queue!!");
		}
		else {
			fileQueue_->setColors(sf::Color::White, sf::Color { 16u, 0u, 8u })
				.setSize(ir::Vector { 400.f, 389.f })
				.setPosition(ir::Vector { 880.f, 330.f });

			createUITitle();
			createUIFileList();
		}

		selectionRect_ = std::make_unique<ir::render::Rectangle>();
		selectionRect_->setSize(ir::Vector { 395.f, 16.f })
			.setColor(sf::Color::Transparent)
			.setMode(ir::render::Mode::WIREFRAME);
	}

	void FileQueue::processEvent(const sf::Event& evt, ir::input::Mouse& mouseInput) {
		if (fileQueue_) {
			if (mouseInput.cursorPosition().x < fileQueue_->absolutePosition().x
			|| mouseInput.cursorPosition().x > fileQueue_->absolutePosition().x + fileQueue_->size().x
			|| mouseInput.cursorPosition().y < fileQueue_->absolutePosition().y
			|| mouseInput.cursorPosition().y > fileQueue_->absolutePosition().y + fileQueue_->size().y) {
				return;
			}
		
			fileQueue_->processEvent(evt);
		}

		if (watchedQueue_) {
			if (auto scroll = evt.getIf<sf::Event::MouseWheelScrolled>()) {
				listOffset_ -= scroll->delta;
				if (listOffset_ < 0 || watchedQueue_->size() <= 18) {
					listOffset_ = 0;
				}
				else if (listOffset_ > static_cast<int>(watchedQueue_->size() - 18)) {
					listOffset_ = watchedQueue_->size() - 18;
				}
				updateFileList();
			}
		}
	}

	void FileQueue::update(ir::input::Mouse& mouseInput) {
		if (fileQueue_ && fileQueue_->update(mouseInput)) {
			/// One-line offset to avoid the selection frame displaying when the mouse hovers over the title bar
			int id { static_cast<int>((mouseInput.cursorPosition().y - fileQueue_->absolutePosition().y  - 10.f) / 20.f) - 1 };
			id += listOffset_;
			if (watchedQueue_->size() > static_cast<unsigned int>(id)) {
				selectedPath_ = watchedQueue_->at(id);

				selectionRect_->setColor(sf::Color {255u, 64u, 160u })
					.setPosition(fileQueue_->absolutePosition() + ir::Vector { 2.f, static_cast<float>(id - listOffset_) * 20.f + 32.f });
			}
			else {
				selectedPath_ = {};
				selectionRect_->setColor(sf::Color::Transparent);
			}

			if (mouseInput.isPressed(sf::Mouse::Button::Left) && selectedPath_.has_value()) {
				auto pathIt { std::find(watchedQueue_->begin(), watchedQueue_->end(), selectedPath_.value()) };
				if (pathIt != watchedQueue_->end()) {
					watchedQueue_->erase(pathIt);
					selectedPath_ = {};
					updateFileList();
				}
			}
		}
	}

	void FileQueue::render(ir::render::VertexRenderer& renderer) const {
		if (fileQueue_) {
			fileQueue_->render(renderer);
		}
		if (selectionRect_) {
			selectionRect_->render(renderer);
		}
	}

	void FileQueue::setWatchedQueue(std::vector<std::filesystem::path>* queue) {
		watchedQueue_ = queue;
		updateFileList();
	}

	void FileQueue::createUITitle() {
		auto field { std::make_unique<ir::vgui::FramedElement>() };
		field->setSize(ir::Vector { 400.f, 30.f })
			.setPosition(ir::Vector { 0.f, 0.f })
			.setColors(sf::Color::White, sf::Color::Transparent);

			auto labelTitle { std::make_unique<ir::vgui::Label>("Processing queue") };
			labelTitle->setPosition(ir::Vector { 6.f, 6.f });
			
			field->addChildElement("LabelTitle", std::move(labelTitle));

			auto buttonClear { ir::vgui::makeIconButton(ir::Vector { 26.f, 26.f }, "trash", sf::Color(224u, 64u, 128u)) };
			buttonClear->setPosition(ir::Vector { 372.f, 2.f })
				.registerClickEvent([&]() { evtQueue_->add(filigree::Event::CLEAR_QUEUE); });
			
			field->addChildElement("ButtonClear", std::move(buttonClear));

		fileQueue_->addChildElement("FieldTitle", std::move(field));
	}

	void FileQueue::createUIFileList() {
		for (size_t i = 0; i < 18; i++) {
			auto el { std::make_unique<ir::vgui::FramedElement>() };
			el->setPosition(ir::Vector { 0.f, 30.f + static_cast<float>(i) * 20.f })
				.setSize(ir::Vector { 400.f, 20.f })
				.setColors(sf::Color { 255u, 0u, 128u, 32u }, i % 2 ? sf::Color { 255u, 255u, 255u, 8u } : sf::Color::Transparent);

				auto label { std::make_unique<ir::vgui::Label>("") };
				label->setScale(12.f)
					.setPosition(ir::Vector { 3.f, 1.f });

				el->addChildElement("Label", std::move(label));

			fileQueue_->addChildElement(std::string { "Field" } + std::to_string(i), std::move(el));
		}
	}

	void FileQueue::updateFileList() {
		for (size_t i = 0; i < 18; i++) { /// 360 px / 20 px per field
			auto field { fileQueue_->getChild(std::string { "Field" } + std::to_string(i))->getChild<ir::vgui::Label>("Label") };
			if (i + listOffset_ < watchedQueue_->size()) {
				auto path { watchedQueue_->at(i + listOffset_) };
				field->setLabel(filigree::shortenPath(path, 1));
			}
			else {
				field->setLabel("");
			}
		}
	}
}