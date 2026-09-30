#include "gui/title_bar.hpp"

#include <vgui/element.hpp>
#include <vgui/label.hpp>
#include <vgui/icon.hpp>
#include <vgui/factory.hpp>

#include <rendering/model_renderer.hpp>
#include <rendering/text.hpp>

namespace filigree::gui {
	TitleBar::TitleBar(filigree::EventQueue &evtQueue) {
		evtQueue_ = &evtQueue;

		titleBar_ = std::make_unique<ir::vgui::FramedElement>();
		if (!titleBar_) {
			LOG_ERROR("Could not create title bar!!");
		}
		else {
			titleBar_->setColors(sf::Color::White, sf::Color { 8u, 0u, 16u })
				.setSize(ir::Vector { 1279.f, 30.f })
				.setPosition(ir::Vector { 1.f, 0.f });

			createMinimizeButton();
			createExitButton();
			createTitle();
		}
	}

	void TitleBar::update(ir::input::Mouse& mouseInput) {
		if (titleBar_) {
			titleBar_->update(mouseInput);
		}
	}

	void TitleBar::render(ir::render::VertexRenderer& renderer) const {
		if (titleBar_) {
			titleBar_->render(renderer);
		}
	}
	
	void TitleBar::createMinimizeButton() {
		auto button { ir::vgui::makeIconButton(ir::Vector { 26.f, 26.f }, "tools\\line", sf::Color(32u, 224u, 192u)) };
		button->setPosition(ir::Vector { 1223.f, 2.f })
			.registerClickEvent([&]() { evtQueue_->add(filigree::Event::MINIMIZE); });
		button->getChild("Icon")->setPosition(ir::Vector { 2.f, 19.f });
		titleBar_->setChildElement("ButtonMinimize", std::move(button));
	}

	void TitleBar::createExitButton() {
		auto button { ir::vgui::makeIconButton(ir::Vector { 26.f, 26.f }, "tools\\cross", sf::Color(224u, 48u, 92u)) };
		button->setPosition(ir::Vector { 1251.f, 2.f })
			.registerClickEvent([&]() { evtQueue_->add(filigree::Event::EXIT); });
		titleBar_->setChildElement("ButtonExit", std::move(button));
	}

	void TitleBar::createTitle() {
		auto title { titleBar_->addChildElement<ir::vgui::Label>("Title", "Filigree --- Image Watermarker") };
		title->setScale(18.f)
			.setAnchor(ir::vgui::Label::Anchor::OVER)
			.setColor(sf::Color { 192u, 128u, 255u });
	}
}