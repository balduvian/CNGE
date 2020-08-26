
#include <iostream>

#include <al\al.h>

#include "sound.h"

namespace CNGE {
	Sound::Sound(const char* wavFilePath)
		: Resource(true), wavFilePath(wavFilePath), wav(), source() {}

	auto Sound::customGather() -> LoadError {
		wav = std::make_unique<Wav>(wavFilePath);

		if (wav->getErr())
			return LoadError(std::string("wav file ") + wavFilePath + " could not be read");

		return LoadError::none();
	}

	auto Sound::customProcess() -> LoadError {
		auto buffer = AudioBuffer(wav.get());

		alGenSources(1, &source);
		alSourcef(source, AL_GAIN, 1);
		alSource3f(source, AL_POSITION, 0, 0, 0);
		alSource3f(source, AL_DIRECTION, 0, 0, 0);
		alSourcei(source, AL_BUFFER, buffer.getBuffer());

		return LoadError::none();
	}

	auto Sound::customUnload() -> LoadError {
		alDeleteSources(1, &source);
		return LoadError::none();
	}

	auto Sound::customDiscard() -> LoadError {
		wav = nullptr;
		return LoadError::none();
	}

	auto Sound::play() -> void {
		alSourcePlay(source);
	}

	auto Sound::stop() -> void {
		alSourceStop(source);
	}

	auto Sound::pause() -> void {
		alSourcePause(source);
	}

	auto Sound::loop(bool loop) -> void {
		alSourcei(source, AL_LOOPING, loop);
	}

	auto Sound::setVolume(f32 volume) -> void {
		alSourcef(source, AL_GAIN, volume);
	}

	Sound::~Sound() {
		destroy();
	}
}
