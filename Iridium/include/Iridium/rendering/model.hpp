#ifndef IRIDIUM_RENDERING_MODEL_HPP_
#define IRIDIUM_RENDERING_MODEL_HPP_

#include "libraries.hpp"
#include "rendering/shape.hpp"

namespace ir {
	namespace render {
		struct Vertex {
			short x;
			short y;
			sf::Color color;

			void setPosition(short x, short y) {
				this->x = x;
				this->y = y;
			}

			[[nodiscard]] sf::Vector2i position() const { return sf::Vector2i { x, y }; }
		};

		struct Component {
			enum class Type : unsigned char {
				POINT = 0,
				LINE = 1,
				TRIANGLE = 2
			};
			Type type;
			std::vector<Vertex> vertices;

			Component();
			Component(Vertex v1);
			Component(Vertex v1, Vertex v2);
			Component(Vertex v1, Vertex v2, Vertex v3);

			inline Component& setPosition(size_t vertex, short x, short y) {
				vertices[vertex].setPosition(x, y);
				return *this;
			}

			inline Component& setColor(size_t vertex, sf::Color color) {
				vertices[vertex].color = color;
				return *this;
			}
			
			auto begin() { return vertices.begin(); }
			auto begin() const { return vertices.begin(); }
			auto end() { return vertices.end(); }
			auto end() const { return vertices.end(); }
		};

		/// @brief Data storage for VMF models.
		/// 
		/// VMF (Vector Model Format) is a file format designed for use in Iridium. It stores OpenGL vector graphics data
		/// under the form of groups of coordinate-color pairs. Each such group corresponds to a single graphical element
		/// (a line, a point, or a triangle). Coordinates are stored on a discrete pixel grid, rather than floating-point.
		/// 
		/// Models must be passed to an instance of ir::render::ModelRenderer for on-screen drawing. This utility class
		/// comes with farious functions to change orientation, scale, add a global color filter, and more.
		class Model {
		public:
			Model() = default;
			
			/// @brief Appends a component at the end of the component list.
			/// @return Index of the newly added component
			size_t addComponent(Component&& cmp);

			/// @brief Attempts to load a VMF model from a .vmf file placed in resources\models.
			/// @param file File name
			static Model loadFromFile(std::filesystem::path file);

			/// @brief Saves VMF model data into a .vmf file. The saved file is placed in resources\models.
			/// @param file File path
			/// @return Whether the model was saved successfully
			bool saveToFile(std::filesystem::path file);

			/// @return Dummy model for testing VMF APIs
			static Model testTriangle();

			[[nodiscard]] size_t componentCount() const { return components_.size(); } ///< @return Number of components (points, lines and triangles) in the model
			[[nodiscard]] const Component& component(size_t index) const { return components_[index]; } ///< @return Component data of the component with requested index

			void removeLastComponent() {
				if (components_.size() > 0) {
					components_.pop_back();
				}
			}
			
			auto begin() { return components_.begin(); } ///< @brief For iterating over component list
			auto begin() const { return components_.begin(); } ///< @brief For iterating over component list
			auto end() { return components_.end(); } ///< @brief For iterating over component list
			auto end() const { return components_.end(); } ///< @brief For iterating over component list

			[[nodiscard]] unsigned int width() const; ///< @return Width of the model in grid units
			[[nodiscard]] unsigned int height() const; ///< @return Height of the model in grid units

		private:
			std::vector<Component> components_;
		};
	}
}

#endif