#include "assets/sound_manager.hpp"
#include "assets/asset_manager.hpp"

namespace ir {
	SoundManager::SoundManager(ir::AssetManager* manager) : manager_(manager) {
		
	}

	void SoundManager::update() {
		for (auto itr = playBuffer_.begin(); itr != playBuffer_.end(); itr++) {
			if (itr->getStatus() == sf::Sound::Status::Stopped) {
				itr = playBuffer_.erase(itr);
			}
		}
	}

	void SoundManager::playSound(ir::SoundHandle handle) {
		sf::SoundBuffer* buf = manager_->getSound(handle);
		if (buf) {
			playBuffer_.push_front(sf::Sound{ *buf });
			playBuffer_.begin()->play();
		}
	}

	void SoundManager::stopAllSounds() {
		for (auto& snd : playBuffer_) {
			snd.stop();
		}
		playBuffer_.clear();
	}
}