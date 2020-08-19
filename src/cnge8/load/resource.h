
#ifndef CNGE_RESOURCE
#define CNGE_RESOURCE

#include <thread>
#include <chrono>
#include <atomic>
#include <mutex>
#include "../types.h"

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
		virtual auto customGather() -> bool = 0; // Gather the required resources from disk
		virtual auto customDiscard() -> bool = 0; // Discard the data from disk once it's been processed or if the resource is to be unloaded

		virtual auto customProcess() -> bool = 0; // Process the data retrieved from disk (e.g. sending to gpu)
		virtual auto customUnload() -> bool = 0; // Unload the resource

		auto destroy() -> void;

	public:
		Resource(bool hasGather);

		auto joinThread() -> bool;

		auto needsGather() -> bool;
		auto isGathering() -> bool;
		auto finishedGathering() -> bool;
		auto gather() -> void;
		auto quickGather() -> bool;

		auto needsDiscard() -> bool;
		auto discard() -> bool;

		auto needsProcess() -> bool;
		auto process() -> bool;

		auto needsUnload() -> bool;
		auto unload() -> bool;

		auto getGatherStatus() const -> GatherStatus;
		auto getProcessStatus() const -> ProcessStatus;

		auto getHasGather() -> bool;
	};
}

#endif
