#pragma once

#include <cstdint>
#include <SFML/Audio/Sound.hpp>
#include <string>
#include <unordered_map>

enum class Sound : std::uint8_t {
	Move,
	Take,
};

class SoundSystem {
  public:
	static void init();
	static void playSound(Sound type);

  private:
	static void addSoundBuffer(Sound type, const std::string_view path);

  private:
	static std::unordered_map<Sound, sf::SoundBuffer> s_soundBuffers;
	static sf::Sound s_sound;
};
