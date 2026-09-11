#include "log.hpp"
#include <iostream>
#include <list>
#include <thread>

namespace ir::log {
	namespace {
		struct Entry {
			std::string msg;

			enum class Level : char {
				INFO = 0,
				WARN,
				ERROR
			};
			Level level;
		};
		std::list<Entry> gEntryList;

		std::mutex gMutex;
		std::jthread gThread;
		std::atomic<bool> gIsRunning { true };

#if IRIDIUM_LOG_TO_FILE == 1
		std::unique_ptr<std::ofstream> file;
		std::streambuf* coutOriginal;
#endif

		std::string assembleMessage(Entry& e) {
			std::string str;
			str += "[";
			switch (e.level) {
				case Entry::Level::INFO: str += "INFO"; break;
				case Entry::Level::WARN: str += "WARN"; break;
				case Entry::Level::ERROR: str += "ERROR"; break;
			}
			str += "] ";
			str += e.msg;
			return str;
		}

		void update() {
			while (ir::log::gIsRunning.load()) {
				if (gEntryList.size() != 0) {
					std::lock_guard<std::mutex> lock { ir::log::gMutex };
					std::cout << ir::log::assembleMessage(*ir::log::gEntryList.begin()) << std::endl;
					ir::log::gEntryList.pop_front();
				}
			}
#if IRIDIUM_LOG_TO_FILE == 1
			std::cout.rdbuf(ir::log::coutOriginal);
			ir::log::file->close();
#endif
		}
	}

	void startSession() {
#if IRIDIUM_LOG_TO_FILE == 1
		ir::log::file = std::make_unique<std::ofstream>("EditorLog.txt");
		if (file && !file->fail()) {
			ir::log::coutOriginal = std::cout.rdbuf();
			std::cout.rdbuf(ir::log::file->rdbuf());
		}
#endif

		ir::log::gThread = std::jthread(ir::log::update);
	}

	void info(std::string_view msg) {
		std::lock_guard<std::mutex> lock { ir::log::gMutex };
		ir::log::gEntryList.push_back({ msg.data(), ir::log::Entry::Level::INFO });
	}

	void warn(std::string_view msg) {
		std::lock_guard<std::mutex> lock { ir::log::gMutex };
		ir::log::gEntryList.push_back({ msg.data(), ir::log::Entry::Level::WARN });
	}

	void error(std::string_view msg) {
		std::lock_guard<std::mutex> lock { ir::log::gMutex };
		ir::log::gEntryList.push_back({ msg.data(), ir::log::Entry::Level::ERROR });
	}

	void endSession() {
		ir::log::gIsRunning.store(false);
		ir::log::gThread.join();
	}
}