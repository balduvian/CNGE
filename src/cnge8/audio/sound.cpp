
#include <iostream>

#include <AL\al.h>

#include "sound.h"

namespace CNGE {
	Sound::Sound(const char* wavFilePath)
		: Resource(true), wavFilePath(wavFilePath), wav(), source() {}

	auto Sound::customGather() -> bool {
		wav = std::make_unique<Wav>(wavFilePath);

		return !wav->getErr();
	}

	auto Sound::customProcess() -> bool {
		auto buffer = AudioBuffer(wav.get());

		alGenSources(1, &source);
		alSourcef(source, AL_GAIN, 1);
		alSource3f(source, AL_POSITION, 0, 0, 0);
		alSource3f(source, AL_DIRECTION, 0, 0, 0);
		alSourcei(source, AL_BUFFER, buffer.getBuffer());

		return true;
	}

	auto Sound::customUnload() -> bool {
		alDeleteSources(1, &source);

		return true;
	}

	auto Sound::customDiscard() -> bool {
		wav = nullptr;

		return true;
	}

	void Sound::play() {
		alSourcePlay(source);
	}

	void Sound::stop() {
		alSourceStop(source);
	}

	void Sound::pause() {
		alSourcePause(source);
	}

	void Sound::loop(bool loop) {
		alSourcei(source, AL_LOOPING, loop);
	}

	auto Sound::setVolume(f32 volume) {
		alSourcef(source, AL_GAIN, volume);
	}

	Sound::~Sound() {
		destroy();
	}
}
