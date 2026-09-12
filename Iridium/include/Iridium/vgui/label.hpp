#ifndef IRIDIUM_VGUI_LABEL_HPP_
#define IRIDIUM_VGUI_LABEL_HPP_

#include <string>
#include "vgui/element.hpp"
// #include "rendering/text.hpp"

namespace ir {
	namespace render {
		class Text;
	}

	namespace vgui {
		/// @brief VGUI element for rendering VMF-based text as UI labels.
		/// @note Labels block updates, and do not prevent their parent from updating.
		class Label : public Element {
		public:
			enum class Anchor : unsigned char {
				NONE,
				LEFT,
				RIGHT,
				TOP,
				BOTTOM,
				OVER
			};

			Label(std::string text = "label");

			virtual bool update(ir::input::Mouse& mouse) override;
			virtual void render(ir::render::VertexRenderer& renderer) const override;

			ir::vgui::Label& setScale(float scale);
			float scale();

			ir::vgui::Label& setLabel(std::string text);
			[[nodiscard]] std::string label() const;

			ir::vgui::Label& setColor(sf::Color clr);
			[[nodiscard]] sf::Color color() const;

			ir::vgui::Label& setAnchor(Anchor anchor);
			[[nodiscard]] Anchor anchor() const;
			
			virtual ir::vgui::Element& setPosition(ir::Vector pos) override;

		protected:
			std::unique_ptr<ir::render::Text> label_;
			Anchor anchor_;
		};
	}
}
	
#endif // IRIDIUM_VGUI_LABEL_HPP_