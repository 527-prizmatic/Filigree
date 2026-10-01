#include "gui/context_menu.hpp"

#include <vgui/label.hpp>

#include "filepath_funcs.hpp"

namespace filigree::gui {
	ContextMenu::ContextMenu(filigree::EventQueue& evtQueue) {
		evtQueue_= &evtQueue;
	}

	void ContextMenu::processEvent(const sf::Event& evt, ir::input::Mouse& mouseInput) {
		if (ctxMenu_) {
			ctxMenu_->processEvent(evt);
		}
	}
	
	void ContextMenu::update(ir::input::Mouse& mouseInput) {
		if (ctxMenu_) {
			if (!ctxMenu_->update(mouseInput) && (mouseInput.isPressed(sf::Mouse::Button::Left) || mouseInput.isPressed(sf::Mouse::Button::Right))) {
				close();
			}
		}
	}
	
	void ContextMenu::render(ir::render::VertexRenderer& renderer) const {
		if (ctxMenu_) {
			ctxMenu_->render(renderer);
		}
	}
	
	void ContextMenu::create(std::filesystem::path path, ir::Vector pos) {
		currentPath_ = path;

		if (std::filesystem::is_directory(path)) {
			createUIDir();
		}
		else if (filigree::isImage(path)) {
			createUIImg();
		}
		else {
			createUIOther();
		}
		
		ctxMenu_->setPosition(pos);
	}
	
	void ContextMenu::createParent(std::filesystem::path path, ir::Vector pos) {
		currentPath_ = path;
		createUIParent();
		ctxMenu_->setPosition(pos);
	}
	
	void ContextMenu::close() {
		ctxMenu_.reset();
	}

	ir::vgui::Element* ContextMenu::setupButton(ir::vgui::Element* el, float posY, filigree::Event evt, std::string label) {
		el->setPosition(ir::Vector { 5.f, posY })
			.setSize(ir::Vector { 290.f, 30.f })
			.setColors(sf::Color::Transparent, sf::Color::Transparent);

		el->registerClickEvent([&, evt](){ evtQueue_->add(evt); })
		.registerHoverEvent([el](){ el->setBackgroundColor(sf::Color { 255u, 255u, 255u, 32u }); })
		.registerIdleEvent([el](){ el->setBackgroundColor(sf::Color::Transparent); });

		el->addChildElement<ir::vgui::Label>("Label", label)
			->setAnchor(ir::vgui::Label::Anchor::OVER)
			.setScale(15.f);
			
		return el;
	}
	
	void ContextMenu::createUITitle(int entryCount, sf::Color clrTitle) {
		ctxMenu_ = std::make_unique<ir::vgui::FramedElement>();
		ctxMenu_->setSize(ir::Vector { 300.f, 40.f + entryCount * 30.f })
			.setColors(clrTitle, sf::Color { 0u, 0u, 0u, 224u });

		auto title { ctxMenu_->addChildElement<ir::vgui::Label>("Title", currentPath_.filename().string()) };
		title->setColor(clrTitle)
			.setPosition(ir::Vector { 10.f, 9.f });
		
		auto div { ctxMenu_->addChildElement<ir::vgui::FramedElement>("Divider") };
		div->setPosition(ir::Vector { 5.f, 34.f })
			.setSize(ir::Vector { 290.f, 0.f })
			.setFrameColor(sf::Color { clrTitle.r, clrTitle.g, clrTitle.b, 64u });
		
	}

	void ContextMenu::createUIDir() {
		createUITitle(3, sf::Color { 128u, 255u, 192u });

		auto nav { ctxMenu_->addChildElement<ir::vgui::FramedElement>("FieldNav") };
		setupButton(nav, 35.f, filigree::Event::CONTEXT_MENU_NAVIGATE, "Open");

		auto scan { ctxMenu_->addChildElement<ir::vgui::FramedElement>("FieldScan") };
		setupButton(scan, 65.f, filigree::Event::CONTEXT_MENU_SCAN_FOLDER, "Add contents to queue");

		auto setOutput { ctxMenu_->addChildElement<ir::vgui::FramedElement>("FieldOut") };
		setupButton(setOutput, 95.f, filigree::Event::CONTEXT_MENU_SET_OUTPUT, "Set as output folder");
	}

	void ContextMenu::createUIImg() {
		createUITitle(3, sf::Color { 192u, 128u, 255u });

		auto queue { ctxMenu_->addChildElement<ir::vgui::FramedElement>("FieldQueue") };
		setupButton(queue, 35.f, filigree::Event::CONTEXT_MENU_QUEUE_FILE, "Add image to queue");

		auto fil { ctxMenu_->addChildElement<ir::vgui::FramedElement>("FieldFiligree") };
		setupButton(fil, 65.f, filigree::Event::CONTEXT_MENU_SET_FILIGREE, "Use as filigree");

		auto stamp { ctxMenu_->addChildElement<ir::vgui::FramedElement>("FieldStamp") };
		setupButton(stamp, 95.f, filigree::Event::CONTEXT_MENU_SET_STAMP, "Use as stamp");
	}
	
	void ContextMenu::createUIOther() {
		createUITitle(1, sf::Color { 255u, 192u, 128u });

		auto none { ctxMenu_->addChildElement<ir::vgui::FramedElement>("FieldClose") };
		setupButton(none, 35.f, filigree::Event::CONTEXT_MENU_CLOSE, "No actions available");
	}

	void ContextMenu::createUIParent() {
		createUITitle(2, sf::Color { 192u, 192u, 192u });

		auto setOutput { ctxMenu_->addChildElement<ir::vgui::FramedElement>("FieldOut") };
		setupButton(setOutput, 35.f, filigree::Event::CONTEXT_MENU_SET_OUTPUT, "Set as output folder");

		auto scan { ctxMenu_->addChildElement<ir::vgui::FramedElement>("FieldScan") };
		setupButton(scan, 65.f, filigree::Event::CONTEXT_MENU_SCAN_FOLDER, "Add contents to queue");
	}
}