#ifndef FILIGREE_GUI_QUEUE_HPP_
#define FILIGREE_GUI_QUEUE_HPP_

#include "events.hpp"

#include <input/mouse.hpp>
#include <rendering/vertex_renderer.hpp>
#include <vgui/element.hpp>

namespace filigree::gui {
	class FileQueue {
	public:
		FileQueue(filigree::EventQueue& evtQueue);

		void processEvent(const sf::Event& evt, ir::input::Mouse& mouseInput);
		void update(ir::input::Mouse& mouseInput);
		void render(ir::render::VertexRenderer& renderer) const;

		void setWatchedQueue(std::vector<std::filesystem::path>* queue = nullptr);
		void updateFileList();

	private:
		void createUITitle();
		void createUIFileList();

		filigree::EventQueue* evtQueue_ { nullptr };
		std::vector<std::filesystem::path>* watchedQueue_ { nullptr };
		std::optional<std::filesystem::path> selectedPath_ {};
		
		std::unique_ptr<ir::vgui::FramedElement> fileQueue_ { nullptr };
		std::unique_ptr<ir::render::Rectangle> selectionRect_ { nullptr };
		int listOffset_ { 0 };
	};
}

#endif // FILIGREE_GUI_QUEUE_HPP_