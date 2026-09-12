#ifndef FILIGREE_GUI_EXPLORER_HPP_
#define FILIGREE_GUI_EXPLORER_HPP_

#include "events.hpp"

#include <input/mouse.hpp>
#include <rendering/vertex_renderer.hpp>
#include <vgui/element.hpp>

namespace filigree::gui {
	class FileExplorer {
	public:
		FileExplorer(filigree::EventQueue& evtQueue);

		void processEvent(const sf::Event& evt);
		void update(ir::input::Mouse& mouseInput);
		void render(ir::render::VertexRenderer& renderer) const;

		std::filesystem::path activeFolder() const;
		std::filesystem::path selectedFile() const;

		void moveToParent();
		void setPath(std::filesystem::path path);

	private:
	//	void createMinimizeButton();
	//	void createExitButton();
	//	void createTitle();

		void createPathBar();
		static std::string concisePath(std::filesystem::path& path);

		std::unique_ptr<ir::vgui::FramedElement> fileExplorer_;
		filigree::EventQueue* evtQueue_;
		std::filesystem::path activeDir_;
		unsigned int listOffset_;
	};
}

#endif // FILIGREE_GUI_EXPLORER_HPP_