#ifndef IRIDIUM_VGUI_FACTORY_HPP_
#define IRIDIUM_VGUI_FACTORY_HPP_

#include "libraries.hpp"
#include "vector.hpp"
#include "vgui/element.hpp"
#include "vgui/icon.hpp"

namespace ir::vgui {
	inline std::unique_ptr<ir::vgui::FramedElement> makeIconButton(ir::Vector size, std::string path, sf::Color clr) {
		auto button { std::make_unique<ir::vgui::FramedElement>() };
		button->setPosition(ir::Vector { 423.f, 2.f })
			.setSize(size)
			.setColors(clr, sf::Color(clr.r, clr.g, clr.b, 32u));
			
		auto icon { std::make_unique<ir::vgui::Icon>(path) };
		icon->setScale(ir::math::min(size.x, size.y) - 4.f)
			.setFrameColor(clr)
			.setPosition(ir::Vector { 2.f, 2.f });

		button->setChildElement("Icon", std::move(icon));
		return button;
	}

	inline ir::vgui::Label* addLabel(ir::vgui::Element* parent, std::string label, ir::vgui::Label::Anchor anchor) {
		auto l { parent->addChildElement<ir::vgui::Label>("Label", label) };
		l->setAnchor(anchor);
		return l;
	}
}

#endif // IRIDIUM_VGUI_FACTORY_HPP_