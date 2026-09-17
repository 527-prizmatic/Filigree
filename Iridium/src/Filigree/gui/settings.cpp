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
			createUIWatermark(90.f);
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
				.setPosition(ir::Vector { 3.f, 3.f });
				
				auto labelCheck { std::make_unique<ir::vgui::Label>("Resize image") };
				labelCheck->setAnchor(ir::vgui::Label::Anchor::RIGHT);

			auto resize { std::make_unique<ir::vgui::IntField>(1000) };
			resize->setScale(12.f)
				.setMaxChars(5u)
				.setPosition(ir::Vector { 2.f, 32.f })
				.setSize(ir::Vector { 96.f, 26.f });
				
				auto labelResize { std::make_unique<ir::vgui::Label>("Largest dimension") };
				labelResize->setAnchor(ir::vgui::Label::Anchor::RIGHT);

				check->addChildElement("Label", std::move(labelCheck));
			field->addChildElement("CheckboxResize", std::move(check));

				resize->addChildElement("Label", std::move(labelResize));
			field->addChildElement("Resolution", std::move(resize));
		
		settings_->addChildElement("FieldResize", std::move(field));

		resizeEnabled_ = settings_->getChild<ir::vgui::FramedElement>("FieldResize")->getChild<ir::vgui::Checkbox>("CheckboxResize");
		resizeSize_ = settings_->getChild<ir::vgui::FramedElement>("FieldResize")->getChild<ir::vgui::IntField>("Resolution");
	}

	void SettingsUI::createUIWatermark(float yPos) {
		auto field { std::make_unique<ir::vgui::FramedElement>() };
		field->setSize(ir::Vector { 399.f, 120.f })
			.setPosition(ir::Vector { 0.f, yPos })
			.setColors(sf::Color::White, sf::Color::Transparent);

			auto checkF { std::make_unique<ir::vgui::Checkbox>() };
			checkF->setChecked(true)
				.setPosition(ir::Vector { 3.f, 3.f });
				
				auto labelF { std::make_unique<ir::vgui::Label>("Apply filigree") };
				labelF->setAnchor(ir::vgui::Label::Anchor::RIGHT);

				checkF->addChildElement("Label", std::move(labelF));
			field->addChildElement("CheckF", std::move(checkF));

			auto labelS { std::make_unique<ir::vgui::Label>("Apply stamps:") };
			labelS->setPosition(ir::Vector { 5.f, 34.f });
			
			field->addChildElement("LabelS", std::move(labelS));

			auto checkTL { std::make_unique<ir::vgui::Checkbox>() };
			checkTL->setPosition(ir::Vector { 3.f, 63.f });
				
				auto labelTL { std::make_unique<ir::vgui::Label>("Top left") };
				labelTL->setAnchor(ir::vgui::Label::Anchor::RIGHT);

				checkTL->addChildElement("Label", std::move(labelTL));
			field->addChildElement("CheckTL", std::move(checkTL));

			auto checkTR { std::make_unique<ir::vgui::Checkbox>() };
			checkTR->setPosition(ir::Vector { 372.f, 63.f });
				
				auto labelTR { std::make_unique<ir::vgui::Label>("Top right") };
				labelTR->setAnchor(ir::vgui::Label::Anchor::LEFT);

				checkTR->addChildElement("Label", std::move(labelTR));
			field->addChildElement("CheckTR", std::move(checkTR));

			auto checkBL { std::make_unique<ir::vgui::Checkbox>() };
			checkBL->setPosition(ir::Vector { 3.f, 93.f });
				
				auto labelBL { std::make_unique<ir::vgui::Label>("Bottom left") };
				labelBL->setAnchor(ir::vgui::Label::Anchor::RIGHT);

				checkBL->addChildElement("Label", std::move(labelBL));
			field->addChildElement("CheckBL", std::move(checkBL));

			auto checkBR { std::make_unique<ir::vgui::Checkbox>() };
			checkBR->setChecked(true)
				.setPosition(ir::Vector { 372.f, 93.f });
				
				auto labelBR { std::make_unique<ir::vgui::Label>("Bottom right") };
				labelBR->setAnchor(ir::vgui::Label::Anchor::LEFT);

				checkBR->addChildElement("Label", std::move(labelBR));
			field->addChildElement("CheckBR", std::move(checkBR));

		settings_->addChildElement("FieldWatermark", std::move(field));

		filigreeEnabled_ = settings_->getChild<ir::vgui::FramedElement>("FieldWatermark")->getChild<ir::vgui::Checkbox>("CheckF");
		stampTLEnabled_ = settings_->getChild<ir::vgui::FramedElement>("FieldWatermark")->getChild<ir::vgui::Checkbox>("CheckTL");
		stampTREnabled_ = settings_->getChild<ir::vgui::FramedElement>("FieldWatermark")->getChild<ir::vgui::Checkbox>("CheckTR");
		stampBLEnabled_ = settings_->getChild<ir::vgui::FramedElement>("FieldWatermark")->getChild<ir::vgui::Checkbox>("CheckBL");
		stampBREnabled_ = settings_->getChild<ir::vgui::FramedElement>("FieldWatermark")->getChild<ir::vgui::Checkbox>("CheckBR");
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
		settings.applyFiligree = filigreeEnabled_->checked();

		/// @todo oh no
		settings.applyStamps = ProcessorSettings::StampsToApply::NONE;
		if (stampTLEnabled_->checked())
			settings.applyStamps = static_cast<ProcessorSettings::StampsToApply>(settings.applyStamps | ProcessorSettings::StampsToApply::TOP_LEFT);
		if (stampTREnabled_->checked())
			settings.applyStamps = static_cast<ProcessorSettings::StampsToApply>(settings.applyStamps | ProcessorSettings::StampsToApply::TOP_RIGHT);
		if (stampBLEnabled_->checked())
			settings.applyStamps = static_cast<ProcessorSettings::StampsToApply>(settings.applyStamps | ProcessorSettings::StampsToApply::BOTTOM_LEFT);
		if (stampBREnabled_->checked())
			settings.applyStamps = static_cast<ProcessorSettings::StampsToApply>(settings.applyStamps | ProcessorSettings::StampsToApply::BOTTOM_RIGHT);

		return settings;
	}
#pragma endregion
}