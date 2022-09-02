#pragma once

#include <SFML/Audio/Sound.hpp>
#include <string>
#include <unordered_map>

enum class SoundType {
	Move,
	Take,
};

class SoundSystem {
public:
	static void init();
	static void playSound(SoundType type);

private:
	static void addSoundBuffer(SoundType type, const std::string &path);

private:
	static std::unordered_map<SoundType, sf::SoundBuffer> s_soundBuffers;
	static sf::Sound s_sound;
};