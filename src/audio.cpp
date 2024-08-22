#include "audio.hpp"
#include <iostream>

sf::Sound Audio::s_sound;
std::unordered_map<Sound, std::vector<sf::SoundBuffer>> Audio::s_soundBuffers;

void Audio::init() {
	addSoundBuffer(Sound::Move, "assets/sounds/move1.ogg");
	addSoundBuffer(Sound::Move, "assets/sounds/move2.ogg");
	addSoundBuffer(Sound::Move, "assets/sounds/move3.ogg");
	addSoundBuffer(Sound::Move, "assets/sounds/move4.ogg");
	addSoundBuffer(Sound::Capture, "assets/sounds/capture1.ogg");
	addSoundBuffer(Sound::Capture, "assets/sounds/capture2.ogg");
	addSoundBuffer(Sound::Check, "assets/sounds/check.ogg");
	addSoundBuffer(Sound::Checkmate, "assets/sounds/checkmate.ogg");
	addSoundBuffer(Sound::Stalemate, "assets/sounds/stalemate.wav");
}

void Audio::playSound(Sound type) {
	const std::unordered_map<Sound, std::vector<sf::SoundBuffer>>::iterator it = s_soundBuffers.find(type);

	if(it == s_soundBuffers.end()) {
		std::cerr << "\e[1;31mSoundBuffer for SoundType " <<(int)type << " not found!\e[0m\n";
		return;
	}

	const std::size_t idx = it->second.size() == 1 ? 0 : rand() % it->second.size();
	const sf::SoundBuffer &buf = it->second[idx];

	s_sound.setBuffer(buf);
	s_sound.setPitch((9 +(rand() % 3)) / 10.0f);
	s_sound.play();
}

void Audio::addSoundBuffer(Sound type, const std::string_view path) {
	sf::SoundBuffer buf;
	if(!buf.loadFromFile(std::string(path))) {
		std::cerr << "\e[1;31mFailed to load sound " << path << "!\e[0m\n";
		return;
	}

	std::unordered_map<Sound, std::vector<sf::SoundBuffer>>::iterator it = s_soundBuffers.find(type);
	if(it != s_soundBuffers.end()) {
		it->second.push_back(buf);
	} else {
		s_soundBuffers.insert({type, {buf}});
	}
}
