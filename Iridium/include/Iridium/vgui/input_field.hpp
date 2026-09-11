#ifndef IRIDIUM_VGUI_INFIELD_HPP_
#define IRIDIUM_VGUI_INFIELD_HPP_

#include <string>
#include "vgui/element.hpp"

namespace ir {
	namespace render {
		class Text;
	}

	namespace vgui {
		/// @brief VGUI input field, allowing the user to type in values as text.
		/// @tparam T What data type the user will be able to input (text or numbers, most likely)
		template <typename T>
		class InputField : public Element {
		public:
			using DataType = T;

			InputField(T defaultValue);

			virtual bool update(ir::input::Mouse& mouse) override;
			virtual void render(ir::render::VertexRenderer& renderer) const override;

		//	virtual void onIdle() override;
			virtual void onClick() override;
			virtual void onDeselect() override;
			virtual void onSfEvent(const sf::Event& e) override;

			void setValue(T value);
			[[nodiscard]] T value() const;

			void setMaxChars(unsigned int max); ///< @brief Sets how long the input can be
			unsigned int getMaxChars() const; ///< @return How long the input can be

			void setScale(float scale); ///< @brief Sets label scale. As with ir::render::Text, scale equals pixel height.
			float scale() const; ///< @return Label scale. As with ir::render::Text, scale equals pixel height.

			void setLabelColor(sf::Color clr);
			[[nodiscard]] sf::Color labelColor() const;

			void setColorUnfocused(sf::Color clr); ///< @brief Sets a background color to use when not focused.
			[[nodiscard]] sf::Color colorUnfocused() const;

			void setColorFocused(sf::Color clr); ///< @brief Sets a background color to use when focused.
			[[nodiscard]] sf::Color colorFocused() const;

			/// @return Whether the input field is being focused. Only one input field can be focused at a time.
			/// It is only possible to type in values in a focused input field.
			[[nodiscard]] bool hasFocus() const;
			void focus();

		private:
			std::unique_ptr<ir::render::Text> label_;
			T value_ {};
			bool focused_ { false };
			unsigned int maxChars_ { 20 };

			sf::Color clrUnfocused_ { sf::Color::Blue };
			sf::Color clrFocused_ { sf::Color::Green };
		};

		using TextField = ir::vgui::InputField<std::string>;
		using IntField = ir::vgui::InputField<int>;
	}
}

#include "detail/vgui/input_field.inl"

#endif // IRIDIUM_VGUI_INFIELD_HPP_