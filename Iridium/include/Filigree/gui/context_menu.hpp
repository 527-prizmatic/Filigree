#ifndef FILIGREE_GUI_CONTEXTMENU_HPP_
#define FILIGREE_GUI_CONTEXTMENU_HPP_

#include "events.hpp"

#include <input/mouse.hpp>
#include <rendering/vertex_renderer.hpp>
#include <vgui/element.hpp>

namespace filigree::gui {
	class ContextMenu {
	public:
		enum class Mode {
			DIRECTORY,
			IMAGE,
			OTHER
		};

		ContextMenu(filigree::EventQueue& evtQueue);

		void processEvent(const sf::Event& evt, ir::input::Mouse& mouseInput);
		void update(ir::input::Mouse& mouseInput);
		void render(ir::render::VertexRenderer& renderer) const;

		void create(std::filesystem::path path, ir::Vector pos);
		void createParent(std::filesystem::path path, ir::Vector pos);
		void close();

		[[nodiscard]] bool active() const { return ctxMenu_ != nullptr; }
		[[nodiscard]] std::filesystem::path path() const { return currentPath_; }

	private:
		void createUITitle(int entryCount, sf::Color clrTitle);
		void createUIDir();
		void createUIImg();
		void createUIOther();
		void createUIParent();

		ir::vgui::Element* setupButton(ir::vgui::Element* el, float posY, filigree::Event evt, std::string label);
		
		filigree::EventQueue* evtQueue_ { nullptr };
		std::unique_ptr<ir::vgui::FramedElement> ctxMenu_ { nullptr };
		std::filesystem::path currentPath_ {};
	};
}

#endif // FILIGREE_GUI_CONTEXTMENU_HPP_