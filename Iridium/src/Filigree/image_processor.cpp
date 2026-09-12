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
		sf::Image img { image };

		for (unsigned int x = 0; x < img.getSize().x; x++) {
			for (unsigned int y = 0; y < img.getSize().y; y++) {
				sf::Color clr { img.getPixel(sf::Vector2u { x, y }) };
				clr.r = rand() % 64u;
				img.setPixel(sf::Vector2u { x, y }, clr);
			}
		}

		if (img.saveToFile(outputDir / image.filename())) {
			LOG_INFO(std::string { "File " } + image.filename().string() + " processed successfully");
		}
		else {
			LOG_INFO(std::string { "Failed processing file " } + image.filename().string());
		}
	}
}