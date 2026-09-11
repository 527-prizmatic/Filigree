#ifndef IRIDIUM_COLORS_HPP_
#define IRIDIUM_COLORS_HPP_

#include "libraries.hpp"

namespace ir {
	struct RGBColor;
	struct HSLColor;

	struct RGBColor {
	public:
		std::uint8_t& r { value_.r };
		std::uint8_t& g { value_.g };
		std::uint8_t& b { value_.b };
		std::uint8_t& a { value_.a };

		RGBColor();
		RGBColor(const std::uint8_t red, const std::uint8_t green, const std::uint8_t blue, const std::uint8_t alpha = 255u);
		RGBColor(const RGBColor& other);

		ir::RGBColor& operator=(const ir::RGBColor& other);

		[[nodiscard]] ir::HSLColor toHSL() const;
		[[nodiscard]] sf::Color toSfColor() const { return value_; }
		
	private:
		sf::Color value_ { sf::Color::Black };
	};

	struct HSLColor {
	public:
		std::uint8_t& h { value_.r }; ///< @todo Replace this with an unsigned short for the true 0-360 range
		std::uint8_t& s { value_.g };
		std::uint8_t& l { value_.b };
		std::uint8_t& a { value_.a };

		HSLColor();
		HSLColor(const std::uint8_t hue, const std::uint8_t sat, const std::uint8_t lum, const std::uint8_t alpha = 255u);
		HSLColor(const HSLColor& other);

		ir::HSLColor& operator=(const ir::HSLColor& other);

		[[nodiscard]] ir::RGBColor toRGB() const;
		
	private:
		sf::Color value_ { sf::Color::Black };
	};

	[[nodiscard]] inline ir::RGBColor toRGB(const ir::HSLColor hsl) { return hsl.toRGB(); }
	[[nodiscard]] inline ir::HSLColor toHSL(const ir::RGBColor rgb) { return rgb.toHSL(); }
}

#endif // IRIDIUM_COLORS_HPP_