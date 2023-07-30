#include "sound_system.h"
#include <iostream>
#include <SFML/Audio/SoundBuffer.hpp>
#include <unordered_map>

sf::Sound SoundSystem::s_sound;
std::unordered_map<SoundType, sf::SoundBuffer> SoundSystem::s_soundBuffers;

void SoundSystem::init() {
	addSoundBuffer(SoundType::Move, "assets/sounds/move.wav");
	addSoundBuffer(SoundType::Take, "assets/sounds/take.wav");
}

void SoundSystem::playSound(SoundType type) {
	std::unordered_map<SoundType, sf::SoundBuffer>::iterator it = s_soundBuffers.find(type);

	if (it == s_soundBuffers.end()) {
		std::cerr << "\e[1;31mSoundBuffer for SoundType " << (int)type << " not found!\e[0m\n";
		return;
	}

	sf::SoundBuffer &buf = it->second;

	s_sound.setBuffer(buf);
	s_sound.setPitch((9 + (rand() % 3)) / 10.0f);
	s_sound.play();
}

void SoundSystem::addSoundBuffer(SoundType type, const std::string &path) {
	sf::SoundBuffer buf;
	if (!buf.loadFromFile(path)) {
		std::cerr << "\e[1;31mFailed to load sound " << path << "!\e[0m\n";
		return;
	}
	s_soundBuffers.insert({type, buf});
}
