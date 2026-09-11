#ifndef IRIDIUM_VGUI_SLIDER_HPP_
#define IRIDIUM_VGUI_SLIDER_HPP_

#include "vgui/element.hpp"

namespace ir::vgui {
	/// @brief VGUI slider, for selecting numerical values by moving a cursor along a horizontal line.
	class Slider : public Element {
	public:
		Slider(int lowerBound = 0, int upperBound = 10);

		virtual bool update(ir::input::Mouse& mouse) override;
		virtual void render(ir::render::VertexRenderer& renderer) const override;

		void setValue(int val);
		[[nodiscard]] int value() const;

		void setUpperBound(int upper);
		[[nodiscard]] int upperBound() const;

		void setLowerBound(int lower);
		[[nodiscard]] int lowerBound() const;

	private:
		void clampValue();
		[[nodiscard]] float getValueRatio() const; ///< @brief Reverse-interpolates the current value along the slider's interval

		struct {
			int upper_ { 0 };
			int lower_ { 10 };
		} bounds_;
		int value_;
		
		static inline constexpr float kBarWidth { 8.f };
		static inline constexpr float kBarMargin { 5.f };
		static inline constexpr float kCursorHeight { 20.f };
		static inline constexpr float kCursorWidth { 10.f };
	};
}

#endif // IRIDIUM_VGUI_SLIDER_HPP_