#include "gui/image_preview.hpp"

#include <time.hpp>
#include <vgui/label.hpp>

namespace filigree::gui {
	ImagePreview::ImagePreview(filigree::EventQueue& evtQueue) {
		evtQueue_ = &evtQueue;

		preview_ = std::make_unique<ir::vgui::FramedElement>();
		if (!preview_) {
			LOG_ERROR("Could not create file explorer!!");
		}
		else {
			preview_->setColors(sf::Color::White, sf::Color { 16u, 8u, 0u })
				.setSize(ir::Vector { 400.f, 300.f })
				.setPosition(ir::Vector { 880.f, 30.f });
			createUIProgressBar();
		}

	}

	void ImagePreview::update(ir::input::Mouse& mouseInput) {
		if (preview_) {
			preview_->update(mouseInput);
		}

		if (shouldDelete_) {
			img_.reset();
			quad_.reset();
			shouldDelete_ = false;
		}

		if (texPath_.has_value()) {
			setTexture(texPath_.value());
			texPath_.reset();
		}

		if (progressBar_ && progressBar_->enabled()) {
			progressBar_->update(mouseInput);
			std::lock_guard<std::mutex> lock { mutex_ };
			auto fill { progressBar_->getChild("Fill") };
			fill->setSize(ir::Vector { 300.f * static_cast<float>(pbStatus_) / static_cast<float>(pbCount_), 20.f });

			auto label { progressBar_->getChild<ir::vgui::Label>("Label") };
			label->setLabel(std::to_string(pbStatus_) + " / " + std::to_string(pbCount_));
		}
	}

	void ImagePreview::render(ir::render::VertexRenderer& renderer) {
		if (preview_) {
			preview_->render(renderer);

			ir::Vector center { preview_->size() * .5f + preview_->absolutePosition() };
			if (img_ != nullptr) {
				std::lock_guard<std::mutex> lock { mutex_ };
				if (quad_) {
					quad_->setPosition(center - quad_->size() * .5f);
					quad_->render(renderer);
				}
			}
			else {
				drawSpinner(renderer, center);
			}
		}

		if (progressBar_ && progressBar_->enabled()) {
			progressBar_->render(renderer);
		}
	}

	
	void ImagePreview::prepareTexture(std::filesystem::path path) {
		texPath_ = path;
	}

	void ImagePreview::deleteTexture() {
		shouldDelete_ = true;
	}

	void ImagePreview::setProgressBarCount(int count) {
		std::lock_guard<std::mutex> lock { mutex_ };
		pbCount_ = count;
	}

	void ImagePreview::setProgressBarStatus(int count) {
		std::lock_guard<std::mutex> lock { mutex_ };
		pbStatus_ = count;
	}
	
	void ImagePreview::setProgressBarVisibility(bool visible) {
		if (progressBar_) {
			std::lock_guard<std::mutex> lock { mutex_ };
			progressBar_->setEnabled(visible);
		}
	}
	
	void ImagePreview::incrementProgressBar() {
		std::lock_guard<std::mutex> lock { mutex_ };
		pbStatus_++;
	}

	void ImagePreview::setTexture(std::filesystem::path tex) {
		std::lock_guard<std::mutex> lock { mutex_ };
		if (tex.empty()) {
			img_.reset();
			quad_.reset();
		}
		else {
			quad_ = std::make_unique<ir::render::Quad>();
			img_ = std::make_unique<sf::Texture>(tex);
			
			quad_->setTexture(*img_);
			quad_->setUVs(ir::Vector::kZero, ir::Vector::fromSFMLVector(img_->getSize()));

			float ratioX { img_->getSize().x / 400.f };
			float ratioY { img_->getSize().y / 300.f };

			if (ratioX > ratioY) {
				quad_->setSize(img_->getSize().x / ratioX * .8f, img_->getSize().y / ratioX * .8f);
			}
			else {
				quad_->setSize(img_->getSize().x / ratioY * .8f, img_->getSize().y / ratioY * .8f);
			}

			quad_->setMode(ir::render::Mode::SOLID);
			
		}
	}
	
	void ImagePreview::createUIProgressBar() {
		progressBar_ = std::make_unique<ir::vgui::FramedElement>();
		progressBar_->setPosition(preview_->position() + ir::Vector { 50.f, 270.f })
			.setSize(ir::Vector { 300.f, 20.f })
			.setColors(sf::Color::White, sf::Color { 0u, 0u, 0u, 128u });
		
		auto label { progressBar_->addChildElement<ir::vgui::Label>("Label", "0 / 0") };
		label->setAnchor(ir::vgui::Label::Anchor::OVER)
			.setScale(12.f);
		
		auto progress { progressBar_->addChildElement<ir::vgui::FramedElement>("Fill") };
		progress->setPosition(ir::Vector { 0.f, 0.f })
			.setSize(ir::Vector { 0.f, 20.f })
			.setColors(sf::Color::White, sf::Color { 255u, 170u, 85u });
		
		progressBar_->setEnabled(false);
	}

	void ImagePreview::updateSpinnerAngle(float dt) {
		spinnerAngle_ += dt;
	}

	void ImagePreview::drawSpinner(ir::render::VertexRenderer& renderer, ir::Vector center) {
		constexpr int sides { 6 };
		constexpr float angle { ir::math::tau / static_cast<float>(sides) };
		for (size_t i = 0; i < sides; i++) {
			ir::Vector offset1 { ir::Vector { 100.f, 0.f }.rotate(spinnerAngle_ + angle * static_cast<float>(i)) };
			ir::Vector offset2 { ir::Vector { 100.f, 0.f }.rotate(spinnerAngle_ + angle * (static_cast<float>(i) + .5f)) };
			ir::Vector offset3 { ir::Vector { 100.f, 0.f }.rotate(spinnerAngle_ + angle * static_cast<float>(i + 1)) };

			/// Create a dot function / operator asap
			auto dot { [](ir::Vector a, ir::Vector b) { return a.x * b.x + a.y * b.y; } };

			ir::Vector direction { ir::Vector::polar(100.f, ir::math::tau * -.125f) };
			float maxDot { dot(direction, direction) };
			float dotSide { dot(direction, offset2) };
			sf::Color clr { 255u, 255u, 255u, 0u };
			clr.a = 32.f * std::abs(ir::math::powi(dotSide / maxDot, 3));

			renderer.reset(sf::PrimitiveType::TriangleStrip);
			renderer.addPoint(center + offset1, clr);
			renderer.addPoint(center + offset1 * .5f, clr);
			renderer.addPoint(center + offset3, clr);
			renderer.addPoint(center + offset3 * .5f, clr);
			renderer.flush();

			renderer.reset(sf::PrimitiveType::Lines);
			renderer.addPoint(center + offset1, sf::Color::White);
			renderer.addPoint(center + offset3, sf::Color::White);
			renderer.flush();
		}
	}
}