#ifndef FILIGREE_GUI_PREVIEW_HPP_
#define FILIGREE_GUI_PREVIEW_HPP_

#include "events.hpp"

#include <atomic>
#include <rendering/quad.hpp>
#include <rendering/vertex_renderer.hpp>
#include <vgui/element.hpp>

namespace filigree::gui {
	class ImagePreview {
	public:
		ImagePreview(filigree::EventQueue& evtQueue);

		void processEvent(const sf::Event& evt, ir::input::Mouse& mouseInput);
		void update(ir::input::Mouse& mouseInput);
		void render(ir::render::VertexRenderer& renderer);

		void updateSpinnerAngle(float dt);

		void setTexture(std::filesystem::path tex);

		void prepareTexture(std::filesystem::path path);
		void deleteTexture();

	private:
		void createUIPreview();
		void drawSpinner(ir::render::VertexRenderer& renderer, ir::Vector center);

		filigree::EventQueue* evtQueue_ { nullptr };
		
		std::unique_ptr<ir::vgui::FramedElement> preview_ { nullptr };

		std::unique_ptr<sf::Texture> img_ { nullptr };
		std::unique_ptr<ir::render::Quad> quad_ { nullptr };

		std::mutex mutex_;

		float spinnerAngle_ { 0.f };

		std::optional<std::filesystem::path> texPath_ {};
		std::atomic<bool> shouldDelete_ { false };
	};
}

#endif // FILIGREE_GUI_PREVIEW_HPP_