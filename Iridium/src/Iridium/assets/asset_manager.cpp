#include "assets/asset_manager.hpp"

namespace ir {
	AssetManager::AssetManager() {
		
	}

	#pragma region sf::Texture
	template <>
	ir::TextureHandle AssetManager::registerAsset<ir::TextureAsset, ir::TextureHandle>(std::filesystem::path name) {
		ir::TextureHandle handle = std::hash<std::string>{}(name.string());
		if (textures_.find(handle) != textures_.end()) {
			throw name.string();
		}
		textures_[handle] = std::make_unique<ir::TextureAsset>("..\\resources\\textures\\" + std::string(name.string()));
		return handle;
	}

	template <>
	sf::Texture* AssetManager::getAsset<ir::TextureAsset, ir::TextureHandle>(ir::TextureHandle handle) {
		if (textures_.find(handle) == textures_.end()) {
			return nullptr;
		}
		return &*textures_.at(handle);
	}
	
	ir::TextureHandle AssetManager::registerTexture(std::filesystem::path name) { return registerAsset<ir::TextureAsset, ir::TextureHandle>(name); }
	ir::TextureAsset* AssetManager::getTexture(ir::TextureHandle handle) { return getAsset<ir::TextureAsset, ir::TextureHandle>(handle); }
#pragma endregion

#pragma region sf::SoundBuffer
	template <>
	ir::SoundHandle AssetManager::registerAsset<ir::SoundAsset, ir::SoundHandle>(std::filesystem::path name) {
		ir::SoundHandle handle = std::hash<std::string>{}(name.string());
		if (sounds_.find(handle) != sounds_.end()) {
			throw name.string();
		}
		sounds_[handle] = std::make_unique<ir::SoundAsset>("..\\resources\\sounds\\" + std::string(name.string()));
		return handle;
	}

	template <>
	sf::SoundBuffer* AssetManager::getAsset<ir::SoundAsset, ir::SoundHandle>(ir::SoundHandle handle) {
		if (sounds_.find(handle) == sounds_.end()) {
			return nullptr;
		}
		return &*sounds_.at(handle);
	}
	
	ir::SoundHandle AssetManager::registerSound(std::filesystem::path name) { return registerAsset<ir::SoundAsset, ir::SoundHandle>(name); }
	ir::SoundAsset* AssetManager::getSound(ir::SoundHandle handle) { return getAsset<ir::SoundAsset, ir::SoundHandle>(handle); }
#pragma endregion

#pragma region sf::Music
	template <>
	ir::MusicHandle AssetManager::registerAsset<ir::MusicAsset, ir::MusicHandle>(std::filesystem::path name) {
		ir::MusicHandle handle = std::hash<std::string>{}(name.string());
		if (musics_.find(handle) != musics_.end()) {
			throw name.string();
		}
		musics_[handle] = std::make_unique<ir::MusicAsset>("..\\resources\\music\\" + std::string(name.string()));
		return handle;
	}

	template <>
	ir::MusicAsset* AssetManager::getAsset<ir::MusicAsset, ir::MusicHandle>(ir::MusicHandle handle) {
		if (musics_.find(handle) == musics_.end()) {
			return nullptr;
		}
		return &*musics_.at(handle);
	}
	
	ir::MusicHandle AssetManager::registerMusic(std::filesystem::path name) { return registerAsset<ir::MusicAsset, ir::MusicHandle>(name); }
	ir::MusicAsset* AssetManager::getMusic(ir::MusicHandle handle) { return getAsset<ir::MusicAsset, ir::MusicHandle>(handle); }
#pragma endregion

#pragma region Music playback
	void AssetManager::playMusic(ir::MusicHandle handle) {
		auto music = getMusic(handle);
		if (music == nullptr) {
			throw ir::Exceptions::BadAssetHandle(std::to_string(handle));
		}
		music->play();
	}

	void AssetManager::pauseMusic(ir::MusicHandle handle) {
		auto music = getMusic(handle);
		if (music == nullptr) {
			throw ir::Exceptions::BadAssetHandle(std::to_string(handle));
		}
		music->pause();
	}
	
	void AssetManager::stopMusic(ir::MusicHandle handle) {
		auto music = getMusic(handle);
		if (music == nullptr) {
			throw ir::Exceptions::BadAssetHandle(std::to_string(handle));
		}
		music->stop();
	}
	
	void AssetManager::setMusicVolume(ir::MusicHandle handle, float volume) {
		auto music = getMusic(handle);
		if (music == nullptr) {
			throw ir::Exceptions::BadAssetHandle(std::to_string(handle));
		}
		music->setVolume(ir::math::clamp(volume, 0.f, 100.f));
	}

	sf::Music::Status AssetManager::getMusicStatus(ir::MusicHandle handle) {
		auto music = getMusic(handle);
		if (music == nullptr) {
			throw ir::Exceptions::BadAssetHandle(std::to_string(handle));
		}
		return music->getStatus();
	}
#pragma endregion
}