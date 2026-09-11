#ifndef IRIDIUM_LIBRARIES_HPP_
#define IRIDIUM_LIBRARIES_HPP_

#pragma GCC diagnostic ignored "-Wunused-parameter"

// C++ core and STL features
#include <memory>
#include <type_traits>
#include <functional>
#include <map>
#include <any>
#include <list>
#include <fstream>

// SFML
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

// GSL
#include <gsl/gsl>

// JSON

// Base-level Iridium components
#include "Iridium/log.hpp"
#include "Iridium/math.hpp"

namespace ir {
	using TextureHandle = uint32_t;
	using SoundHandle = uint32_t;
	using MusicHandle = uint32_t;

	using TextureAsset = sf::Texture;
	using SoundAsset = sf::SoundBuffer;
	using MusicAsset = sf::Music;
}

#endif // IRIDIUM_LIBRARIES_HPP_