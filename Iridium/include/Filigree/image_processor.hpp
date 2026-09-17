#ifndef FILIGREE_PROCESSOR_HPP_
#define FILIGREE_PROCESSOR_HPP_

#include <vector>
#include <libraries.hpp>

#include "events.hpp"
#include "gui/settings.hpp"

namespace filigree {
	class Processor {
	public:
		enum class Settings {
			WATERMARK,	///< Apply image-wide watermark
			STAMP,		///< Apply stamps in image corners
			RESIZE,		///< Resize image to target resolution
		};

		Processor(filigree::EventQueue& evtQueue);

		void loadSettings(ProcessorSettings settings);
		void process(std::vector<std::filesystem::path> images, std::filesystem::path outputDir);
		void process(std::filesystem::path image, std::filesystem::path outputDir);

		std::unique_ptr<sf::Image> addGrain(std::unique_ptr<sf::Image> img);
		std::unique_ptr<sf::Image> resize(std::unique_ptr<sf::Image> img);
		std::unique_ptr<sf::Image> watermark(std::unique_ptr<sf::Image> img);
		
	private:	
		std::unique_ptr<sf::Image> assembleWatermark();

		std::unique_ptr<sf::Image> watermark_;
		std::unique_ptr<sf::Image> stamp_;

		ProcessorSettings settings_;
		
		filigree::EventQueue* evtQueue_ { nullptr };
		float grainStrength { .01f };

		// Settings
	};
}

#endif // FILIGREE_PROCESSOR_HPP_