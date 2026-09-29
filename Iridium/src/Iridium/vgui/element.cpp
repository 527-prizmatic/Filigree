#include "vgui/element.hpp"
#include "rendering/rectangle.hpp"
#include "rendering/vertex_renderer.hpp"

#include "input/mouse.hpp"

namespace ir::vgui {
#pragma region Core functions
	std::unique_ptr<ir::render::Rectangle> Element::rect_;

	void Element::createRect() {
		if (!rect_) {
			rect_ = std::make_unique<ir::render::Rectangle>();
		}
	} 

	Element::Element() {
		createRect();

		if (rect_) {
			rect_->setPosition(ir::Vector { 0.f, 0.f });
			rect_->setSize(ir::Vector { 100.f, 100.f });
		}
	}

	bool Element::update(ir::input::Mouse& mouse) {
		if (!enabled_) {
			return false;
		}

		ir::Vector posAbsolute = absolutePosition();
		bool isInArea = mouse.cursorPosition().isInArea(posAbsolute, posAbsolute + size_);

		bool anyChildrenUpdated = false;
		for (auto& child : children_) {
			anyChildrenUpdated |= child.second->update(mouse);
		}

		if (!anyChildrenUpdated && isInArea) {
			if (mouse.isPressed(sf::Mouse::Button::Left)) {
				onClick();
				clickHeld_ = true;
				for (auto evt : clickEvents) {
					evt();
				}
			}
			else {
				onHover();
				for (auto evt : hoverEvents) {
					evt();
				}
			}
		}
		else {
			if (mouse.isPressed(sf::Mouse::Button::Left)) {
				onDeselect();
			}
			else {
				onIdle();
				for (auto evt : idleEvents) {
					evt();
				}
			}
		}

		if (mouse.isReleased(sf::Mouse::Button::Left) && clickHeld_) {
			onRelease();
			for (auto evt : releaseEvents) {
				evt();
			}
			clickHeld_ = false;
		}

		return anyChildrenUpdated || isInArea;
	}

	void Element::render(ir::render::VertexRenderer& renderer) const {
		renderDebugFrame(renderer);
		renderChildren(renderer);
	}
#pragma endregion

#pragma region Child management
	Element* Element::getChild(std::string key) const {
		auto it = children_.find(key);
		if (it != children_.end()) {
			return it->second.get();
		}
		return nullptr;
	}

	/*
	Element* Element::operator[](std::string key) const {
		return getChild(key);
	}

	Element* Element::operator[](const char* key) const {
		return getChild(std::string(key));
	}
	*/

	ir::vgui::Element& Element::setChildElement(std::string key, std::unique_ptr<ir::vgui::Element> child) {
		if (child) {
			child->parent_ = this;
			children_[key] = std::move(child);
			children_[key]->resizeRectangle();
			// return children_[key];
		}
		return *this;
	}

	void Element::renderChildren(ir::render::VertexRenderer& renderer) const {
		for (auto& child : children_) {
			child.second->render(renderer);
		}
	}
#pragma endregion
	
#pragma region Event management
	ir::vgui::Element& Element::registerClickEvent(ir::vgui::ClickEvent event) {
		clickEvents.push_back(std::move(event));
		return *this;
	}
	
	ir::vgui::Element& Element::registerHoverEvent(ir::vgui::ClickEvent event) {
		hoverEvents.push_back(std::move(event));
		return *this;
	}
	
	ir::vgui::Element& Element::registerIdleEvent(ir::vgui::ClickEvent event) {
		idleEvents.push_back(std::move(event));
		return *this;
	}
	
	ir::vgui::Element& Element::registerReleaseEvent(ir::vgui::ClickEvent event) {
		releaseEvents.push_back(std::move(event));
		return *this;
	}
	
	void Element::processEvent(const sf::Event& evt) {
		onSfEvent(evt);
		for (auto& child : children_) {
			child.second->processEvent(evt);
		}
	}
#pragma endregion

#pragma region Mutators and accessors
	ir::vgui::Element& Element::setPosition(ir::Vector pos) {
		pos_ = pos;
		return *this;
	}
	ir::vgui::Element& Element::setSize(ir::Vector size) {
		size_ = size;
		return *this;
	}

	ir::Vector Element::position() const { return pos_; }
	ir::Vector Element::size() const { return size_; }
	ir::Vector Element::absolutePosition() const { return parent_ != nullptr ? pos_ + parent_->absolutePosition() : pos_; }
	
	ir::vgui::Element& Element::setBackgroundColor(sf::Color clr) {
		clrBackground_ = clr;
		return *this;
	}

	sf::Color Element::backgroundColor() const { return clrBackground_; }
		
	ir::vgui::Element& Element::setFrameColor(sf::Color clr) {
		clrFrame_ = clr;
		return *this;
	}

	sf::Color Element::frameColor() const { return clrFrame_; }

	ir::vgui::Element& Element::setColors(sf::Color frame, sf::Color background) {
		clrBackground_ = background;
		clrFrame_ = frame;
		return *this;
	}

	void Element::setDebugMode(bool debug) {
		debugMode = debug;
	}

	ir::vgui::Element& Element::setEnabled(bool enabled) {
		enabled_ = enabled;
		return *this;
	}

	bool Element::enabled() const {
		return enabled_;
	}
#pragma endregion

#pragma region Internal utilities
	void Element::resizeRectangle() const {
		if (rect_) {
			rect_->setPosition(absolutePosition());
			rect_->setSize(size_);
		}
	}

	void Element::renderDebugFrame(ir::render::VertexRenderer& renderer) const {
		if (debugMode) {
			renderFrame(renderer);
		}
	}

	void Element::renderFrame(ir::render::VertexRenderer& renderer) const {
		createRect();

		if (rect_) {
			resizeRectangle();

			rect_->setMode(ir::render::Mode::SOLID);
			rect_->setColor(clrBackground_);
			rect_->render(renderer);

			rect_->setMode(ir::render::Mode::WIREFRAME);
			rect_->setColor(clrFrame_);
			rect_->render(renderer);
		}
	}
#pragma endregion

	void FramedElement::render(ir::render::VertexRenderer& renderer) const {
		renderFrame(renderer);
		renderChildren(renderer);
	}
}