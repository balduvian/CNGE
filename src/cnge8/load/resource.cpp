#include <iostream>

#include "resource.h"

namespace CNGE {
	Resource::Resource(bool hasGather)
		: hasGather(hasGather), gatherStatus(GatherStatus::UNGATHERED), processStatus(ProcessStatus::UNPROCESSED), gatherThread() {}

	/// assets manager deals with this after gathering is complete
	/// this is smart and won't crash on a non running thread
	auto Resource::joinThread() -> bool {
		if (gatherThread.joinable()) {
			gatherThread.join();

			/* gather state if customGather() returned true */
			return gatherStatus == GatherStatus::GATHERED;
		}

		return false;
	}

	auto Resource::needsGather() -> bool {
		return hasGather && gatherStatus == GatherStatus::UNGATHERED;
	}

	auto Resource::isGathering() -> bool {
		return gatherStatus == GatherStatus::GATHERING;
	}

	auto Resource::quickGather() -> bool {
		if (customGather()) {
			return gatherStatus = GatherStatus::GATHERED, true;
		} else {
			return false;
		}
	}

	auto Resource::finishedGathering() -> bool {
		return gatherStatus != GatherStatus::GATHERING;
	}

	auto Resource::gather() -> void {
		gatherStatus = GatherStatus::GATHERING;

		gatherThread = std::thread([this] {
			gatherStatus = customGather() ? GatherStatus::GATHERED : GatherStatus::UNGATHERED;
		});
	}

	auto Resource::needsDiscard() -> bool {
		return gatherStatus == GatherStatus::GATHERED;
	}

	auto Resource::discard() -> bool {
		if (customDiscard()) {
			return gatherStatus = GatherStatus::UNGATHERED, true;
		} else {
			return false;
		}
	}

	auto Resource::needsProcess() -> bool {
		return processStatus == ProcessStatus::UNPROCESSED;
	}

	auto Resource::process() -> bool {
		if (customProcess()) {
			return processStatus = ProcessStatus::PROCESSED, true;
		} else {
			return false;
		}
	}

	auto Resource::needsUnload() -> bool {
		return processStatus == ProcessStatus::PROCESSED;
	}

	auto Resource::unload() -> bool {
		if (customUnload()) {
			return processStatus = ProcessStatus::UNPROCESSED, true;
		} else {
			return false;
		}
	}

	auto Resource::getGatherStatus() const -> GatherStatus {
		return gatherStatus;
	}

	auto Resource::getProcessStatus() const -> ProcessStatus {
		return processStatus;
	}

	auto Resource::getHasGather() -> bool {
		return hasGather;
	}

	auto Resource::destroy() -> void {
		if (processStatus == ProcessStatus::PROCESSED) customUnload();
		if (gatherStatus == GatherStatus::GATHERING) joinThread();
		if (gatherStatus == GatherStatus::UNGATHERED) customDiscard();
	}
}
