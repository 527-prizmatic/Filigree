#include "rendering/model.hpp"
#include "exceptions.hpp"

namespace ir {
	namespace render {
		Component::Component() {
			vertices.resize(3);
		}

		Component::Component(Vertex v1) {
			vertices.resize(1);
			type = Component::Type::POINT;
			vertices[0] = v1;
		}

		Component::Component(Vertex v1, Vertex v2) {
			vertices.resize(2);
			type = Component::Type::LINE;
			vertices[0] = v1;
			vertices[1] = v2;
		}

		Component::Component(Vertex v1, Vertex v2, Vertex v3) {
			vertices.resize(3);
			type = Component::Type::TRIANGLE;
			vertices[0] = v1;
			vertices[1] = v2;
			vertices[2] = v3;
		}

		size_t Model::addComponent(Component&& cmp) {
			components_.push_back(cmp);
			return components_.size() - 1;
		}

		Model Model::loadFromFile(std::filesystem::path file) {
			std::filesystem::path path { "..\\resources\\models\\" + file.string() + ".vmf" };
			path = path.lexically_normal();

			std::ifstream stream(path, std::ios::binary);
			if (stream.fail()) {
				throw ir::Exceptions::BadModelName(file.string());
			}

			Model model;

			char c;
			while (!stream.eof()) {
				stream.read(&c, 1);
				if (stream.eof()) {
					break;
				}

				if (*reinterpret_cast<unsigned char*>(&c) > 2) {
					throw "domaci";
				}

				Component cmp;
				cmp.type = static_cast<ir::render::Component::Type>(c);

				size_t iterations = c;
				for (size_t i = 0; i <= iterations; i++) {
					Vertex v { 0, 0, sf::Color::White };
					stream.read(reinterpret_cast<char*>(&v), sizeof(v));
					cmp.vertices[i] = v;
				}

				model.addComponent(std::move(cmp));
			}
			stream.close();

			LOG_INFO("Loaded model " + path.string());
			return model;
		}

		bool Model::saveToFile(std::filesystem::path file) {
			std::filesystem::path path { "..\\resources\\models\\" + file.string() + ".vmf" };
			path = path.lexically_normal();

			std::ofstream stream(path, std::ios::binary);
			if (!stream.fail()) {
				for (auto& cmp : components_) {
					stream.write(reinterpret_cast<char*>(&cmp.type), 1);
					for (int j = 0; j <= static_cast<int>(cmp.type); j++) {
						stream.write(reinterpret_cast<char*>(&cmp.vertices[j]), sizeof(ir::render::Vertex));
					}
				}
				stream.close();
				
				LOG_INFO("Saved model " + path.string());
				return true;
			}
			
			LOG_WARN("Error while trying to save model " + path.string());
			return false;
		}

		Model Model::testTriangle() {
			Model model;

			Vertex v1(-100, 0, sf::Color(255, 0, 0, 64));
			Vertex v2(50, 83, sf::Color(0, 255, 0, 64));
			Vertex v3(50, -83, sf::Color(0, 0, 255, 64));

			model.addComponent(std::move(Component(v1, v2, v3)));
			model.addComponent(std::move(Component(v1, v2)));
			model.addComponent(std::move(Component(v2, v3)));
			model.addComponent(std::move(Component(v1, v3)));

			return model;
		}
		
		unsigned int Model::width() const {
			int min { 32767 }, max { -32768 };
			
			for (auto& c : components_) {
				for (auto& v : c) {
					if (v.x < min) {
						min = v.x;
					}
					if (v.x > max) {
						max = v.x;
					}
				}
			}	

			return max - min;

		}

		unsigned int Model::height() const {
			int min { 32767 }, max { -32768 };

			for (auto& c : components_) {
				for (auto& v : c) {
					if (v.y < min) {
						min = v.y;
					}
					if (v.y > max) {
						max = v.y;
					}
				}
			}	

			return max - min;
		}
	}
}