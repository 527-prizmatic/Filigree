#include "vgui/slider.hpp"
#include "rendering/vertex_renderer.hpp"
#include "rendering/rectangle.hpp"

#include "input/mouse.hpp"

namespace ir::vgui {
	Slider::Slider(int lowerBound, int upperBound) {
		bounds_.lower_ = lowerBound;
		bounds_.upper_ = upperBound;
		value_ = lowerBound;
	}

	bool Slider::update(ir::input::Mouse& mouse) {
		if (clickHeld_) {
			ir::Vector posBar1 { absolutePosition() + ir::Vector { kBarMargin, size_.y * .5f - kBarWidth * .5f } };
			
			float xMin { posBar1.x }; ///< X position of bar's left edge
			float xMax { posBar1.x + size_.x - kBarMargin * 2.f }; ///< X position of bar's right edge

			float mouseX { ir::math::clamp(mouse.cursorPosition().x, xMin, xMax) };

			float ratio { (mouseX - xMin) / (xMax - xMin) };
			int newVal { static_cast<int>(ir::math::interpolate(static_cast<float>(bounds_.lower_ < 0 ? bounds_.lower_ - 1 : bounds_.lower_), static_cast<float>(bounds_.upper_ < 0 ? bounds_.upper_ - 1 : bounds_.upper_), ratio) + .5f) };
			value_ = newVal;
		}

		return ir::vgui::Element::update(mouse);
	}

	void Slider::render(ir::render::VertexRenderer& renderer) const {
		renderDebugFrame(renderer);

		if (rect_) {
			ir::Vector posBar1 { absolutePosition() + ir::Vector { kBarMargin, size_.y * .5f - kBarWidth * .5f } };
			
			float xMin { posBar1.x }; ///< X position of bar's left edge
			float xMax { posBar1.x + size_.x - kBarMargin * 2.f }; ///< X position of bar's right edge
			float xCursor { ir::math::interpolate(xMin + 4.f, xMax - 4.f, getValueRatio()) - kCursorWidth * .5f }; ///< Cursor X position

			/// Bar background
			rect_->setMode(ir::render::Mode::SOLID);
			rect_->setPosition(posBar1);
			rect_->setSize(ir::Vector { xCursor - xMin + kCursorWidth * .5f, kBarWidth });
			rect_->setColor(clrBackground_);
			rect_->render(renderer);

			/// Bar frame
			rect_->setMode(ir::render::Mode::WIREFRAME);
			rect_->setSize(ir::Vector { size_.x - kBarMargin * 2.f, kBarWidth });
			rect_->setColor(clrFrame_);
			rect_->render(renderer);

			/// Cursor background
			rect_->setMode(ir::render::Mode::SOLID);
			rect_->setPosition(ir::Vector { xCursor, posBar1.y + (kBarWidth - kCursorHeight) * .5f });
			rect_->setSize(ir::Vector { kCursorWidth, kCursorHeight });
			rect_->setColor(clickHeld_ ? sf::Color::Red : clrBackground_);
			rect_->render(renderer);

			/// Cursor frame
			rect_->setMode(ir::render::Mode::WIREFRAME);
			rect_->setColor(clrFrame_);
			rect_->render(renderer);
		}

		renderChildren(renderer);
	}

	void Slider::setValue(int val) {
		value_ = val;
		clampValue();
	}

	int Slider::value() const { return value_; }

	void Slider::setUpperBound(int upper){
		bounds_.upper_ = upper;
		clampValue();
	}

	int Slider::upperBound() const { return bounds_.upper_; }

	void Slider::setLowerBound(int lower) {
		bounds_.lower_ = lower;
		clampValue();
	}

	int Slider::lowerBound() const { return bounds_.lower_; }

	void Slider::clampValue() {
		if (value_ < bounds_.lower_) {
			value_ = bounds_.lower_;
		}
		if (value_ > bounds_.upper_) {
			value_ = bounds_.upper_;
		}
	}

	float Slider::getValueRatio() const {
		return static_cast<float>(value_ - bounds_.lower_) / static_cast<float>(bounds_.upper_ - bounds_.lower_);
	}
}