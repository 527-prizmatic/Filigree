#ifndef IRIDIUM_RANDOM_HPP_
#define IRIDIUM_RANDOM_HPP_

#include "libraries.hpp"

namespace ir {
	namespace Random {
		/// @return Uniformly random 32-bit integer
		int integer32();

		/// @return Uniformly random 32-bit integer within [0, max)
		/// @note If max is inferior or equal to 0, this function returns 0.
		int range(int max);
		
		/// @return Uniformly random 32-bit integer within [_min, max)
		/// @note If max is inferior or equal to min, this function returns min.
		int range(int min, int max);

		/// @param chance Probability to return true, clamped between 0 and 1
		/// @return True with the given probability; false otherwise
		bool chance(float chance);

		/// @brief Computes an idealized dice roll result.
		/// @param count Number of dice to roll
		/// @param size Number of sides to each die
		/// @note If count is inferior or equal to 0, or if size is inferior or equal to 1, this function returns 0.
		int diceRoll(int count, int size);
		
		/// @brief Computes an idealized dice roll result.
		/// @param roll Text notation of a die roll in the form XdY, where X is the number of dice to roll and Y the number of sides
		/// @note If for any reason the provided string is not a valid die roll, this function returns 0.
		int diceRoll(std::string roll);
	}
}




#endif // IRIDIUM_RANDOM_HPP_