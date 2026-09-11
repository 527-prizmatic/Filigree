#include "rendering/model_renderer.hpp"
#include "rendering/vertex_renderer.hpp"

namespace ir {
	namespace render {
		void ModelRenderer::render(ir::render::VertexRenderer& renderer) const {
			for (auto& cmp : model_) {
				if (cmp.type == Component::Type::POINT) {
					renderer.reset(sf::PrimitiveType::Points);
					addVertexToBuffer(cmp.vertices[0], renderer);

				} else if (cmp.type == Component::Type::LINE) {
					renderer.reset(sf::PrimitiveType::Lines);
					addVertexToBuffer(cmp.vertices[0], renderer);
					addVertexToBuffer(cmp.vertices[1], renderer);

				} else if (cmp.type == Component::Type::TRIANGLE) {
					renderer.reset(sf::PrimitiveType::Triangles);
					addVertexToBuffer(cmp.vertices[0], renderer);
					addVertexToBuffer(cmp.vertices[1], renderer);
					addVertexToBuffer(cmp.vertices[2], renderer);
				}
				renderer.flush();
			}
		}
		
		void ModelRenderer::addVertexToBuffer(const Vertex &v, ir::render::VertexRenderer& renderer) const {
			ir::Vector position { static_cast<float>(v.x), static_cast<float>(v.y) };
			position = position.rotate(angle_) * scale_;
			position += position_ - anchor_;
			
			renderer.addPoint(position, v.color * color_);
		}

		ir::Vector ModelRenderer::boundingBoxSize() {
			ir::Vector posMin { 0.f, 0.f };
			ir::Vector posMax { 0.f, 0.f };
			for (auto& cmp : model_) {
				for (auto& v : cmp) {
					posMin.x = ir::math::min(static_cast<short>(posMin.x), v.x);
					posMin.y = ir::math::min(static_cast<short>(posMin.y), v.y);
					posMax.x = ir::math::max(static_cast<short>(posMax.x), v.x);
					posMax.y = ir::math::max(static_cast<short>(posMax.y), v.y);
				}
			}

			return (posMax - posMin) * scale_;
		}

		ModelRenderer& ModelRenderer::setScale(float scale) {
			scale_ = scale;
			return *this;
		}

		ModelRenderer& ModelRenderer::setModel(Model& model) {
			model_ = model;
			return *this;
		}

		ModelRenderer& ModelRenderer::setModel(Model&& model) {
			model_ = model;
			return *this;
		}
	}
}