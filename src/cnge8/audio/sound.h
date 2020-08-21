
#ifndef CNGE_AUDIO_SOURCE
#define CNGE_AUDIO_SOURCE

#include "../load/resource.h"

#include "../types.h"
#include "audioBuffer.h"

namespace CNGE {
	class Sound : public Resource {
	private:
		u32 source;

		const char* wavFilePath;
		std::unique_ptr<Wav> wav;

	public:
		Sound(const char*);
		
		auto play() -> void;
		auto pause() -> void;
		auto stop() -> void;
		
		auto loop(bool = true) -> void;
		
		auto customGather() -> LoadError override;
		auto customProcess() -> LoadError override;
		auto customDiscard() -> LoadError override;
		auto customUnload() -> LoadError override;

		auto setVolume(f32) -> void;

		~Sound(); 
	};
}

#endif
