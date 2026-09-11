#ifndef IRIDIUM_DEVUTILS_HPP_
#define IRIDIUM_DEVUTILS_HPP_

#include "libraries.hpp"

namespace ir {
	/// @brief Runs a snippet of code a large number of times and prints total execution time to console.
	/// @param _func Function to run
	/// @param _loops Number of times to run the function
	inline void benchmark(std::function<void(void)> func, unsigned long loops = 1'000'000, std::string testName = "") {
		std::chrono::steady_clock clock;
		auto startTime = clock.now();

		for (unsigned long a = 0; a < loops; a++) {
			func();
		}

		auto endTime = clock.now();
		if (testName.compare(""))
			std::cout << "[" << testName << "] ";
		std::cout << std::to_string(loops) << " operations took " << std::to_string((endTime - startTime).count()) << "ns to complete" << std::endl;
	}
}

#endif // IRIDIUM_DEVUTILS_HPP_