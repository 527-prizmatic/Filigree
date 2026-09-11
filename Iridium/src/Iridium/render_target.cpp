#include "render_target.hpp"
#include "exceptions.hpp"

namespace ir {
	void RenderTarget::expectValid() {
		if (!isValid()) {
			throw ir::Exceptions::InvalidRenderTarget{};
		}
	}
}