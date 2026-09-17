#include "image_processor.hpp"

namespace filigree {
	Processor::Processor(filigree::EventQueue& evtQueue) {
		evtQueue_ = &evtQueue;
	}

	void Processor::loadSettings(ProcessorSettings settings) {
		settings_ = settings;

		filigree_ = std::make_unique<sf::Image>(settings.filigreePath);
		stamp_ = std::make_unique<sf::Image>(settings.stampPath);
	}
	
	void Processor::process(std::vector<std::filesystem::path> images, std::filesystem::path outputDir) {
		for (auto& img : images) {
			process(img, outputDir);
		}
	}

	void Processor::process(std::filesystem::path image, std::filesystem::path outputDir) {
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

		if (settings_.applyFiligree || settings_.applyStamps != ProcessorSettings::StampsToApply::NONE) {
			img = watermark(std::move(img));
		}

	//	img = addGrain(std::move(img));

		if (img->saveToFile(outputDir / image.filename())) {
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
	//	auto ret = std::make_unique<sf::Image>(img->getSize());


		
		return img;
	}

	std::unique_ptr<sf::Image> Processor::watermark(std::unique_ptr<sf::Image> img) {
		auto wm { assembleWatermark() };

		for (unsigned int x = 0; x < outputSize_.x; x++) {
			for (unsigned int y = 0; y < outputSize_.y; y++) {
				sf::Vector2u pos { x, y };
				if (wm->getPixel(pos).a > 128u) {
					sf::Color clr { img->getPixel(pos) };
					/// @todo Modify this function for more efficient and better-looking watermarking (look at how the previous iteration did it)
					clr.r = static_cast<std::uint8_t>(ir::math::clamp(static_cast<std::uint16_t>(clr.r) + 16u, 0u, 255u));
					clr.a = static_cast<std::uint8_t>(ir::math::clamp(static_cast<std::uint16_t>(clr.a) + 16u, 0u, 255u));
					img->setPixel(pos, clr);
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

		if (stamp_ && settings_.applyStamps != ProcessorSettings::StampsToApply::NONE) {
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
				if (settings_.applyStamps & ProcessorSettings::StampsToApply::BOTTOM_RIGHT) {
					wm = stampAt(std::move(wm), outputSize_.x - stamp_->getSize().x, outputSize_.y - stamp_->getSize().y);
				}

				bool xDouble { outputSize_.x > stamp_->getSize().x * 2 };
				bool yDouble { outputSize_.y > stamp_->getSize().y * 2 };

				if (yDouble && settings_.applyStamps & ProcessorSettings::StampsToApply::TOP_RIGHT) {
					wm = stampAt(std::move(wm), outputSize_.x - stamp_->getSize().x, 0);
				}

				if (xDouble && settings_.applyStamps & ProcessorSettings::StampsToApply::BOTTOM_LEFT) {
					wm = stampAt(std::move(wm), 0, outputSize_.y - stamp_->getSize().y);
				}

				if (xDouble && yDouble && settings_.applyStamps & ProcessorSettings::StampsToApply::TOP_LEFT) {
					wm = stampAt(std::move(wm), 0, 0);
				}
			}
		}

		return wm;
	}
}