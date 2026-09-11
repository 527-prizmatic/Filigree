#include "time.hpp"

namespace ir {
#pragma region GameClock
	GameClock::GameClock() {
		last_ = clock_.now();
	}

	void GameClock::zero(bool resetTimeScale) {
		clock_ = ClockType();
		last_ = clock_.now();
		deltaUnscaled_ = 0.f;
		deltaScaled_ = 0.f;

		if (resetTimeScale) {
			setTimeScale(1.f);
		}
	}

	void GameClock::startTick() {
		Timestamp now = clock_.now();
		deltaUnscaled_ = static_cast<float>((now - last_).count() * 1e-9f);
		deltaScaled_ = deltaUnscaled_ * timeScale_;
		last_ = now;
		clock_ = ClockType{};
	}

	float GameClock::getDeltaTime() {
		return deltaScaled_;
	}

	float GameClock::getDeltaTimeUnscaled() {
		return deltaUnscaled_;
	}

	void GameClock::setTimeScale(float timeScale) {
		timeScale_ = timeScale;
		deltaScaled_ = deltaUnscaled_ * timeScale_;
	}

	float GameClock::getTimeScale() {
		return timeScale_;
	}
#pragma endregion
}
