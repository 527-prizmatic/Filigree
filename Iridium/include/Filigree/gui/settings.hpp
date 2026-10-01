#ifndef FILIGREE_GUI_SETTINGS_HPP_
#define FILIGREE_GUI_SETTINGS_HPP_

#include "events.hpp"

#include <input/mouse.hpp>
#include <rendering/vertex_renderer.hpp>
#include <vgui/element.hpp>
#include <vgui/checkbox.hpp>
#include <vgui/input_field.hpp>
#include <vgui/slider.hpp>

namespace filigree {
	struct ProcessorSettings {
		bool resize { false };
		int resizeSize { 1000 };

		std::filesystem::path pathOutput {};
		std::filesystem::path pathFiligree {};
		std::filesystem::path pathStamp {};
		
		bool applyFiligree { false };
		bool applyStampTL { false };
		bool applyStampTR { false };
		bool applyStampBL { false };
		bool applyStampBR { false };
		float watermarkOpacity { .2f };

		bool applyNoise { false };
		float noiseOpacity { .05f };

		char colorDepth { 2u };
	};

	namespace gui {
		class SettingsUI {
		public:
			SettingsUI(filigree::EventQueue& evtQueue);

			void processEvent(const sf::Event& evt);
			void update(ir::input::Mouse& mouseInput);
			void render(ir::render::VertexRenderer& renderer) const;

			bool resizeEnabled() const;
			int resizeSize() const;

			void setOutputPath(std::filesystem::path path);
			void setFiligreePath(std::filesystem::path path);
			void setStampPath(std::filesystem::path path);

			const ProcessorSettings assembleSettings() const;

		private:
			void createTitle(float yPos);
			void createUIResize(float yPos);
			void createUIWatermark(float yPos);
			void createUINoise(float yPos);
			void createUIDepth(float yPos);
			void createUIPaths(float yPos);
			void createUIStartButton(float yPos);

			std::unique_ptr<ir::vgui::FramedElement> settings_;
			filigree::EventQueue* evtQueue_ { nullptr };

			ir::vgui::Checkbox* resizeEnabled_ { nullptr };
			ir::vgui::IntField* resizeSize_ { nullptr };
			
			ir::vgui::Checkbox* filigreeEnabled_ { nullptr };
			ir::vgui::Checkbox* stampTLEnabled_ { nullptr };
			ir::vgui::Checkbox* stampTREnabled_ { nullptr };
			ir::vgui::Checkbox* stampBLEnabled_ { nullptr };
			ir::vgui::Checkbox* stampBREnabled_ { nullptr };

			ir::vgui::Slider* watermarkOpacity_ { nullptr };
			
			ir::vgui::Checkbox* noiseEnabled_ { nullptr };
			ir::vgui::Slider* noiseOpacity_ { nullptr };
			ir::vgui::Slider* colorDepth_ { nullptr };

			ir::vgui::FramedElement* startProcessingButton_ { nullptr };

			std::filesystem::path pathOutput_ { std::filesystem::current_path().parent_path().append("out\\") };
			std::filesystem::path pathFiligree_ { "..\\resources\\default_filigree.png" };
			std::filesystem::path pathStamp_ { "..\\resources\\default_stamp.png" };
		};
	}
}

#endif // FILIGREE_GUI_SETTINGS_HPP_