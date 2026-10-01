#ifndef FILIGREE_GUI_CURSOR_HPP_
#define FILIGREE_GUI_CURSOR_HPP_

#include <libraries.hpp>
#include <random.hpp>
#include <math.hpp>
#include <vector.hpp>
#include <rendering/vertex_renderer.hpp>

namespace filigree::gui {
	struct MouseParticle {
		ir::Vector position {};
		ir::Vector velocity {};
		float angle {};
		float radius { 5.f };
		float lifetime { 1.f };
		float lifetimeMax { 1.f };
		float rotationSpeed { 0.f };
		float opacity { 1.f };

		MouseParticle(ir::Vector pos, ir::Vector vel);

		void update(float dt);
		void render(ir::render::VertexRenderer& renderer) const;
	};

	class MouseCursor {
	public:
		MouseCursor();

		void update(const ir::Vector mousePos, float dt);
		void render(ir::render::VertexRenderer& renderer) const;
		void onClick();

		void base() { mode_ = Mode::BASE; }
		void loading() { mode_ = Mode::LOADING; }


	private:
		void renderBase(ir::render::VertexRenderer& renderer) const;
		void renderLoading(ir::render::VertexRenderer& renderer) const;

		enum class Mode {
			BASE,
			LOADING,
		};
		Mode mode_ { Mode::BASE };

		ir::Vector pos_ {};
		ir::Vector currentVelocity_ {};
		float angle_ { 0.f };

		float rotationSpeed_ { ir::math::tau * .25f };
		static constexpr float kRadius_ { 15.f };

		std::list<filigree::gui::MouseParticle> particles_ {};
	};
}

#endif // FILIGREE_GUI_CURSOR_HPP_