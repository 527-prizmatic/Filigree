static_assert(true);

#include "rendering/text.hpp"
#include "rendering/vertex_renderer.hpp"

#include <charconv>

namespace ir::vgui {
#pragma region Core functions
	template <typename T>
	InputField<T>::InputField(T defaultValue) {
		value_ = defaultValue;
		
		ir::render::Text::loadModels();

		label_ = std::make_unique<ir::render::Text>();
		if (label_) {
			label_->setScale(10.f);
		}
		else {
			LOG_ERROR("Error during creation of VGUI input field");
		}
	}

	template <typename T>
	bool InputField<T>::update(ir::input::Mouse& mouse) {
		clrBackground_ = focused_ ? clrFocused_ : clrUnfocused_;

		return ir::vgui::Element::update(mouse);
	}

	template <typename T>
	void InputField<T>::render(ir::render::VertexRenderer& renderer) const {
		renderFrame(renderer);

		if (label_) {
			label_->setString(std::to_string(value_));
			label_->setPosition(absolutePosition() + size_ * .5f - label_->boundingBoxSize() * .5f);
			label_->render(renderer);
		}
	}
	
	template <>
	inline void InputField<std::string>::render(ir::render::VertexRenderer& renderer) const {
		renderFrame(renderer);

		if (label_) {
			label_->setString(value_);
			label_->setPosition(absolutePosition() + size_ * .5f - label_->boundingBoxSize() * .5f);
			label_->render(renderer);
		}
	}
#pragma endregion

#pragma region Event hooks
	template <typename T>
	void InputField<T>::onClick() {
		focused_ = true;
	}

	template <typename T>
	void InputField<T>::onDeselect() {
		focused_ = false;
	}
	
	template <typename T>
	void InputField<T>::onSfEvent(const sf::Event& e) {
		if (focused_) {
			if (e.is<sf::Event::TextEntered>()) {
				auto evt = e.getIf<sf::Event::TextEntered>();
				char in = static_cast<char>(evt->unicode);

				std::string val { std::to_string(value_) };
				
				if (in == 8) { // Backspace
					if (val.length() > 0) {
						val.erase(val.end() - 1);
					}
				}
				else if (val.length() < maxChars_) {
					val += evt->unicode;
					std::from_chars(val.data(), val.data() + val.size(), value_);
				}
			}
		}
	}
	
	template <>
	inline void InputField<int>::onSfEvent(const sf::Event& e) {
		if (focused_) {
			if (e.is<sf::Event::TextEntered>()) {
				auto evt = e.getIf<sf::Event::TextEntered>();
				char in = static_cast<char>(evt->unicode);
				
				std::string val { std::to_string(value_) };
				if (in == 8) { // Backspace
					if (val.length() > 0) {
						val.erase(val.end() - 1);
					}
				}
				else if (val.length() < maxChars_) {
					if (in >= '0' && in <= '9') {
						val += evt->unicode;
						std::from_chars(val.data(), val.data() + val.size(), value_);
					}
				}

			}
		}
	}
	
	template <>
	inline void InputField<std::string>::onSfEvent(const sf::Event& e) {
		if (focused_) {
			if (e.is<sf::Event::TextEntered>()) {
				auto evt = e.getIf<sf::Event::TextEntered>();
				char in = static_cast<char>(evt->unicode);

				if (in == 8) { // Backspace
					if (value_.length() > 0) {
						value_.erase(value_.end() - 1);
					}
				}
				else if (value_.length() < maxChars_) {
					value_ += in;
				}
			}
		}
	}
#pragma endregion

#pragma region Mutators and accessors
	template <typename T>
	void InputField<T>::setValue(T val) { value_ = val; }

	template <typename T>
	T InputField<T>::value() const { return value_; }

	template <typename T>
	void InputField<T>::setMaxChars(unsigned int max) { 
		maxChars_ = max;
		
		std::string val { std::to_string(value_) };
		while (val.length() > maxChars_) {
			val.erase(val.end() - 1);
		}
		std::from_chars(val.data(), val.data() + val.size(), value_);
	}
	
	template <>
	inline void InputField<std::string>::setMaxChars(unsigned int max) { 
		maxChars_ = max;
		
		while (value_.length() > maxChars_) {
			value_.erase(value_.end() - 1);
		}
	}

	template <typename T>
	unsigned int InputField<T>::getMaxChars() const { return maxChars_; }

	template <typename T>
	void InputField<T>::setScale(float scale) {
		if (label_) {
			label_->setScale(scale);
		}
	}

	template <typename T>
	float InputField<T>::scale() const {
		if (label_) {
			return label_->scale();
		}
		return -1.f;
	}

	template <typename T>
	void InputField<T>::setLabelColor(sf::Color clr) {
		if (label_) {
			label_->setColor(clr);
		}
	}
	
	template <typename T>
	sf::Color InputField<T>::labelColor() const {
		if (label_) {
			return label_->color();
		}
		return sf::Color::Transparent;
	}

	template <typename T>
	void InputField<T>::setColorUnfocused(sf::Color clr) { clrUnfocused_ = clr; }
	
	template <typename T>
	sf::Color InputField<T>::colorUnfocused() const { return clrUnfocused_; }

	template <typename T>
	void InputField<T>::setColorFocused(sf::Color clr) { clrFocused_ = clr; }
	
	template <typename T>
	sf::Color InputField<T>::colorFocused() const { return clrFocused_; }

	template <typename T>
	bool InputField<T>::hasFocus() const { return focused_; }

	template <typename T>
	void InputField<T>::focus() { focused_ = true; }
#pragma endregion
}