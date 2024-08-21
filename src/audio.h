#pragma once

#include <cstdint>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <string>
#include <vector>
#include <unordered_map>

enum class Sound : std::uint8_t {
	Move,
	Capture,
	Check,
	Checkmate,
	Stalemate
};

class Audio {
  public:
	static void init();
	static void playSound(Sound type);

  private:
	static void addSoundBuffer(Sound type, const std::string_view path);

  private:
	static std::unordered_map<Sound, std::vector<sf::SoundBuffer>> s_soundBuffers;
	static sf::Sound s_sound;
};
