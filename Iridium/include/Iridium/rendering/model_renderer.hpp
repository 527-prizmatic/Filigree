#ifndef IRIDIUM_RENDERING_MODEL_RENDERER_HPP_
#define IRIDIUM_RENDERING_MODEL_RENDERER_HPP_

#include "rendering/shape.hpp"
#include "rendering/model.hpp"

namespace ir {
	namespace render {
		/// @brief Utility class for rendering VMF models.
		class ModelRenderer : public Shape {
		public:
			virtual void render(ir::render::VertexRenderer& renderer) const;

			ModelRenderer& setModel(Model& model);
			ModelRenderer& setModel(Model&& model);
			[[nodiscard]] Model& model() { return model_; }

			/// @attention At a scale of 1, one grid unit in the model corresponds to one screen pixel.
			virtual ModelRenderer& setScale(float scale);

			/// @attention At a scale of 1, one grid unit in the model corresponds to one screen pixel.
			[[nodiscard]] virtual float scale() const { return scale_; }

			[[nodiscard]] ir::Vector boundingBoxSize(); ///< @return Scale-adjusted size of the model's bounding box

		protected:
			float scale_ { 1.f };
			/// @todo Find a way to replace this with a reference or pointer
			/// This would require a solid memory management system though, to avoid leaks in case the renderer ever gains ownership of the model one way or another
			/// (Though ideally it should never do so)
			Model model_;

			virtual void addVertexToBuffer(const Vertex& v, ir::render::VertexRenderer& renderer) const;
		};
	}
}





#endif