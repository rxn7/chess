#include "sound_system.h"
#include <iostream>
#include <SFML/Audio/SoundBuffer.hpp>
#include <unordered_map>

sf::Sound SoundSystem::s_sound;
std::unordered_map<Sound, sf::SoundBuffer> SoundSystem::s_soundBuffers;

void SoundSystem::init() {
	addSoundBuffer(Sound::Move, "assets/sounds/move.ogg");
	addSoundBuffer(Sound::Capture, "assets/sounds/take.wav");
	addSoundBuffer(Sound::Check, "assets/sounds/check.ogg");
	addSoundBuffer(Sound::Checkmate, "assets/sounds/checkmate.ogg");
	addSoundBuffer(Sound::Stalemate, "assets/sounds/stalemate.wav");
}

void SoundSystem::playSound(Sound type) {
	const std::unordered_map<Sound, sf::SoundBuffer>::iterator it = s_soundBuffers.find(type);

	if (it == s_soundBuffers.end()) {
		std::cerr << "\e[1;31mSoundBuffer for SoundType " << (int)type << " not found!\e[0m\n";
		return;
	}

	const sf::SoundBuffer &buf = it->second;

	s_sound.setBuffer(buf);
	s_sound.setPitch((9 + (rand() % 3)) / 10.0f);
	s_sound.play();
}

void SoundSystem::addSoundBuffer(Sound type, const std::string_view path) {
	sf::SoundBuffer buf;
	if (!buf.loadFromFile(std::string(path))) {
		std::cerr << "\e[1;31mFailed to load sound " << path << "!\e[0m\n";
		return;
	}
	s_soundBuffers.insert({type, buf});
}
