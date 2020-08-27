
#ifndef CNGE_RESOURCE
#define CNGE_RESOURCE

#include <thread>
#include <chrono>
#include <atomic>
#include <mutex>

#include "../types.h"
#include "loadError.h"

namespace CNGE {
	class Resource {
	public:
		enum class GatherStatus {
			UNGATHERED,
			GATHERING,
			GATHERED
		};

		enum class ProcessStatus {
			UNPROCESSED,
			PROCESSED
		};

	private:
		GatherStatus gatherStatus;
		ProcessStatus processStatus;

		std::thread gatherThread;

		bool hasGather;

	protected:
		virtual auto customGather() -> LoadError; // Gather the required resources from disk
		virtual auto customDiscard() -> LoadError; // Discard the data from disk once it's been processed or if the resource is to be unloaded

		virtual auto customProcess() -> LoadError = 0; // Process the data retrieved from disk (e.g. sending to gpu)
		virtual auto customUnload() -> LoadError = 0; // Unload the resource

		auto destroy() -> void;

	public:
		Resource(bool hasGather);
		~Resource();

		auto joinThread() -> void;

		auto needsGather() -> bool;
		auto isGathering() -> bool;
		auto finishedGathering() -> bool;
		auto gather(LoadError&) -> void;
		auto quickGather(LoadError&) -> void;

		auto needsDiscard() -> bool;
		auto discard(LoadError&) -> void;

		auto needsProcess() -> bool;
		auto process(LoadError&) -> void;

		auto needsUnload() -> bool;
		auto unload(LoadError&) -> void;

		auto getGatherStatus() -> GatherStatus;
		auto getProcessStatus() -> ProcessStatus;
	};
}

#endif
