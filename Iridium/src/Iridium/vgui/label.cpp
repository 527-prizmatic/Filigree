#include "vgui/label.hpp"
#include "rendering/text.hpp"
#include "rendering/vertex_renderer.hpp"

namespace ir::vgui {
	Label::Label(std::string text) {
		ir::render::Text::loadModels();

		label_ = std::make_unique<ir::render::Text>();
		if (label_) {
			label_->setString(text);
			label_->setScale(20.f);
		}
		else {
			LOG_ERROR("Error during creation of VGUI label \"" + text + "\"");
		}
	}

	bool Label::update(ir::input::Mouse& mouse) {
		if (parent_ != nullptr) {
			ir::Vector relativePos {};
			ir::Vector boundingBoxSize { label_->boundingBoxSize() };
			switch (anchor_) {
				default:
				case Anchor::LEFT: {
					relativePos.x = -boundingBoxSize.x - 5.f;
					relativePos.y = parent_->size().y * .5f - boundingBoxSize.y * .5f;
					break;
				}
				case Anchor::RIGHT: {
					relativePos.x = parent_->size().x + 5.f;
					relativePos.y = parent_->size().y * .5f - boundingBoxSize.y * .5f;
					break;
				}
				case Anchor::TOP: {
					relativePos.x = parent_->size().x * .5f - boundingBoxSize.x * .5f;
					relativePos.y = -boundingBoxSize.y - 5.f;
					break;
				}
				case Anchor::BOTTOM: {
					relativePos.x = parent_->size().x * .5f - boundingBoxSize.x * .5f;
					relativePos.y = parent_->size().y  + 5.f;
					break;
				}
				case Anchor::OVER: {
					relativePos.x = parent_->size().x * .5f - label_->boundingBoxSize().x * .5f;
					relativePos.y = parent_->size().y * .5f - label_->boundingBoxSize().y * .5f;
					break;
				}
			}
			pos_ = relativePos;
			size_ = boundingBoxSize;
		}

	//	return Element::update(mouse);
		return false;
	}

	void Label::render(ir::render::VertexRenderer& renderer) const {
		renderDebugFrame(renderer);

		if (label_) {
			label_->setPosition(absolutePosition());
			label_->render(renderer);
		}
		
		renderChildren(renderer);
	}

	void Label::setScale(float scale) {
		if (label_) {
			label_->setScale(scale);
		}
	}

	float Label::scale() {
		if (label_) {
			return label_->scale();
		}
		return -1.f;
	}

	void Label::setLabel(std::string text) {
		if (label_) {
			label_->setString(text);
		}
	}

	std::string Label::label() const {
		if (label_) {
			return label_->string();
		}
		return "ERROR_LABEL";
	}

	void Label::setColor(sf::Color clr) {
		if (label_) {
			label_->setColor(clr);
		}
	}
	
	sf::Color Label::color() const {
		if (label_) {
			return label_->color();
		}
		return sf::Color::Transparent;
	}

	void Label::setAnchor(Label::Anchor anchor) {
		anchor_ = anchor;
	}

	Label::Anchor Label::anchor() const {
		return anchor_;
	}

	void Label::setPosition(ir::Vector pos) {
		if (parent_ == nullptr) {
			pos_ = pos;
		}
	}
}