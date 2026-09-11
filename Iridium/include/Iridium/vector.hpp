#ifndef IRIDIUM_VECTOR_HPP_
#define IRIDIUM_VECTOR_HPP_

namespace ir {
	struct Vector {
	public:
		float x; /// x component
		float y; /// y component

		Vector();

		/// @brief Constructs a vector from cartesian coordinates.
		Vector(float x, float y);

		/// @brief Constructs a vector from a SFML vector.
		template <typename T> static ir::Vector fromSFMLVector(sf::Vector2<T> other) {
			return ir::Vector(static_cast<float>(other.x), static_cast<float>(other.y));
		}

		/// @brief Copies a vector
		Vector(const ir::Vector& other);

		ir::Vector& operator=(const ir::Vector& other);
		
		/// @brief Constructs a cartesian-space vector from polar coordinates.
		[[nodiscard]] static ir::Vector polar(float radius, float angle);

		/// @return Whether both the vector's components are equal to zero 
		[[nodiscard]] bool isZero() const;

		/// @return Vector's magnitude
		/// @note As computing magnitude requires an expensive square root, consider using magnitudeSquare if possible.
		[[nodiscard]] float magnitude() const;

		/// @return Vector's square magnitude
		[[nodiscard]] float magnitudeSquare() const;

		/// @return Angle between the vector and another one. If no reference vector is provided, the angle returned will be relative to the X axis.
		[[nodiscard]] float angle(ir::Vector reference = ir::Vector{1.f, 0.f}) const;

		/// @return Distance between this vector and another one
		[[nodiscard]] float distance(ir::Vector other) const;
		
		/// @return This vector, normalized
		[[nodiscard]] ir::Vector normalize() const;

		/// @brief Clamps the vector's magnitude below the given threshold.
		[[nodiscard]] ir::Vector clampMagnitude(float max) const;

		/// @brief Clamps the vector's magnitude between the given thresholds.
		[[nodiscard]] ir::Vector clampMagnitude(float min, float max) const;

		/// @brief Converts a cartesian-space vector into polar vector space.
		[[nodiscard]] ir::Vector recToPol() const;
		
		/// @brief Converts a polar-space vector into cartesian vector space.
		[[nodiscard]] ir::Vector polToRec() const;

		/// @brief Rotates about the given angle.
		[[nodiscard]] ir::Vector rotate(float angle) const;

		/// @brief Rotates about the given angle, using a point as reference.
		[[nodiscard]] ir::Vector rotate(float angle, ir::Vector reference) const;

		/// @return Axial mirror image of the vector about the given symmetry axis
		[[nodiscard]] ir::Vector mirror(ir::Vector axis) const;

		/// @return Axial mirror image of the vector about the X axis
		[[nodiscard]] ir::Vector mirrorX() const;

		/// @return Axial mirror image of the vector about the Y axis
		[[nodiscard]] ir::Vector mirrorY() const;

		/// @return Opposite of the vector
		[[nodiscard]] ir::Vector negate() const;

		template <typename T>
		[[nodiscard]] explicit operator sf::Vector2<T>() const {
			return sf::Vector2<T>{ static_cast<T>(x), static_cast<T>(y) };
		}
		
		[[nodiscard]] operator std::string() const {
			return '(' + std::to_string(x) + ' ' + std::to_string(y) + ')';
		}

		// [[nodiscard]] inline explicit operator sf::Vector2i() const {
		// 	return sf::Vector2i{ static_cast<int>(x), static_cast<int>(y) };
		// }

		// [[nodiscard]] inline explicit operator sf::Vector2u() const {
		// 	return sf::Vector2u{ static_cast<unsigned int>(x), static_cast<unsigned int>(y) };
		// }
		
		// [[nodiscard]] inline explicit operator sf::Vector2f() const {
		// 	return sf::Vector2f{ x, y };
		// }
		
		[[nodiscard]] inline ir::Vector operator+(const ir::Vector& other) const {
			return ir::Vector { x + other.x, y + other.y };
		}
		
		[[nodiscard]] inline ir::Vector operator-(const ir::Vector& other) const {
			return ir::Vector { x - other.x, y - other.y };
		}
		
		[[nodiscard]] inline ir::Vector operator-() const {
			return ir::Vector { -x, -y };
		}
		
		[[nodiscard]] inline ir::Vector operator*(const float other) const {
			return ir::Vector { x * other, y * other };
		}
		
		[[nodiscard]] inline ir::Vector operator/(const float other) const {
			return ir::Vector { x / other, y / other };
		}
		
		[[nodiscard]] inline float operator*(const ir::Vector& other) const {
			return x * other.x + y * other.y;
		}
		
		
		inline void operator+=(const ir::Vector& other) {
			x += other.x;
			y += other.y;
		}
		
		inline void operator-=(const ir::Vector& other) {
			x -= other.x;
			y -= other.y;
		}
		
		inline void operator*=(const float other) {
			x *= other;
			y *= other;
		}
		
		inline void operator/=(const float other) {
			x /= other;
			y /= other;
		}
		
		[[nodiscard]] inline bool operator==(const ir::Vector& other) const {
			return (*this - other).isZero();
		}

		[[nodiscard]] inline bool isInArea(const ir::Vector& corner1, const ir::Vector& corner2) const {
			ir::Vector topLeft { std::min(corner1.x, corner2.x), std::min(corner1.y, corner2.y) };
			ir::Vector bottomRight { std::max(corner1.x, corner2.x), std::max(corner1.y, corner2.y) };

			return x >= topLeft.x && x <= bottomRight.x
			&& y >= topLeft.y && y <= bottomRight.y;
		}

		static const ir::Vector kZero;
		static const ir::Vector kUnit;
	};
}

#endif // IRIDIUM_VECTOR_HPP_