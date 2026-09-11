#include "rendering/text.hpp"
#include "exceptions.hpp"
#include <sstream>

namespace ir::render {
	namespace {
		constexpr float kModelHeight = 9.f; ///< Used in scale get/set functions
	}

	std::unordered_map<char, std::unique_ptr<Model>> Text::modelSet_;
	
	std::string charToModelName(unsigned char c);
	unsigned char modelNameToChar(std::string name);
	Model placeholderLetter();

	void Text::setString(std::string str) {
		string_ = str;
		float offset { 0.f };
		model_ = Model();
		for (size_t i = 0; i < str.size(); i++) {
			if (modelSet_.contains(str[i])) {
				Model& letter = *modelSet_[str[i]];
				for (auto cmp : letter) {
					for (auto& v : cmp.vertices) {
						v.x += offset;
					}
					model_.addComponent(std::move(cmp));
				}
					offset += letter.width() + 1.f;
			}
			else {
				offset += 5.f;
			}

		}
	}

	void Text::addVertexToBuffer(const Vertex& v, ir::render::VertexRenderer& renderer) const {
		Vertex vNew { v };
		vNew.color *= color_;
		ModelRenderer::addVertexToBuffer(vNew, renderer);
	}

	void Text::loadModels() {
		static bool done { false };
		if (!done) {
			std::string folder { "..\\text\\" };
			unsigned char c = ' ';
			while (c != 255u) {
				std::string name = charToModelName(c);
				if (name != "none") {
					try {
						Model model { Model::loadFromFile(folder + name) };
						modelSet_.emplace(c, std::make_unique<ir::render::Model>(std::move(model)));
					}
					catch (ir::Exceptions::BadModelName& exc) {
						LOG_ERROR(exc.what());
						modelSet_.emplace(c, std::make_unique<ir::render::Model>(std::move(placeholderLetter())));
					}
				}
				c++;
			}

			done = true;
		}
		else {
			LOG_WARN("Attempted to load text models multiple times");
		}
	}

	Text& Text::setScale(float scale) { 
		scale_ = scale / kModelHeight;
		return *this;
	}

	float Text::scale() const {
		return scale_ * kModelHeight;
	}

	std::string charToModelName(unsigned char c) {
		std::stringstream sstr;

		if (c >= 'a' && c <= 'z') {
			sstr << static_cast<char>(c);
			sstr << "_lower";
		}
		else if (c >= 'A' && c <= 'Z') {
			sstr << static_cast<char>(c + 32u);
			sstr << "_upper";
		}
		else if (c >= '0' && c <= '9') {
			sstr << "digit_" << c;
		}
		else if (c == '_') {
			sstr << "underscore";
		}
		else if (c == '.') {
			sstr << "period";
		}
		else if (c == ',') {
			sstr << "comma";
		}
		else if (c == '-') {
			sstr << "hyphen";
		}
		else if (c == '!') {
			sstr << "excl";
		}
		else if (c == '?') {
			sstr << "question";
		}
		else if (c == '(') {
			sstr << "l_bracket";
		}
		else if (c == ')') {
			sstr << "r_bracket";
		}
		else if (c == ':') {
			sstr << "colon";
		}
		else if (c == ';') {
			sstr << "semicolon";
		}
		else if (c == '/') {
			sstr << "slash";
		}
		else {
			sstr << "none";
		}
		return sstr.str();
	}

	unsigned char modelNameToChar(std::string name) {
		return 'a';
	}

	Model placeholderLetter() {
		Model model {};
		model.addComponent(ir::render::Component(ir::render::Vertex{ 0, 2, sf::Color::White }, ir::render::Vertex{ 6, 2, sf::Color::White }));
		model.addComponent(ir::render::Component(ir::render::Vertex{ 0, 2, sf::Color::White }, ir::render::Vertex{ 0, 11, sf::Color::White }));
		model.addComponent(ir::render::Component(ir::render::Vertex{ 0, 11, sf::Color::White }, ir::render::Vertex{ 6, 11, sf::Color::White }));
		model.addComponent(ir::render::Component(ir::render::Vertex{ 6, 2, sf::Color::White }, ir::render::Vertex{ 6, 11, sf::Color::White }));
		return model;
	}
}