#ifndef IRIDIUM_ASSET_MANAGER_HPP_
#define IRIDIUM_ASSET_MANAGER_HPP_

#include "libraries.hpp"
#include "exceptions.hpp"

namespace ir {
	class AssetManager {
	public:
		AssetManager();

		template <typename T>
		void deleteAsset(T handle) {}

		[[nodiscard]] ir::TextureHandle registerTexture(std::filesystem::path name);
		[[nodiscard]] ir::TextureAsset* getTexture(ir::TextureHandle handle);

		[[nodiscard]] ir::SoundHandle registerSound(std::filesystem::path name);
		[[nodiscard]] ir::SoundAsset* getSound(ir::SoundHandle handle);
		
		[[nodiscard]] ir::MusicHandle registerMusic(std::filesystem::path name);
		[[nodiscard]] ir::MusicAsset* getMusic(ir::MusicHandle handle);

		void playMusic(ir::MusicHandle handle);
		void pauseMusic(ir::MusicHandle handle);
		void stopMusic(ir::MusicHandle handle);
		void setMusicVolume(ir::MusicHandle handle, float volume);
		sf::Music::Status getMusicStatus(ir::MusicHandle handle);

	private:
		/// @brief Generic template for registering new assets to resource buffers.
		/// @tparam T Asset type (ir::___Asset type)
		/// @tparam U Handle type (ir::___Handle type)
		template <typename T, typename U>
		U registerAsset(std::filesystem::path name) {
			throw ir::Exceptions::BadAssetType(typeid(T).name());
		}

		/// @tparam T Asset type (ir::___Asset type)
		/// @tparam U Handle type (ir::___Handle type)
		template <typename T, typename U>
		T* getAsset(U handle) {
			return nullptr;
		}

		std::unordered_map<TextureHandle, std::unique_ptr<ir::TextureAsset>> textures_;
		std::unordered_map<SoundHandle, std::unique_ptr<ir::SoundAsset>> sounds_;
		std::unordered_map<MusicHandle, std::unique_ptr<ir::MusicAsset>> musics_;
	};

	template <>	ir::TextureHandle AssetManager::registerAsset<ir::TextureAsset, ir::TextureHandle>(std::filesystem::path name);
	template <>	ir::TextureAsset* AssetManager::getAsset<ir::TextureAsset, ir::TextureHandle>(ir::TextureHandle handle) ;
	
	template <> ir::SoundHandle AssetManager::registerAsset<ir::SoundAsset, ir::SoundHandle>(std::filesystem::path name);
	template <> ir::SoundAsset* AssetManager::getAsset<ir::SoundAsset, ir::SoundHandle>(ir::SoundHandle handle);

	template <> ir::MusicHandle AssetManager::registerAsset<ir::MusicAsset, ir::MusicHandle>(std::filesystem::path name);
	template <> ir::MusicAsset* AssetManager::getAsset<ir::MusicAsset, ir::MusicHandle>(ir::MusicHandle handle);
}

#endif // IRIDIUM_ASSET_MANAGER_HPP_