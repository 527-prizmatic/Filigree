#ifndef IRIDIUM_MATH_HPP_
#define IRIDIUM_MATH_HPP_

#include <cmath>

namespace ir {
	namespace math {
		inline constexpr float pi = 3.14159;
		inline constexpr float tau = 2.f * pi;
		inline constexpr float euler = 2.71828;
		inline constexpr float phi = 1.61803;
		inline constexpr float kDegToRadRatio = math::pi / 180.f;
		inline constexpr float kRadToDegRatio = 180.f / math::pi;

		/// @brief Verifies if a variable is an arithmetic type (int, char, short, float, double, etc. and CV-qualified variants).
		template <typename T> concept Number = std::is_arithmetic<T>::value;

		template <Number T> [[nodiscard]] constexpr bool isZero(T n) noexcept {
			return abs(n) <= std::numeric_limits<T>::epsilon();
		}

		template <Number T> [[nodiscard]] constexpr T powi(T a, int exp) noexcept {
			Expects(exp >= 0);
			if (!exp) [[unlikely]] return { 1 };

			T result { 1. };
			while (1) {
				if (exp & 0b1)
					result *= a;
				exp >>= 1;
				if (!exp)
					break;
				a *= a;
			}

			return result;
		}

		/// @brief Quick square function, using straightforward multiplications to avoid the more complex operations of math::powi.
		template <Number T> [[nodiscard]] constexpr T pow2(const T a) { return a * a; }
		/// @brief Quick cube function, using straightforward multiplications to avoid the more complex operations of math::powi.
		template <Number T> [[nodiscard]] constexpr T pow3(const T a) { return a * a * a; }

		/// @brief Clamps a value between two bounds.
		template <Number T> [[nodiscard]] constexpr T clamp(const T val, const T lower, const T upper) noexcept {
			if (lower > upper) [[unlikely]] return val;
			if (lower == upper) [[unlikely]] return lower;
			if (val < lower) return lower; 
			if (val > upper) return upper;
			return val;
		}

		/// @brief Interpolates linearly between two values.
		template <Number T> [[nodiscard]] constexpr T interpolate(const T a, const T b, const float t) noexcept {
			return t * (b - a) + a;
		}

		/// @brief Compares two numbers of the same type and returns the smallest one.
		template <Number T> [[nodiscard]] constexpr T min(const T a, const T b) noexcept { return std::min(a, b); }
		/// @brief Compares a set of numbers of the same type and returns the smallest one.
		template <Number T, typename... Args> [[nodiscard]] constexpr T min(const T a, Args... args) noexcept {
			return ir::math::min(a, ir::math::min(args...));
		}

		/// @brief Compares two numbers of the same type and returns the greatest one.
		template <Number T> constexpr T max(const T a, const T b) noexcept { return std::max(a, b); }
		/// @brief Compares a set of numbers of the same type and returns the greatest one.
		template <Number T, typename... Args> [[nodiscard]] constexpr T max(const T a, Args... args) noexcept {
			return ir::math::max(a, ir::math::max(args...));
		}

		/// @brief Converts an angle from degrees to radians.
		[[nodiscard]] inline constexpr float degToRad(float degrees) noexcept {
			return degrees * ir::math::kDegToRadRatio;
		}
		
		/// @brief Converts an angle from radians to degrees.
		[[nodiscard]] inline constexpr float radToDeg(float radians) noexcept {
			return radians * ir::math::kRadToDegRatio;
		}
	}
}

#endif // IRIDIUM_MATH_HPP_