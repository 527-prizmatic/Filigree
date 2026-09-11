#ifndef FILIGREE_GUI_TITLEBAR_HPP_
#define FILIGREE_GUI_TITLEBAR_HPP_

#include "events.hpp"

#include <input/mouse.hpp>
#include <rendering/vertex_renderer.hpp>
#include <vgui/element.hpp>

namespace filigree::gui {
	class TitleBar {
	public:
		TitleBar(filigree::EventQueue& evtQueue);

		void update(ir::input::Mouse& mouseInput);
		void render(ir::render::VertexRenderer& renderer) const;

	private:
		void createMinimizeButton();
		void createExitButton();
		void createTitle();

		std::unique_ptr<ir::vgui::FramedElement> titleBar_;
		filigree::EventQueue* evtQueue_;
	};
}

#endif // FILIGREE_GUI_TITLEBAR_HPP_