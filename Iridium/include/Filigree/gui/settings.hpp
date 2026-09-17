#ifndef FILIGREE_GUI_SETTINGS_HPP_
#define FILIGREE_GUI_SETTINGS_HPP_

#include "events.hpp"

#include <input/mouse.hpp>
#include <rendering/vertex_renderer.hpp>
#include <vgui/element.hpp>
#include <vgui/checkbox.hpp>
#include <vgui/input_field.hpp>

namespace filigree {
	struct ProcessorSettings {
		bool resize { false };
		int resizeSize { 1000 };

		std::string filigreePath { "filigree.png" };
		std::string stampPath { "stamp.png" };
		
		bool applyFiligree { false };
		bool applyStampTL { false };
		bool applyStampTR { false };
		bool applyStampBL { false };
		bool applyStampBR { false };
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

			const ProcessorSettings assembleSettings() const;

		private:
			void createUIResize(float yPos);
			void createUIWatermark(float yPos);

			std::unique_ptr<ir::vgui::FramedElement> settings_;
			filigree::EventQueue* evtQueue_ { nullptr };

			ir::vgui::Checkbox* resizeEnabled_ { nullptr };
			ir::vgui::IntField* resizeSize_ { nullptr };
			
			ir::vgui::Checkbox* filigreeEnabled_ { nullptr };
			ir::vgui::Checkbox* stampTLEnabled_ { nullptr };
			ir::vgui::Checkbox* stampTREnabled_ { nullptr };
			ir::vgui::Checkbox* stampBLEnabled_ { nullptr };
			ir::vgui::Checkbox* stampBREnabled_ { nullptr };
		};
	}
}

#endif // FILIGREE_GUI_SETTINGS_HPP_