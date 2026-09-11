#ifndef IRIDIUM_RENDERING_TEXT_HPP_
#define IRIDIUM_RENDERING_TEXT_HPP_

#include "rendering/model_renderer.hpp"

namespace ir::render {
	class Text : public ModelRenderer {
	public:
		void setString(std::string str);
		[[nodiscard]] std::string string() const { return string_; }

		void setColor(sf::Color color) { color_ = color; }
		[[nodiscard]] sf::Color color() const { return color_; }

		static void loadModels();

		void setModel(Model&& model) = delete;
		[[nodiscard]] Model& model() = delete;

		/// @brief This override of setScale takes into account letter model height, so that a scale of 10 means a height of 10 pixels.
		Text& setScale(float scale) override;
		/// @brief This override of scale takes into account letter model height, so that a scale of 10 means a height of 10 pixels.
		[[nodiscard]] float scale() const override;

	private:
		std::string string_;
		sf::Color color_ { sf::Color::White };

		/// @todo This may be a good starting point for a VMF font format.
		static std::unordered_map<char, std::unique_ptr<Model>> modelSet_;

		void addVertexToBuffer(const Vertex& v, ir::render::VertexRenderer& renderer) const override;
	};
}

#endif // IRIDIUM_RENDERING_TEXT_HPP_