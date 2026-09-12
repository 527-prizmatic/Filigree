#ifndef FILIGREE_PROCESSOR_HPP_
#define FILIGREE_PROCESSOR_HPP_

#include <vector>
#include <libraries.hpp>

#include "events.hpp"

namespace filigree {
	class Processor {
	public:
		Processor(filigree::EventQueue& evtQueue);

		void process(std::vector<std::filesystem::path> images, std::filesystem::path outputDir);
		void process(std::filesystem::path image, std::filesystem::path outputDir);
	
	private:
		filigree::EventQueue* evtQueue_ { nullptr };

		// Settings
	};
}

#endif // FILIGREE_PROCESSOR_HPP_