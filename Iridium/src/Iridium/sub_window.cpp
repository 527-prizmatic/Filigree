#include "sub_window.hpp"
#include "exceptions.hpp"
#include "rendering/quad.hpp"

namespace ir {
	SubWindow::SubWindow(ir::Vector size_) {
		allocateResources(size_);
	}
	
	void SubWindow::setSize(ir::Vector size_) {
		allocateResources(size_);
	}

	ir::Vector SubWindow::size() {
		expectValid();

		return ir::Vector::fromSFMLVector(renderTexture_->getSize());
	}

	void SubWindow::clear(sf::Color fill_color) {
		expectValid();

		renderTexture_->clear(fill_color);
	}

	void SubWindow::render(sf::Drawable &drawable, const sf::Texture* texture) {
		expectValid();
			
		renderTexture_->draw(drawable, texture);
	}

	void SubWindow::render(ir::RenderTarget &renderTarget) {
		expectValid();
		
		if (ir::SubWindow* sub = dynamic_cast<ir::SubWindow*>(&renderTarget)) {
			sub->flushToTarget(*this);
		}
	}
	
	void SubWindow::flushToTarget(ir::RenderTarget& renderTarget) {
		if (!renderTarget.isValid() || !isValid()) {
			throw ir::Exceptions::InvalidRenderTarget{};
		}
		
		rect_->setPosition(position_ + ir::Vector{0.f, size().y});
		rect_->render(*renderTarget.getRenderer());

		if (renderFrame_) {
			frame_->setPosition(position_ + ir::Vector(1.f, 0.f));
			frame_->render(*renderTarget.getRenderer());
		}
	}

	void SubWindow::allocateResources() {
		expectValid();

		allocateResources(ir::Vector::fromSFMLVector(renderTexture_->getSize()));
	}

	void SubWindow::allocateResources(ir::Vector size) {
		if (renderTexture_ != nullptr) {
			renderTexture_.reset();
		}

		renderTexture_ = std::make_unique<sf::RenderTexture>(sf::Vector2u{size});
		configureRect(size);
		configureFrame(size);
	}

	void SubWindow::configureRect(ir::Vector size) {
		if (rect_ == nullptr) {
			rect_ = std::make_unique<ir::render::Quad>();
		}

		rect_->setTexture(renderTexture_->getTexture());
		rect_->setSize(size.mirrorX());
	}
	
	void SubWindow::configureFrame(ir::Vector size) {
		if (frame_ == nullptr) {
			frame_ = std::make_unique<ir::render::Rectangle>();
		}
		frame_->setSize(size.x - 1.f, size.y - 1.f); /// -1 on each dimension so it precisely lines up with the edges of the subwindow
	}

	void SubWindow::setRenderFrameColor(sf::Color clr) {
		if (!isValid()) {
			throw ir::Exceptions::InvalidRenderTarget{};
		}
		
		frame_->setColor(clr);
	}
}