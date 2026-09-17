#include "gui/settings.hpp"

#include <vgui/element.hpp>
#include <vgui/label.hpp>
#include <vgui/icon.hpp>

#include <rendering/model_renderer.hpp>
#include <rendering/text.hpp>

namespace filigree::gui {
	SettingsUI::SettingsUI(filigree::EventQueue &evtQueue) {
		evtQueue_ = &evtQueue;

		settings_ = std::make_unique<ir::vgui::FramedElement>();
		if (!settings_) {
			LOG_ERROR("Could not create settings tab!!");
		}
		else {
			settings_->setColors(sf::Color::White, sf::Color { 8u, 16u, 0u })
				.setSize(ir::Vector { 399.f, 689.f })
				.setPosition(ir::Vector { 480.f, 30.f });
				
			createUIResize(30.f);
		}
	}

	void SettingsUI::processEvent(const sf::Event& evt) {
		if (settings_) {
			settings_->processEvent(evt);
		}
	}

	void SettingsUI::update(ir::input::Mouse& mouseInput) {
		resizeSize_->setLabelColor(resizeEnabled_->checked() ? sf::Color::White : sf::Color { 128u, 128u, 128u })
			.setEnabled(resizeEnabled_->checked());

		if (settings_) {
			settings_->update(mouseInput);
		}
	}

	void SettingsUI::render(ir::render::VertexRenderer& renderer) const {
		if (settings_) {
			settings_->render(renderer);
		}
	}
	
	void SettingsUI::createUIResize(float yPos) {
		auto field { std::make_unique<ir::vgui::FramedElement>() };
		field->setSize(ir::Vector { 399.f, 60.f })
			.setPosition(ir::Vector { 0.f, yPos })
			.setColors(sf::Color::White, sf::Color::Transparent);

			auto check { std::make_unique<ir::vgui::Checkbox>() };
			check->setChecked(true)
				.setPosition(ir::Vector { 2.f, 2.f })
				.setSize(ir::Vector { 26.f, 26.f });
				
				auto labelCheck { std::make_unique<ir::vgui::Label>("Resize image") };
				labelCheck->setAnchor(ir::vgui::Label::Anchor::RIGHT)
					.setScale(15.f);

			auto resize { std::make_unique<ir::vgui::IntField>(1000) };
			resize->setScale(12.f)
				.setMaxChars(5u)
				.setPosition(ir::Vector { 2.f, 32.f })
				.setSize(ir::Vector { 96.f, 26.f });
				
				auto labelResize { std::make_unique<ir::vgui::Label>("Largest dimension") };
				labelResize->setAnchor(ir::vgui::Label::Anchor::RIGHT)
					.setScale(15.f);

				check->addChildElement("Label", std::move(labelCheck));
			field->addChildElement("CheckboxResize", std::move(check));

				resize->addChildElement("Label", std::move(labelResize));
			field->addChildElement("Resolution", std::move(resize));
		
		settings_->addChildElement("FieldResize", std::move(field));

		resizeEnabled_ = settings_->getChild<ir::vgui::FramedElement>("FieldResize")->getChild<ir::vgui::Checkbox>("CheckboxResize");
		resizeSize_ = settings_->getChild<ir::vgui::FramedElement>("FieldResize")->getChild<ir::vgui::IntField>("Resolution");
	}



#pragma region Settings accessors
	bool SettingsUI::resizeEnabled() const {
		return resizeEnabled_->checked();
	}

	int SettingsUI::resizeSize() const {
		return resizeSize_->value();
	}

	const ProcessorSettings SettingsUI::assembleSettings() const {
		ProcessorSettings settings;
		
		settings.resize = resizeEnabled();
		settings.resizeSize = resizeSize();

		return settings;
	}
#pragma endregion
}