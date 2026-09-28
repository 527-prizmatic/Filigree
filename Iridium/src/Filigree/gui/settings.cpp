#include "gui/settings.hpp"

#include <vgui/element.hpp>
#include <vgui/label.hpp>
#include <vgui/icon.hpp>

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
			labelO->setLabel("Output path: " + filigree::shortenPath(pathOutput, 1));
		}

		auto labelF { settings_->getChild<ir::vgui::FramedElement>("FieldPaths")->getChild<ir::vgui::Label>("LabelFiligree") };
		if (labelF) {
			labelF->setLabel("Filigree path: " + filigree::shortenPath(pathFiligree, 1));
		}
		
		auto labelS { settings_->getChild<ir::vgui::FramedElement>("FieldPaths")->getChild<ir::vgui::Label>("LabelStamp") };
		if (labelS) {
			labelS->setLabel("Stamp path: " + filigree::shortenPath(pathStamp, 1));
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
			resize->setScale(12.f)
				.setMaxChars(5u)
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
		auto field { std::make_unique<ir::vgui::FramedElement>() };
		field->setSize(ir::Vector { 399.f, 150.f })
			.setPosition(ir::Vector { 0.f, yPos })
			.setColors(sf::Color::White, sf::Color::Transparent);

			auto checkF { std::make_unique<ir::vgui::Checkbox>() };
			checkF->setChecked(true)
				.setPosition(ir::Vector { 3.f, 3.f });
				
				auto labelF { std::make_unique<ir::vgui::Label>("Apply filigree") };
				labelF->setAnchor(ir::vgui::Label::Anchor::RIGHT);

				checkF->setChildElement("Label", std::move(labelF));
			field->setChildElement("CheckF", std::move(checkF));

			auto labelS { std::make_unique<ir::vgui::Label>("Apply stamps:") };
			labelS->setPosition(ir::Vector { 5.f, 34.f });
			field->setChildElement("LabelS", std::move(labelS));

			auto checkTL { std::make_unique<ir::vgui::Checkbox>() };
			checkTL->setPosition(ir::Vector { 3.f, 63.f });
				
				auto labelTL { std::make_unique<ir::vgui::Label>("Top left") };
				labelTL->setAnchor(ir::vgui::Label::Anchor::RIGHT);

				checkTL->setChildElement("Label", std::move(labelTL));
			field->setChildElement("CheckTL", std::move(checkTL));

			auto checkTR { std::make_unique<ir::vgui::Checkbox>() };
			checkTR->setPosition(ir::Vector { 372.f, 63.f });
				
				auto labelTR { std::make_unique<ir::vgui::Label>("Top right") };
				labelTR->setAnchor(ir::vgui::Label::Anchor::LEFT);

				checkTR->setChildElement("Label", std::move(labelTR));
			field->setChildElement("CheckTR", std::move(checkTR));

			auto checkBL { std::make_unique<ir::vgui::Checkbox>() };
			checkBL->setPosition(ir::Vector { 3.f, 93.f });
				
				auto labelBL { std::make_unique<ir::vgui::Label>("Bottom left") };
				labelBL->setAnchor(ir::vgui::Label::Anchor::RIGHT);

				checkBL->setChildElement("Label", std::move(labelBL));
			field->setChildElement("CheckBL", std::move(checkBL));

			auto checkBR { std::make_unique<ir::vgui::Checkbox>() };
			checkBR->setChecked(true)
				.setPosition(ir::Vector { 372.f, 93.f });
				
				auto labelBR { std::make_unique<ir::vgui::Label>("Bottom right") };
				labelBR->setAnchor(ir::vgui::Label::Anchor::LEFT);

				checkBR->setChildElement("Label", std::move(labelBR));
			field->setChildElement("CheckBR", std::move(checkBR));

			auto labelO { std::make_unique<ir::vgui::Label>("Opacity:") };
			labelO->setPosition(ir::Vector { 5.f, 124.f });
			field->setChildElement("LabelO", std::move(labelO));

			auto sliderOpacity { std::make_unique<ir::vgui::Slider>(0, 100) };
			sliderOpacity->setValue(20)
				.setPosition(ir::Vector { 133.f, 123.f })
				.setSize(ir::Vector { 254.f, 24.f })
				.setColors(sf::Color::White, sf::Color { 96u, 192u, 0u });

			field->setChildElement("SliderOpacity", std::move(sliderOpacity));

		settings_->setChildElement("FieldWatermark", std::move(field));

		filigreeEnabled_ = settings_->getChild<ir::vgui::FramedElement>("FieldWatermark")->getChild<ir::vgui::Checkbox>("CheckF");
		stampTLEnabled_ = settings_->getChild<ir::vgui::FramedElement>("FieldWatermark")->getChild<ir::vgui::Checkbox>("CheckTL");
		stampTREnabled_ = settings_->getChild<ir::vgui::FramedElement>("FieldWatermark")->getChild<ir::vgui::Checkbox>("CheckTR");
		stampBLEnabled_ = settings_->getChild<ir::vgui::FramedElement>("FieldWatermark")->getChild<ir::vgui::Checkbox>("CheckBL");
		stampBREnabled_ = settings_->getChild<ir::vgui::FramedElement>("FieldWatermark")->getChild<ir::vgui::Checkbox>("CheckBR");
		watermarkOpacity_ = settings_->getChild<ir::vgui::FramedElement>("FieldWatermark")->getChild<ir::vgui::Slider>("SliderOpacity");
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

		settings.pathOutput = pathOutput;
		settings.pathFiligree = pathFiligree;
		settings.pathStamp = pathStamp;

		settings.applyNoise = noiseEnabled_->checked();
		settings.noiseOpacity = noiseOpacity_->value() * .01f;

		settings.colorDepth = colorDepth_->value();

		return settings;
	}
#pragma endregion
}