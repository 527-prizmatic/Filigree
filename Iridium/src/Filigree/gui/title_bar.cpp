#include "gui/title_bar.hpp"

#include <vgui/element.hpp>
#include <vgui/label.hpp>
#include <vgui/icon.hpp>

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
			titleBar_->setColors(sf::Color::White, sf::Color { 8u, 0u, 16u });
			titleBar_->setSize(ir::Vector { 1279.f, 30.f });
			titleBar_->setPosition(ir::Vector { 1.f, 0.f });

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
		auto buttonMinimize { std::make_unique<ir::vgui::FramedElement>() };
		buttonMinimize->setPosition(ir::Vector { 1223.f, 2.f });
		buttonMinimize->setSize(ir::Vector { 26.f, 26.f });
		buttonMinimize->setColors(sf::Color(32u, 224u, 192u, 255u), sf::Color(32u, 224u, 192u, 32u));
		buttonMinimize->registerClickEvent([&]() { evtQueue_->add(filigree::Event::MINIMIZE); });

		auto icon { std::make_unique<ir::vgui::Icon>("tools\\line") };
		icon->setFrameColor(sf::Color(32u, 224u, 192u, 255u));
		icon->setScale(22.f);
		icon->setPosition(ir::Vector { 2.f, 19.f });

		buttonMinimize->addChildElement("Icon", std::move(icon));
		titleBar_->addChildElement("ButtonMinimize", std::move(buttonMinimize));
	}

	void TitleBar::createExitButton() {
		auto buttonExit { std::make_unique<ir::vgui::FramedElement>() };
		buttonExit->setPosition(ir::Vector { 1251.f, 2.f });
		buttonExit->setSize(ir::Vector { 26.f, 26.f });
		buttonExit->setColors(sf::Color(224u, 48u, 92u, 255u), sf::Color(224u, 48u, 92u, 32u));
		buttonExit->registerClickEvent([&]() { evtQueue_->add(filigree::Event::EXIT); });

		auto icon { std::make_unique<ir::vgui::Icon>("tools\\cross") };
		icon->setFrameColor(sf::Color(224u, 48u, 92u, 255u));
		icon->setScale(21.f);
		icon->setPosition(ir::Vector { 2.f, 2.f });

		buttonExit->addChildElement("Icon", std::move(icon));
		titleBar_->addChildElement("ButtonExit", std::move(buttonExit));
	}

	void TitleBar::createTitle() {
		auto title { std::make_unique<ir::vgui::Label>() };
		title->setScale(18.f);
		title->setAnchor(ir::vgui::Label::Anchor::OVER);
		title->setLabel("Filigree --- Image Watermarker");
		title->setColor(sf::Color { 192u, 128u, 255u });
		titleBar_->addChildElement("Title", std::move(title));
	}
}