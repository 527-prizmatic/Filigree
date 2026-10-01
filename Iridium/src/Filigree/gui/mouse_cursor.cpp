#include "gui/mouse_cursor.hpp"
#include <colors.hpp>

namespace filigree::gui {
	namespace detail {
		void renderStar(ir::render::VertexRenderer& renderer, ir::Vector pos, float angle, float radius, float opacity = 1.f) {
			static constexpr float tau4 { ir::math::tau * .25f };
			const sf::Color clrLines { 255u, 255u, 255u, static_cast<std::uint8_t>(opacity * 255.f) };

			for (int i = 0; i < 4; i++) {
				ir::Vector offset1 { ir::Vector { radius, 0.f }.rotate(angle + tau4 * static_cast<float>(i)) };
				ir::Vector offset2 { ir::Vector { radius * .4f, 0.f }.rotate(angle + tau4 * (static_cast<float>(i) + .5f)) };
				ir::Vector offset3 { ir::Vector { radius, 0.f }.rotate(angle + tau4 * static_cast<float>(i + 1)) };
				
				ir::Vector dirSide { offset2 * 2.5f }; ///< Direction of side 1
				ir::Vector dirLight { ir::Vector::polar(radius, ir::math::tau * -.125f) }; ///< Lighting direction
				float maxDot { dirLight.magnitudeSquare() }; ///< Max possible value for dot product

				renderer.reset(sf::PrimitiveType::Triangles);

				sf::Color clr { 255u, 255u, 255u, 0u };
				clr.a = 128.f * opacity * std::abs(ir::math::pow3(ir::Vector::dot(dirLight, dirSide) / maxDot * .5f + .5f)); ///< Compute lighting intensity
				
				renderer.addPoint(pos, clr);
				renderer.addPoint(pos + offset1, clr);
				renderer.addPoint(pos + offset2, clr);

				renderer.addPoint(pos, clr);
				renderer.addPoint(pos + offset2, clr);
				renderer.addPoint(pos + offset3, clr);

				renderer.flush();

				renderer.reset(sf::PrimitiveType::Lines);
				renderer.addPoint(pos + offset1, clrLines);
				renderer.addPoint(pos + offset2, clrLines);
				renderer.addPoint(pos + offset2, clrLines);
				renderer.addPoint(pos + offset3, clrLines);
				renderer.flush();
			}

			renderer.flush();
		}
	}

#pragma region MouseParticle class
	MouseParticle::MouseParticle(ir::Vector pos, ir::Vector vel) {
		position = pos;
		velocity = vel + ir::Vector { ir::Random::range(-1000, 1000) * .1f, ir::Random::range(-1000, 1000) * .1f };

		angle = ir::Random::range(628) * .01f; ///< Rough 0-tau random (temporary solution)
		radius = ir::math::pow3(ir::Random::range(50) * .1f) * .04f;
		lifetimeMax = ir::Random::range(radius * 10) * .01f + 1.f;
		lifetime = lifetimeMax;

		rotationSpeed = ir::Vector::dot(velocity.normalize(), vel.normalize().rotate(ir::math::pi * .5f)) * 50.f;
	}

	void MouseParticle::update(float dt) {
		rotationSpeed *= std::powf(.25f, dt);
		velocity *= std::powf(.05f, dt);

		angle += rotationSpeed * dt;
		position += velocity * dt;
		lifetime -= dt;
		opacity = ir::math::clamp(ir::math::max(lifetime, 0.f) / lifetimeMax, 0.f, 1.f);
	}

	void MouseParticle::render(ir::render::VertexRenderer& renderer) const {
		detail::renderStar(renderer, position, angle, radius, opacity);
	}
#pragma endregion

#pragma region MouseCursor class
	MouseCursor::MouseCursor() {

	}
	
	void MouseCursor::update(const ir::Vector mousePos, float dt) {
		ir::Vector posOffset { mousePos - pos_ };

		/// Reads cursor trajectory and adds torque accordingly (speeds up / slows down rotation with cursor movement)
		ir::Vector prevVelocity = currentVelocity_;
		currentVelocity_ = posOffset / dt;
		float dot { ir::Vector::dot(prevVelocity.normalize(), currentVelocity_.normalize().rotate(ir::math::pi * -.5f)) };
		rotationSpeed_ += dot * .25f;
		rotationSpeed_ = ir::math::interpolate(rotationSpeed_, (mode_ == Mode::LOADING) ? (-ir::math::tau) : (ir::math::tau * .25f), dt * .75f);

		pos_ = mousePos;

		/// Rotation and clamps angle to [0, 2T[
		angle_ += MouseCursor::rotationSpeed_ * dt;
		if (angle_ >= ir::math::tau) {
			angle_ -= ir::math::tau;
		}
		if (angle_ < 0.f) {
			angle_ += ir::math::tau;
		}
		
		/// Spawns particles around cursor (more at faster cursor speeds)
		float chance { .01f + std::min(400.f, currentVelocity_.magnitude()) * .002f };
		if (ir::Random::chance(chance)) {
			particles_.push_back(filigree::gui::MouseParticle { pos_, currentVelocity_ * .5f });
		}

		/// Updates particles
		for (auto itr = particles_.begin(); itr != particles_.end(); ++itr) {
			itr->update(dt);
			if (itr->lifetime < 0.f) {
				itr = particles_.erase(itr);
			}
		}
	}

	void MouseCursor::render(ir::render::VertexRenderer& renderer) const {
		for (auto itr = particles_.begin(); itr != particles_.end(); ++itr) {
			itr->render(renderer);
		}

		switch (mode_) {
			default:
			case Mode::BASE: {
				renderBase(renderer);
				break;
			}
			case Mode::LOADING: {
				renderLoading(renderer);
				break;
			}
		}
	}

	void MouseCursor::onClick() {
			for (int i = 0; i < 50; i++) {
				particles_.push_back(filigree::gui::MouseParticle { pos_, ir::Vector { ir::Random::range(2000, 3000) * .1f, 0.f }.rotate(ir::Random::range(628) * .01f) });
			}
		}

	void MouseCursor::renderBase(ir::render::VertexRenderer& renderer) const {
		detail::renderStar(renderer, pos_, angle_, kRadius_);
	}

	void MouseCursor::renderLoading(ir::render::VertexRenderer& renderer) const {
		auto getHue { [](int i, float angle) -> std::uint8_t {
			int hue = 32 * i;
			hue += static_cast<int>(255.f / ir::math::tau * angle);
			return static_cast<std::uint8_t>(hue % 256);
		} };

		float pulseFactor { .1f * std::sinf(angle_) };
		float effectiveRadius { kRadius_ * (1.f + pulseFactor) };
		float innerRadius { .5f * (1.f - pulseFactor * 2.f) };

		for (int i = 0; i < 8; i++) {
			ir::HSLColor clrHSL { getHue(i, angle_), 255u, 128u };
			ir::Vector off1 { ir::Vector { effectiveRadius, 0.f }.rotate(-ir::math::tau / 8.f * static_cast<float>(i)) };
			ir::Vector off2 { ir::Vector { effectiveRadius, 0.f }.rotate(-ir::math::tau / 8.f * static_cast<float>(i + 1)) };

			sf::Color clr { clrHSL.toRGB().toSfColor() };
			sf::Color clrTransp { clr.r, clr.g, clr.b, 64u };

			renderer.reset(sf::PrimitiveType::TriangleStrip);
			renderer.addPoint(pos_ + off1, clrTransp);
			renderer.addPoint(pos_ + off1 * innerRadius, clrTransp);
			renderer.addPoint(pos_ + off2, clrTransp);
			renderer.addPoint(pos_ + off2 * innerRadius, clrTransp);
			renderer.flush();

			renderer.reset(sf::PrimitiveType::Lines);
			renderer.addPoint(pos_ + off1, clr);
			renderer.addPoint(pos_ + off2, clr);
			renderer.addPoint(pos_ + off1 * innerRadius, clr);
			renderer.addPoint(pos_ + off2 * innerRadius, clr);
			renderer.flush();
		}
	}
#pragma endregion
}