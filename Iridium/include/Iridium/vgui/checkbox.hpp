#ifndef IRIDIUM_VGUI_CHECKBOX_HPP_
#define IRIDIUM_VGUI_CHECKBOX_HPP_

#include "vgui/element.hpp"

namespace ir::vgui {
	/// @brief VGUI checkbox, toggleable with a left mouse click.
	class Checkbox : public Element {
	public:
		Checkbox();

		virtual void render(ir::render::VertexRenderer& renderer) const override;
		virtual void onIdle() override;
		virtual void onHover() override;
		virtual void onClick() override;
		virtual void onDeselect() override;

		ir::vgui::Checkbox& setChecked(bool checked);
		[[nodiscard]] bool checked() const { return checked_; } ///< @return Whether the checkbox is ticked

		inline static void setDefaultSize(ir::Vector size) { defaultSize_ = size; }
		[[nodiscard]] inline static ir::Vector defaultSize() { return defaultSize_; }

	private:
		void renderCheckbox(ir::render::VertexRenderer& renderer) const;

		bool checked_ { false };

		inline static ir::Vector defaultSize_ { 26.f, 26.f };
	};
}

#endif // IRIDIUM_VGUI_CHECKBOX_HPP_