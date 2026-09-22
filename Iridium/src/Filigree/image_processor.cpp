#include "image_processor.hpp"

namespace filigree {
	Processor::Processor(filigree::EventQueue& evtQueue) {
		evtQueue_ = &evtQueue;
	}

	void Processor::loadSettings(ProcessorSettings settings) {
		settings_ = settings;

		filigree_ = std::make_unique<sf::Image>(settings.pathFiligree);
		stamp_ = std::make_unique<sf::Image>(settings.pathStamp);
	}
	
	void Processor::process(std::vector<std::filesystem::path> images) {
		for (auto& img : images) {
			process(img);
		}
	}

	void Processor::process(std::filesystem::path image) {
		auto img = std::make_unique<sf::Image>(image);
		outputSize_ = ir::Vector::fromSFMLVector(img->getSize());

		/*
		for (unsigned int x = 0; x < img.getSize().x; x++) {
			for (unsigned int y = 0; y < img.getSize().y; y++) {
				sf::Color clr { img.getPixel(sf::Vector2u { x, y }) };
				clr.r = rand() % 64u;
				img.setPixel(sf::Vector2u { x, y }, clr);
			}
		}
		*/

		if (settings_.resize) {
			img = resize(std::move(img));
		}

		/// Relevant settings checks done inside
		img = watermark(std::move(img));

	//	img = addGrain(std::move(img));

		if (img->saveToFile(settings_.pathOutput / image.filename())) {
			LOG_INFO(std::string { "File " } + image.filename().string() + " processed successfully");
		}
		else {
			LOG_INFO(std::string { "Failed processing file " } + image.filename().string());
		}
	}

	std::unique_ptr<sf::Image> Processor::addGrain(std::unique_ptr<sf::Image> img) {
		auto ret = std::make_unique<sf::Image>(img->getSize());

		for (unsigned int x = 0; x < img->getSize().x; x++) {
			for (unsigned int y = 0; y < img->getSize().y; y++) {
				sf::Color clr { img->getPixel(sf::Vector2u { x, y }) };

				int interval { static_cast<int>(grainStrength * 255) };
				clr.r = static_cast<unsigned int>(ir::math::clamp(rand() % (interval * 2) - interval + static_cast<int>(clr.r), 0, 255));
				clr.g = static_cast<unsigned int>(ir::math::clamp(rand() % (interval * 2) - interval + static_cast<int>(clr.g), 0, 255));
				clr.b = static_cast<unsigned int>(ir::math::clamp(rand() % (interval * 2) - interval + static_cast<int>(clr.b), 0, 255));

				ret->setPixel(sf::Vector2u { x, y }, clr);
			}
		}
		return ret;
	}

	std::unique_ptr<sf::Image> Processor::resize(std::unique_ptr<sf::Image> img) {
		auto originalSize { img->getSize() };
		sf::Vector2u newSize {};

		if (originalSize.x < originalSize.y) {
			newSize.y = settings_.resizeSize;
			float ratio { static_cast<float>(newSize.y) / static_cast<float>(originalSize.y) };
			newSize.x = originalSize.x * ratio;
		}
		else if (originalSize.x > originalSize.y) {
			newSize.x = settings_.resizeSize;
			float ratio { static_cast<float>(newSize.x) / static_cast<float>(originalSize.x) };
			newSize.y = originalSize.y * ratio;
		}
		else { // if (originalSize.y == originalSize.x)
			newSize = { static_cast<unsigned int>(settings_.resizeSize), static_cast<unsigned int>(settings_.resizeSize) };
		}

		auto ret = std::make_unique<sf::Image>(newSize);

		auto randomRounding { [](float val) -> unsigned int {
			unsigned int rounded { static_cast<unsigned int>(val) };
			unsigned int threshold { static_cast<unsigned int>((val - static_cast<float>(rounded)) * 1000.f) };
			return rounded + ((rand() % 200u + 400u) < threshold ? 1 : 0);
		}};

		for (unsigned int x = 0; x < newSize.x; x++) {
			float xRatio { static_cast<float>(x) / static_cast<float>(newSize.x) };
			for (unsigned int y = 0; y < newSize.y; y++) {
				float yRatio { static_cast<float>(y) / static_cast<float>(newSize.y) };

				sf::Vector2u mappedOriginalPos {
					ir::math::clamp(randomRounding(originalSize.x * xRatio), 0u, originalSize.x - 1),
					ir::math::clamp(randomRounding(originalSize.y * yRatio), 0u, originalSize.y - 1)
				};

				ret->setPixel(sf::Vector2u { x, y }, img->getPixel(mappedOriginalPos));
			}
		}

		outputSize_ = ir::Vector::fromSFMLVector(newSize);
		
		return ret;
	}

	std::unique_ptr<sf::Image> Processor::watermark(std::unique_ptr<sf::Image> img) {
		ir::Vector imageCenter { outputSize_.x * .5f, outputSize_.y * .5f };

		auto colorBlend { [&](sf::Color clrI, sf::Color clrF, float opacity, unsigned int x, unsigned int y) -> sf::Color {
			if (clrF.a > 128) {
				ir::HSLColor clrFHSL { static_cast<std::uint8_t>(sqrt(ir::math::pow2(x - imageCenter.x) + ir::math::pow2(y - imageCenter.y)) * .25f), 40u, 128u }; ///< HSL watermark, hue-adjusted for distance from image center
				sf::Color clrFinal { clrFHSL.toRGB().toSfColor() }; ///< RGB equivalent
				
				sf::Color clrO { sf::Color::Transparent }; ///< Output color

				/// Force image color to hue-adjusted watermark color in fully transparent areas
				if (clrI.a == 0u) {
					clrI = { clrFinal.r, clrFinal.g, clrFinal.b, 0u };
				}

				/// Interpolate between image and hue-adjusted watermark
				clrO.r = ir::math::interpolate(clrI.r, clrFinal.r, opacity);
				clrO.g = ir::math::interpolate(clrI.g, clrFinal.g, opacity);
				clrO.b = ir::math::interpolate(clrI.b, clrFinal.b, opacity);

				if (clrI.a == 255u) { /// Fully opaque areas stay opaque
					clrO.a = 255u;
				}
				else { /// Others either keep their original opacity or take the watermark's, whichever is higher
					clrO.a = std::max(ir::math::interpolate(clrI.a, static_cast<std::uint8_t>(255u), opacity), static_cast<std::uint8_t>(clrF.a * opacity));
				}
				
				/// Slightly darken watermarked areas of high lightness, so the watermark pops more
				ir::HSLColor clrIHSL { ir::RGBColor { clrI.r, clrI.g, clrI.b, clrI.a }.toHSL() };
				ir::HSLColor clrOHSL { ir::RGBColor { clrO.r, clrO.g, clrO.b, clrO.a }.toHSL() };
				if (int d { clrIHSL.l - clrOHSL.l }; d < 16 && d >= 0) {
					clrOHSL.l -= std::min((16 - d), static_cast<int>(opacity * 100));
					clrO = clrOHSL.toRGB().toSfColor();
				}

				return clrO;
			}
			else {
				return clrI;
			}
		} };

		auto wm { assembleWatermark() };

		for (unsigned int x = 0; x < outputSize_.x; x++) {
			for (unsigned int y = 0; y < outputSize_.y; y++) {
				sf::Vector2u pos { x, y };
				if (wm->getPixel(pos).a > 128u) {
					sf::Color clrImg { img->getPixel(pos) };
					sf::Color clrWatermark { wm->getPixel(pos) };
					
					img->setPixel(pos, colorBlend(clrImg, clrWatermark, settings_.watermarkOpacity, x, y));
				}
			}
		}

		return img;
	}

	std::unique_ptr<sf::Image> Processor::assembleWatermark() {
		auto wm { std::make_unique<sf::Image>(static_cast<sf::Vector2u>(outputSize_), sf::Color::Transparent) };

		if (filigree_ && settings_.applyFiligree) {
			for (unsigned int x = 0; x < outputSize_.x; x++) {
				unsigned int xMod { x % filigree_->getSize().x };
				for (unsigned int y = 0; y < outputSize_.y; y++) {
					sf::Color clr { filigree_->getPixel(sf::Vector2u { xMod, y % filigree_->getSize().y }) };
					if (clr.a > 128u) {
						wm->setPixel(sf::Vector2u { x, y }, sf::Color::Black);
					}
					else {
						wm->setPixel(sf::Vector2u { x, y }, sf::Color::Transparent);
					}
				}
			}
		}

		auto stampAt = [&](std::unique_ptr<sf::Image> img, int posX, int posY) -> std::unique_ptr<sf::Image> {
			sf::Vector2i sizeStampI { stamp_->getSize() };
			sf::Vector2i sizeImageI { img->getSize() };
			for (int x = -10; x < sizeStampI.x + 10; x++) {
				int xOff { x + posX };
				for (int y = -10; y < sizeStampI.y + 10; y++) {
					int yOff { y + posY };

					if (xOff >= 0 && yOff >= 0 && xOff < sizeImageI.x && yOff < sizeImageI.y) {
						sf::Color clr;

						if (x >= 0 && y >= 0 && x < sizeStampI.x && y < sizeStampI.y) {
							clr = stamp_->getPixel(sf::Vector2u { static_cast<unsigned int>(x), static_cast<unsigned int>(y) }).a > 128u ? sf::Color::Black : sf::Color::Transparent;
						}
						else {
							clr = sf::Color::Transparent;
						}

						img->setPixel(sf::Vector2u { static_cast<unsigned int>(xOff), static_cast<unsigned int>(yOff) }, clr);
					}
				}
			}

			return img;
		};

		if (outputSize_.x > stamp_->getSize().x && outputSize_.y > stamp_->getSize().y) {
			if (settings_.applyStampBR) {
				wm = stampAt(std::move(wm), outputSize_.x - stamp_->getSize().x, outputSize_.y - stamp_->getSize().y);
			}

			bool xDouble { outputSize_.x > stamp_->getSize().x * 2 };
			bool yDouble { outputSize_.y > stamp_->getSize().y * 2 };

			if (yDouble && settings_.applyStampTR) {
				wm = stampAt(std::move(wm), outputSize_.x - stamp_->getSize().x, 0);
			}

			if (xDouble && settings_.applyStampBL) {
				wm = stampAt(std::move(wm), 0, outputSize_.y - stamp_->getSize().y);
			}

			if (xDouble && yDouble && settings_.applyStampTL) {
				wm = stampAt(std::move(wm), 0, 0);
			}
		}

		return wm;
	}
}