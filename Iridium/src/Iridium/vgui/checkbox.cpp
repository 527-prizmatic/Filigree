#include "vgui/checkbox.hpp"
#include "rendering/vertex_renderer.hpp"

namespace ir::vgui {
	Checkbox::Checkbox() {
		size_ = defaultSize_;
	}

	void Checkbox::onIdle() {
		clrBackground_ = sf::Color::Transparent;
	}

	void Checkbox::onHover() {
		clrBackground_ = sf::Color(255, 255, 255, 64);
	}

	void Checkbox::onClick() {
		checked_ = !checked_;
	}

	void Checkbox::onDeselect() {
	//	enabled_ = false;
	}

	void Checkbox::render(ir::render::VertexRenderer& renderer) const {
		renderFrame(renderer);
		if (checked_) {
			renderCheckbox(renderer);
		}
		renderChildren(renderer);
	}

	void Checkbox::renderCheckbox(ir::render::VertexRenderer& renderer) const {
		ir::Vector absPos { absolutePosition() };

		renderer.reset(sf::PrimitiveType::TriangleFan);
		renderer.addPoint(absPos + ir::Vector { 4.f, 5.f }, clrFrame_);
		renderer.addPoint(absPos + ir::Vector { 4.f, size_.y - 4.f }, clrFrame_);
		renderer.addPoint(absPos + ir::Vector { size_.x - 5.f, size_.y - 4.f }, clrFrame_);
		renderer.addPoint(absPos + ir::Vector { size_.x - 5.f, 5.f }, clrFrame_);
		renderer.flush();
	}

	ir::vgui::Checkbox& Checkbox::setChecked(bool checked) {
		checked_= checked;
		return *this;
	}
}