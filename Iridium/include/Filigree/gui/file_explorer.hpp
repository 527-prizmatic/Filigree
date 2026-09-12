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
		std::optional<std::filesystem::path> selectedPath() const { return selectedPath_; }

		void moveToParent();
		void setPath(std::filesystem::path path);

		void processFileSelection();

	private:
	//	void createMinimizeButton();
	//	void createExitButton();
	//	void createTitle();

		void createPathBar();
		void createFileFields();
		static std::string concisePath(std::filesystem::path& path);

		void populateFileList();
		void updateFileList();

		std::unique_ptr<ir::vgui::FramedElement> fileExplorer_ {};
		filigree::EventQueue* evtQueue_ { nullptr };
		std::filesystem::path activeDir_ {};
		int listOffset_ { 0 };

		std::vector<std::filesystem::path> pathList_ {};
		std::optional<std::filesystem::path> selectedPath_ {};

		std::unique_ptr<ir::render::Rectangle> selectionRect_ {};
	};
}

#endif // FILIGREE_GUI_EXPLORER_HPP_