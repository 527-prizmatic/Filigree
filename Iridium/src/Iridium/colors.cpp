#include "colors.hpp"

namespace ir {
	namespace {
		inline constexpr float inv255 { 1.f / 255.f }; ///< Used for division optimization
	}

#pragma region RGBColor
	RGBColor::RGBColor() {
		value_ = sf::Color::Black;
	}

	RGBColor::RGBColor(const std::uint8_t red, const std::uint8_t green, const std::uint8_t blue, const std::uint8_t alpha) {
		r = red;
		g = green;
		b = blue;
		a = alpha;
	}
	
	RGBColor::RGBColor(const RGBColor& other) {
		r = other.r;
		g = other.g;
		b = other.b;
		a = other.a;
	}
	
	ir::RGBColor& RGBColor::operator=(const ir::RGBColor& other) {
		r = other.r;
		g = other.g;
		b = other.b;
		a = other.a;
		return *this;
	}

	[[nodiscard]] ir::HSLColor RGBColor::toHSL() const {
		ir::HSLColor hsl;

		float rConv { static_cast<float>(r) * ir::inv255 };
		float gConv { static_cast<float>(g) * ir::inv255 };
		float bConv { static_cast<float>(b) * ir::inv255 };

		float minF { ir::math::min(rConv, gConv, bConv) };
		float maxF { ir::math::max(rConv, gConv, bConv) };

		int maxI { ir::math::max(r, g, b) };

		float h { 0.f };
		float s { 0.f };
		float l { (minF + maxF) * .5f };

		if (ir::math::min(r, g, b) != maxI) {
			float diffF { maxF - minF };
			if (l <= .5f) {
				s = diffF / (maxF + minF);
			}
			else {
				s = diffF / (2.f - (maxF + minF));
			}

			if (maxI == r) {
				h = 0.f + (gConv - bConv)/ diffF;
			}
			else if (maxI == g) {
				h = 2.f + (bConv - rConv)/ diffF;
			}
			else /* if (maxI == rgb.b) */ {
				h = 4.f + (rConv - gConv)/ diffF;
			}

			h *= 60.f;
			if (h < 0.f) {
				h += 360.f;
			}
		}

		hsl.h = { static_cast<std::uint8_t>(h * 255.f / 360.f) };
		hsl.s = { static_cast<std::uint8_t>(s * 255.f) };
		hsl.l = { static_cast<std::uint8_t>(l * 255.f) };
		hsl.a = a;
		return hsl;
	}
#pragma endregion

#pragma region HSLColor
	HSLColor::HSLColor() {
		value_ = sf::Color::Black;
	}

	HSLColor::HSLColor(const std::uint8_t hue, const std::uint8_t sat, const std::uint8_t lum, const std::uint8_t alpha) {
		h = hue;
		s = sat;
		l = lum;
		a = alpha;
	}

	HSLColor::HSLColor(const HSLColor& other) {
		h = other.h;
		s = other.s;
		l = other.l;
		a = other.a;
	}

	ir::HSLColor& HSLColor::operator=(const ir::HSLColor& other) {
		h = other.h;
		s = other.s;
		l = other.l;
		a = other.a;
		return *this;
	}

	[[nodiscard]] ir::RGBColor HSLColor::toRGB() const {
		ir::RGBColor rgb;

		float hF { static_cast<float>(h * 6.f) * ir::inv255 };
		float sF { static_cast<float>(s) * ir::inv255 };
		float lF { static_cast<float>(l) * ir::inv255 };

		float hMod { hF };
		while (hMod > 2.f) {
			hMod -= 2.f;
		}

		float c { (1.f - std::abs(2.f * lF - 1.f)) * sF };
		float x { c * (1.f - std::abs(hMod - 1.f)) };
		float m { lF - c * .5f };

		std::uint8_t iC { static_cast<std::uint8_t>(c * 255u) };
		std::uint8_t iX { static_cast<std::uint8_t>(x * 255u) };
		std::uint8_t iM { static_cast<std::uint8_t>(m * 255u) };

		if (hF < 1.f) {
			rgb = ir::RGBColor(iC, iX, 0);
		}
		else if (hF < 2.f) {
			rgb = ir::RGBColor(iX, iC, 0);
		}
		else if (hF < 3.f) {
			rgb = ir::RGBColor(0, iC, iX);
		}
		else if (hF < 4.f) {
			rgb = ir::RGBColor(0, iX, iC);
		}
		else if (hF < 5.f) {
			rgb = ir::RGBColor(iX, 0, iC);
		}
		else /* if (hF < 6.f) */ {
			rgb = ir::RGBColor(iC, 0, iX);
		}

		rgb.r += iM;
		rgb.g += iM;
		rgb.b += iM;

		rgb.a = a;
		return rgb;
	}
#pragma endregion
}