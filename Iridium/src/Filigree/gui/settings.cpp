#include "gui/settings.hpp"

#include <vgui/element.hpp>
#include <vgui/label.hpp>
#include <vgui/icon.hpp>
#include <vgui/factory.hpp>

#include <rendering/model_renderer.hpp>
#include <rendering/text.hpp>

#include "filepath_funcs.hpp"

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
				.setPosition(ir::Vector { 481.f, 30.f });
				
			createTitle(0.f);
			createUIResize(30.f);
			createUIWatermark(90.f);
			createUINoise(240.f);
			createUIDepth(300.f);

			createUIPaths(570.f);
			createUIStartButton(630.f);
		}
	}

	void SettingsUI::processEvent(const sf::Event& evt) {
		if (settings_) {
			settings_->processEvent(evt);
		}
	}

	void SettingsUI::update(ir::input::Mouse& mouseInput) {
		if (evtQueue_->isOpen()) {
			startProcessingButton_->setColors(sf::Color::White, sf::Color { 32u, 64u, 0u });
		}
		else {
			startProcessingButton_->setColors(sf::Color::White, sf::Color { 48u, 48u, 48u });
		}

		if (resizeSize_) {
			resizeSize_->setLabelColor(resizeEnabled_->checked() ? sf::Color::White : sf::Color { 128u, 128u, 128u })
				.setEnabled(resizeEnabled_->checked());
		}
		
		if (watermarkOpacity_) {
			auto labelO { settings_->getChild<ir::vgui::FramedElement>("FieldWatermark")->getChild<ir::vgui::Label>("LabelO") };
			if (labelO) {
				labelO->setLabel("Opacity: " + std::to_string(watermarkOpacity_->value()));
			}
		}

		if (noiseOpacity_) {
			auto labelO { settings_->getChild<ir::vgui::FramedElement>("FieldNoise")->getChild<ir::vgui::Label>("LabelO") };
			if (labelO) {
				labelO->setLabel("Opacity: " + std::to_string(noiseOpacity_->value()));
			}
		}

		auto labelD { settings_->getChild<ir::vgui::FramedElement>("FieldDepth")->getChild<ir::vgui::Label>("Label") };
		if (labelD) {
			labelD->setLabel("Color depth: " + std::to_string(colorDepth_->value()) + " bits");
		}

		auto labelO { settings_->getChild<ir::vgui::FramedElement>("FieldPaths")->getChild<ir::vgui::Label>("LabelOutput") };
		if (labelO) {
			labelO->setLabel("Output path: " + filigree::shortenPath(pathOutput_, 1));
		}

		auto labelF { settings_->getChild<ir::vgui::FramedElement>("FieldPaths")->getChild<ir::vgui::Label>("LabelFiligree") };
		if (labelF) {
			labelF->setLabel("Filigree path: " + filigree::shortenPath(pathFiligree_, 1));
		}
		
		auto labelS { settings_->getChild<ir::vgui::FramedElement>("FieldPaths")->getChild<ir::vgui::Label>("LabelStamp") };
		if (labelS) {
			labelS->setLabel("Stamp path: " + filigree::shortenPath(pathStamp_, 1));
		}
		
		if (settings_) {
			settings_->update(mouseInput);
		}
	}

	void SettingsUI::render(ir::render::VertexRenderer& renderer) const {
		if (settings_) {
			settings_->render(renderer);
		}
	}
	
	void SettingsUI::createTitle(float yPos) {
		auto titleField { std::make_unique<ir::vgui::FramedElement>() };
		titleField->setSize(ir::Vector { 399.f, 30.f })
			.setPosition(ir::Vector { 0.f, yPos })
			.setColors(sf::Color::White, sf::Color { 32u, 64u, 0u });

			auto label { std::make_unique<ir::vgui::Label>("Configuration") };
			label->setScale(15.f)
				.setAnchor(ir::vgui::Label::Anchor::OVER);

			titleField->setChildElement("Label", std::move(label));

		settings_->setChildElement("Title", std::move(titleField));
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
			resize->setScale(15.f)
				.setMaxChars(5u)
				.setColorUnfocused(sf::Color { 16u, 32u, 0u })
				.setColorFocused(sf::Color { 48u, 96u, 0u })
				.setPosition(ir::Vector { 2.f, 32.f })
				.setSize(ir::Vector { 96.f, 26.f });
				
				auto labelResize { std::make_unique<ir::vgui::Label>("Largest dimension") };
				labelResize->setAnchor(ir::vgui::Label::Anchor::RIGHT);

				check->setChildElement("Label", std::move(labelCheck));
			field->setChildElement("CheckboxResize", std::move(check));

				resize->setChildElement("Label", std::move(labelResize));
			field->setChildElement("Resolution", std::move(resize));
		
		settings_->setChildElement("FieldResize", std::move(field));

		resizeEnabled_ = settings_->getChild<ir::vgui::FramedElement>("FieldResize")->getChild<ir::vgui::Checkbox>("CheckboxResize");
		resizeSize_ = settings_->getChild<ir::vgui::FramedElement>("FieldResize")->getChild<ir::vgui::IntField>("Resolution");
	}

	void SettingsUI::createUIWatermark(float yPos) {
		/// Root
		auto field { settings_->addChildElement<ir::vgui::FramedElement>("FieldWatermark") };
		field->setSize(ir::Vector { 399.f, 150.f })
			.setPosition(ir::Vector { 0.f, yPos })
			.setColors(sf::Color::White, sf::Color::Transparent);

		/// Checkbox to enable filigree
		filigreeEnabled_ = field->addChildElement<ir::vgui::Checkbox>("CheckboxF");
		filigreeEnabled_->setChecked(true)
			.setPosition(ir::Vector { 3.f, 3.f });
		ir::vgui::addLabel(filigreeEnabled_, "Apply filigree", ir::vgui::Label::Anchor::RIGHT);

		/// Stamps section label
		auto labelS { field->addChildElement<ir::vgui::Label>("LabelS", "Apply stamps:") };
		labelS->setPosition(ir::Vector { 5.f, 34.f });
		
		/// Stamps checkboxes
		stampTLEnabled_ = field->addChildElement<ir::vgui::Checkbox>("CheckboxTL");
		stampTLEnabled_->setPosition(ir::Vector { 3.f, 63.f });
		ir::vgui::addLabel(stampTLEnabled_, "Top left", ir::vgui::Label::Anchor::RIGHT);

		stampTREnabled_ = field->addChildElement<ir::vgui::Checkbox>("CheckboxTR");
		stampTREnabled_->setPosition(ir::Vector { 372.f, 63.f });
		ir::vgui::addLabel(stampTREnabled_, "Top right", ir::vgui::Label::Anchor::LEFT);

		stampBLEnabled_ = field->addChildElement<ir::vgui::Checkbox>("CheckboxBL");
		stampBLEnabled_->setPosition(ir::Vector { 3.f, 63.f });
		ir::vgui::addLabel(stampBLEnabled_, "Bottom left", ir::vgui::Label::Anchor::RIGHT);

		stampBREnabled_ = field->addChildElement<ir::vgui::Checkbox>("CheckboxBR");
		stampBREnabled_->setPosition(ir::Vector { 372.f, 63.f });
		ir::vgui::addLabel(stampBREnabled_, "Bottom right", ir::vgui::Label::Anchor::LEFT);

		/// Opacity slider
		watermarkOpacity_ = field->addChildElement<ir::vgui::Slider>("SliderOpacity", 0, 100);
		watermarkOpacity_->setValue(20)
			.setPosition(ir::Vector { 133.f, 123.f })
			.setSize(ir::Vector { 254.f, 24.f })
			.setColors(sf::Color::White, sf::Color { 96u, 192u, 0u });
	}

	void SettingsUI::createUINoise(float yPos) {
		auto field { std::make_unique<ir::vgui::FramedElement>() };
		field->setSize(ir::Vector { 399.f, 60.f })
			.setPosition(ir::Vector { 0.f, yPos })
			.setColors(sf::Color::White, sf::Color::Transparent);

			auto check { std::make_unique<ir::vgui::Checkbox>() };
			check->setChecked(true)
				.setPosition(ir::Vector { 3.f, 3.f });
				
				auto labelCheck { std::make_unique<ir::vgui::Label>("Add pixel noise") };
				labelCheck->setAnchor(ir::vgui::Label::Anchor::RIGHT);

				check->setChildElement("Label", std::move(labelCheck));
			field->setChildElement("CheckboxNoise", std::move(check));

			auto labelO { std::make_unique<ir::vgui::Label>("Opacity:") };
			labelO->setPosition(ir::Vector { 5.f, 34.f });
			field->setChildElement("LabelO", std::move(labelO));

			auto sliderOpacity { std::make_unique<ir::vgui::Slider>(0, 100) };
			sliderOpacity->setValue(5)
				.setPosition(ir::Vector { 133.f, 33.f })
				.setSize(ir::Vector { 254.f, 24.f })
				.setColors(sf::Color::White, sf::Color { 96u, 192u, 0u });

			field->setChildElement("SliderOpacity", std::move(sliderOpacity));

		settings_->setChildElement("FieldNoise", std::move(field));

		noiseEnabled_ = settings_->getChild<ir::vgui::FramedElement>("FieldNoise")->getChild<ir::vgui::Checkbox>("CheckboxNoise");
		noiseOpacity_ = settings_->getChild<ir::vgui::FramedElement>("FieldNoise")->getChild<ir::vgui::Slider>("SliderOpacity");
	}

	void SettingsUI::createUIDepth(float yPos) {
		auto field { settings_->addChildElement<ir::vgui::FramedElement>("FieldDepth") };
		field->setSize(ir::Vector { 399.f, 30.f })
			.setPosition(ir::Vector { 0.f, yPos })
			.setColors(sf::Color::White, sf::Color::Transparent);

		auto label { field->addChildElement<ir::vgui::Label>("Label", "Color depth:") };
		label->setPosition(ir::Vector { 5.f, 4.f });

		auto slider { field->addChildElement<ir::vgui::Slider>("SliderDepth", 1, 8) };
		slider->setValue(8)
			.setPosition(ir::Vector { 203.f, 3.f })
			.setSize(ir::Vector { 184.f, 24.f })
			.setColors(sf::Color::White, sf::Color { 96u, 192u, 0u });
		colorDepth_ = slider;
	}

	void SettingsUI::createUIPaths(float yPos) {
		auto field { std::make_unique<ir::vgui::FramedElement>() };
		field->setSize(ir::Vector { 399.f, 60.f })
			.setPosition(ir::Vector { 0.f, yPos })
			.setColors(sf::Color::White, sf::Color::Transparent);

			auto labelO { std::make_unique<ir::vgui::Label>("Output path:") };
			labelO->setScale(12.f)
				.setPosition(ir::Vector { 5.f, 3.f });
			field->setChildElement("LabelOutput", std::move(labelO));

			auto labelF { std::make_unique<ir::vgui::Label>("Filigree path:") };
			labelF->setScale(12.f)
				.setPosition(ir::Vector { 5.f, 23.f });
			field->setChildElement("LabelFiligree", std::move(labelF));

			auto labelS { std::make_unique<ir::vgui::Label>("Stamp path:") };
			labelS->setScale(12.f)
				.setPosition(ir::Vector { 5.f, 43.f });
			field->setChildElement("LabelStamp", std::move(labelS));

		settings_->setChildElement("FieldPaths", std::move(field));
	}

	void SettingsUI::createUIStartButton(float yPos) {
		auto button { std::make_unique<ir::vgui::FramedElement>() };
		button->setSize(ir::Vector { 393.f, 53.f })
			.setPosition(ir::Vector { 3.f, yPos + 3.f })
			.setColors(sf::Color::White, sf::Color { 32u, 64u, 0u })
			.registerClickEvent([&]() { evtQueue_->add(filigree::Event::START_FILE_PROCESSING); });

			auto label { std::make_unique<ir::vgui::Label>("Start processing") };
			label->setScale(24.f)
				.setAnchor(ir::vgui::Label::Anchor::OVER);

			button->setChildElement("Label", std::move(label));

		settings_->setChildElement("ButtonStart", std::move(button));

		startProcessingButton_ = settings_->getChild<ir::vgui::FramedElement>("ButtonStart");
	}

#pragma region Settings accessors
	bool SettingsUI::resizeEnabled() const {
		return resizeEnabled_->checked();
	}

	int SettingsUI::resizeSize() const {
		return resizeSize_->value();
	}

	void SettingsUI::setOutputPath(std::filesystem::path path) {
		pathOutput_ = path;
	}

	void SettingsUI::setFiligreePath(std::filesystem::path path) {
		pathFiligree_ = path;
	}
	
	void SettingsUI::setStampPath(std::filesystem::path path) {
		pathStamp_ = path;
	}
	
	const ProcessorSettings SettingsUI::assembleSettings() const {
		ProcessorSettings settings;

		settings.resize = resizeEnabled();
		settings.resizeSize = resizeSize();

		settings.applyFiligree = filigreeEnabled_->checked();
		settings.applyStampTL = stampTLEnabled_->checked();
		settings.applyStampTR = stampTREnabled_->checked();
		settings.applyStampBL = stampBLEnabled_->checked();
		settings.applyStampBR = stampBREnabled_->checked();
		settings.watermarkOpacity = watermarkOpacity_->value() * .01f;

		settings.pathOutput = pathOutput_;
		settings.pathFiligree = pathFiligree_;
		settings.pathStamp = pathStamp_;

		settings.applyNoise = noiseEnabled_->checked();
		settings.noiseOpacity = noiseOpacity_->value() * .01f;

		settings.colorDepth = colorDepth_->value();

		return settings;
	}
#pragma endregion
}