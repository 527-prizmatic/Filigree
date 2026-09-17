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

			std::unique_ptr<ir::vgui::FramedElement> settings_;
			filigree::EventQueue* evtQueue_ { nullptr };

			ir::vgui::Checkbox* resizeEnabled_ { nullptr };
			ir::vgui::IntField* resizeSize_ { nullptr };
		};
	}
}

#endif // FILIGREE_GUI_SETTINGS_HPP_