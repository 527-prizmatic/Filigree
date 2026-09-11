#include "application.hpp"
#include "rendering/text.hpp"

#if __has_include("states/entry_state.hpp")
	#include "states/entry_state.hpp"
#else
	#error entry_state.hpp not found. (Have you run IrSetup.exe?)
#endif

int main() {
	ir::log::startSession();
	try {
		ir::render::Text::loadModels();
		ir::Application app;
		app.run<EntryState>();
	}
	catch (...) {
		LOG_ERROR("Something terrible happened (caught unhandled exception, exiting)");
	}
	ir::log::endSession();
}