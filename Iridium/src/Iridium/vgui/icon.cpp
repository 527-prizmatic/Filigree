#include "vgui/icon.hpp"
#include "rendering/model_renderer.hpp"

namespace ir::vgui {
	Icon::Icon() {
		modelRenderer_ = std::make_unique<ir::render::ModelRenderer>();
	}

	Icon::Icon(std::filesystem::path filename) {
		modelRenderer_ = std::make_unique<ir::render::ModelRenderer>();
		setIcon(filename);
	}

	bool Icon::update(ir::input::Mouse& mouse) {
		return false;
	}

	void Icon::render(ir::render::VertexRenderer& renderer) const {
		if (modelRenderer_) {
			modelRenderer_->setColor(clrFrame_);
			modelRenderer_->setPosition(absolutePosition());
			modelRenderer_->render(renderer);
		}
	}

	void Icon::setScale(float scale) {
		scale_ = scale;
		if (modelRenderer_) {
			unsigned int modelSize { std::max(modelRenderer_->model().height(), modelRenderer_->model().width()) };
			modelRenderer_->setScale(scale_ / static_cast<float>(modelSize));
		}
	}

	void Icon::setIcon(std::filesystem::path filename) {
		if (modelRenderer_) {
			modelRenderer_->setModel(ir::render::Model::loadFromFile(filename));
			setScale(scale_);
		}
	}
}