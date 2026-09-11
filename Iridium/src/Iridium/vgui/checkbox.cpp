#include "vgui/checkbox.hpp"
#include "rendering/vertex_renderer.hpp"

namespace ir::vgui {
	void Checkbox::onIdle() {
		clrBackground_ = sf::Color::Transparent;
	}

	void Checkbox::onHover() {
		clrBackground_ = sf::Color(255, 255, 255, 64);
	}

	void Checkbox::onClick() {
		enabled_ = !enabled_;
	}

	void Checkbox::onDeselect() {
	//	enabled_ = false;
	}

	void Checkbox::render(ir::render::VertexRenderer& renderer) const {
		renderFrame(renderer);
		if (enabled_) {
			renderCheckbox(renderer);
		}
		renderChildren(renderer);
	}

	void Checkbox::renderCheckbox(ir::render::VertexRenderer& renderer) const {
		ir::Vector absPos { absolutePosition() };

		renderer.reset();
		renderer.addPoint(absPos + ir::Vector { 0.f, size_.y * .5f }, clrFrame_);
		renderer.addPoint(absPos + ir::Vector { size_.x * .5f, size_.y }, clrFrame_);

		renderer.addPoint(absPos + ir::Vector { size_.x * .5f, size_.y }, clrFrame_);
		renderer.addPoint(absPos + ir::Vector { size_.x, size_.y * .5f }, clrFrame_);

		renderer.addPoint(absPos + ir::Vector { size_.x, size_.y * .5f }, clrFrame_);
		renderer.addPoint(absPos + ir::Vector { size_.x * .5f, 0.f }, clrFrame_);

		renderer.addPoint(absPos + ir::Vector { size_.x * .5f, 0.f }, clrFrame_);
		renderer.addPoint(absPos + ir::Vector { 0.f, size_.y * .5f }, clrFrame_);

		renderer.flush();
	}
}