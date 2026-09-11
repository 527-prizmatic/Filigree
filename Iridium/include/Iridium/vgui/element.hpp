#ifndef IRIDIUM_VGUI_ELEMENT_HPP_
#define IRIDIUM_VGUI_ELEMENT_HPP_

#include <memory>
#include <vector>
#include "vector.hpp"

namespace ir {
	namespace input {
		class Mouse;
	}

	namespace render {
		class VertexRenderer;
		class Rectangle;
	}

	namespace vgui {
		using ClickEvent = std::function<void(void)>;
		
		/// @brief Parent element for all VGUI utilities.
		///
		/// VGUI (Vector Graphics UI) is a system that allows to easily build GUIs through a recursive system of nested elements.
		/// Each element comes with a large set of event triggers to which the user can register custom callbacks.
		class Element {
		public:
			Element();

			/// @brief Recursively updates the element, as well as its children.
			/// @return Whether the mouse is in this component's area. Note that if a child element is updated, the parent will not be.
			virtual bool update(ir::input::Mouse& mouse);

			/// @brief Recursively renders the element, as well as its children.
			virtual void render(ir::render::VertexRenderer& renderer) const;

			/// @brief Looks up and returns a child element with matching key.
			/// @tparam T Which type the child element should be cast to. If omitted, it will simply return a ir::vgui::Element.
			template <typename T>
			T* getChild(std::string key) const {
				ir::vgui::Element* elt { getChild(key) };
				if (elt) {
					return dynamic_cast<T*>(elt);
				}
				return nullptr;
			}

			/// @brief Looks up and returns a child element with matching key.
			/// @tparam T Which type the child element should be cast to. If omitted, it will simply return a ir::vgui::Element.
			[[nodiscard]] Element* getChild(std::string key) const;
		
			/// These used to be aliases for getChild(), but they turned out to be too cumbersome to use practically
		//	Element* operator[](std::string key) const;
		//	Element* operator[](const char* key) const;

			/// @brief Adds a child to this element and assigns it the given key for later lookup.
			void addChildElement(std::string key, std::unique_ptr<ir::vgui::Element> child);
		//	void removeChildElement(std::string key);
		//	void removeChildElement(ir::vgui::Element* child);
		//	void removeParent();

			virtual void setPosition(ir::Vector pos); ///< @brief Sets position relative to the parent (or the window if there is none)
			virtual void setSize(ir::Vector size); ///< @brief Sets element size, in pixels

			[[nodiscard]] ir::Vector position() const; ///< @return Position relative to the parent (or the window if there is none)
			[[nodiscard]] ir::Vector size() const; ///< @return Element size, in pixels
			[[nodiscard]] ir::Vector absolutePosition() const; ///< @return Window-adjusted position (recursively computed as the sum of all parents' relative positions)

			void setBackgroundColor(sf::Color clr); ///< @brief Sets color of solid background
			[[nodiscard]] sf::Color backgroundColor() const;
			
			void setFrameColor(sf::Color clr); ///< @brief Sets color of outer frame
			[[nodiscard]] sf::Color frameColor() const;

			void setColors(sf::Color frame, sf::Color background); ///< @brief Sets colors for outer frame and solid background in one function call
			static void setDebugMode(bool debug); ///< @brief Enables or disables debug mode (forced frame/background rendering)
			
			void registerClickEvent(ir::vgui::ClickEvent event);
			void registerHoverEvent(ir::vgui::ClickEvent event);
			void registerReleaseEvent(ir::vgui::ClickEvent event);
			void processEvent(const sf::Event& evt); ///< @brief Recursively processes SFML events for the element, as well as its children.

		protected:
			void renderFrame(ir::render::VertexRenderer& renderer) const; ///< @brief Always renders element frame and background
			void renderDebugFrame(ir::render::VertexRenderer& renderer) const; ///< @brief Only renders element frame and background is debugMode_ is set to true
			void renderChildren(ir::render::VertexRenderer& renderer) const;
			void resizeRectangle() const;

			static void createRect(); 

			/// @brief Internal function defining behavior when not hovered by the mouse.
			virtual void onIdle() {}

			/// @brief Internal function defining behavior while hovered by the mouse.
			virtual void onHover() {}

			/// @brief Internal function defining behavior when clicked.
			virtual void onClick() {}

			/// @brief Internal function defining behavior when releasing click.
			virtual void onRelease() {}

			/// @brief Internal function defining behavior when clicking away.
			virtual void onDeselect() {}

			/// @brief Internal function defining behavior when receiving a SFML window event.
			virtual void onSfEvent(const sf::Event& evt) {}

			static std::unique_ptr<ir::render::Rectangle> rect_;

			std::unordered_map<std::string, std::unique_ptr<ir::vgui::Element>> children_;
			ir::vgui::Element* parent_ { nullptr };

			ir::Vector pos_ { 0.f, 0.f };
			ir::Vector size_ { 100.f, 100.f };

			sf::Color clrFrame_ { sf::Color::White };
			sf::Color clrBackground_ { sf::Color::Blue };

			/// @brief Additional user-defined click events.
			std::vector<ir::vgui::ClickEvent> clickEvents {};
			std::vector<ir::vgui::ClickEvent> hoverEvents {};
			std::vector<ir::vgui::ClickEvent> releaseEvents {};

			bool clickHeld_ { false }; ///< @brief Whether left-click is held (for event detection purposes)

			inline static bool debugMode { false }; ///< @brief Whether debug mode is enabled for all VGUI elements (forces frame rendering)
		};

		class FramedElement : public Element {
		public:
			virtual void render(ir::render::VertexRenderer& renderer) const override;
		};
	}
}

#endif // IRIDIUM_VGUI_ELEMENT_HPP_