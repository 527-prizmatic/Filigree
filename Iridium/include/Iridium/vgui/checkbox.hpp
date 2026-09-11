#ifndef IRIDIUM_VGUI_CHECKBOX_HPP_
#define IRIDIUM_VGUI_CHECKBOX_HPP_

#include "vgui/element.hpp"

namespace ir::vgui {
	/// @brief VGUI checkbox, toggleable with a left mouse click.
	class Checkbox : public Element {
	public:
		virtual void render(ir::render::VertexRenderer& renderer) const override;
		virtual void onIdle() override;
		virtual void onHover() override;
		virtual void onClick() override;
		virtual void onDeselect() override;

		void setEnabled(bool enabled) { enabled_= enabled; }
		[[nodiscard]] bool isEnabled() const { return enabled_; } ///< @return Whether the checkbox is ticked

	private:
		void renderCheckbox(ir::render::VertexRenderer& renderer) const;

		bool enabled_;
	};
}

#endif // IRIDIUM_VGUI_CHECKBOX_HPP_