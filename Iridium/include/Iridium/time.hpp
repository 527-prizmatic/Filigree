#ifndef IRIDIUM_TIME_HPP_
#define IRIDIUM_TIME_HPP_

#include "libraries.hpp"

namespace ir {	
	using ClockType = std::chrono::steady_clock;
	using Timestamp = std::chrono::time_point<std::chrono::steady_clock>;

	/// @brief Time management subsystem.
	class GameClock {
	public:
		GameClock();

		void zero(bool resetTimeScale = false);
		void startTick();

		void setTimeScale(float timeScale);

		float getDeltaTime(); ///< @return Scaled time elapsed during last frame, in seconds
		float getDeltaTimeUnscaled(); ///< @return Unscaled time elapsed during last frame, in seconds
		float getTimeScale();

	private:
		ClockType clock_;
		Timestamp last_;
		float deltaUnscaled_ { 0.f };
		float deltaScaled_ { 0.f };
		float timeScale_ { 1.f };
		bool isFirstTick_ { true };
	};
}

#endif // IRIDIUM_TIME_HPP_