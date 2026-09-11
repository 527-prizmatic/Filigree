#include "random.hpp"
#include <random>

namespace ir {
	namespace Random {
		namespace detail {
			std::mt19937 gGenerator { static_cast<unsigned int>(std::time(nullptr)) };
		}

		int integer32() {
			return ir::Random::detail::gGenerator();
		}

		int range(int max) {
			if (max <= 0) {
				return 0;
			}
			return ir::Random::detail::gGenerator() % max;
		}

		int range(int min, int max) {
			if (max <= min + 1) {
				return min;
			}
			return min + ir::Random::detail::gGenerator() % (max - min);
		}

		bool chance(float chance) {
			return ir::Random::range(1'000'000) < static_cast<int>(ir::math::clamp(chance, 0.f, 1.f) * 1'000'000);
		}

		int diceRoll(int count, int size) {
			if (count <= 0 || size <= 2) {
				return 0;
			}
			int result { 0 };
			for (int i { 0 }; i < count; i++) {
				result += ir::Random::range(1, size + 1);
			}
			return result;
		}

		int diceRoll(std::string roll) {
			std::size_t position = roll.find('d');
			Ensures(position != roll.npos);
			try {
				int count = position == 0 ? 1 : std::stoi(roll.substr(0, position));
				int size = std::stoi(roll.substr(position + 1));
				return ir::Random::diceRoll(count, size);
			}
			catch (std::exception& e) {
				return 0;
			}
		}
	}
}