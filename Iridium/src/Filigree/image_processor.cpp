#include "image_processor.hpp"

namespace filigree {
	Processor::Processor(filigree::EventQueue& evtQueue) {
		evtQueue_ = &evtQueue;
	}
	
	void Processor::process(std::vector<std::filesystem::path> images, std::filesystem::path outputDir) {
		for (auto& img : images) {
			process(img, outputDir);
		}
	}

	void Processor::process(std::filesystem::path image, std::filesystem::path outputDir) {
		auto img = std::make_unique<sf::Image>(image);

		/*
		for (unsigned int x = 0; x < img.getSize().x; x++) {
			for (unsigned int y = 0; y < img.getSize().y; y++) {
				sf::Color clr { img.getPixel(sf::Vector2u { x, y }) };
				clr.r = rand() % 64u;
				img.setPixel(sf::Vector2u { x, y }, clr);
			}
		}
		*/

		auto grainy { addGrain(std::move(img)) };

		if (grainy->saveToFile(outputDir / image.filename())) {
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
		auto ret = std::make_unique<sf::Image>(img->getSize());


		
		return ret;
	}

	std::unique_ptr<sf::Image> Processor::watermark(std::unique_ptr<sf::Image> img) {
		auto ret = std::make_unique<sf::Image>(img->getSize());


		
		return ret;
	}
}