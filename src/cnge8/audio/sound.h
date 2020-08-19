
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
		
		void play();
		void pause();
		void stop();
		
		void loop(bool = true);
		
		auto customGather() -> bool override;
		auto customProcess() -> bool override;
		auto customDiscard() -> bool override;
		auto customUnload() -> bool override;

		auto setVolume(f32);

		~Sound(); 
	};
}

#endif
