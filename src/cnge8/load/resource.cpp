#include <iostream>

#include "resource.h"

namespace CNGE {
	Resource::Resource(bool hasGather)
		: hasGather(hasGather), gatherStatus(GatherStatus::UNGATHERED), processStatus(ProcessStatus::UNPROCESSED), gatherThread() {}

	/* gather */

	auto Resource::joinThread() -> void {
		if (gatherThread.joinable())
			gatherThread.join();
	}

	auto Resource::needsGather() -> bool {
		return hasGather && gatherStatus == GatherStatus::UNGATHERED;
	}

	auto Resource::isGathering() -> bool {
		return gatherStatus == GatherStatus::GATHERING;
	}

	auto Resource::finishedGathering() -> bool {
		return gatherStatus != GatherStatus::GATHERING;
	}

	auto Resource::quickGather(LoadError& error) -> void {
		error.setStage(LoadError::LoadStage::GATHER);
		error.takeOver(customGather());
		gatherStatus = error ? GatherStatus::UNGATHERED : GatherStatus::GATHERED;
	}

	auto Resource::gather(LoadError& error) -> void {
		error.setStage(LoadError::LoadStage::GATHER);
		gatherStatus = GatherStatus::GATHERING;

		gatherThread = std::thread([this, &error] {
			error.takeOver(customGather());
			gatherStatus = error ? GatherStatus::UNGATHERED : GatherStatus::GATHERED;
		});
	}

	/* discard */

	auto Resource::needsDiscard() -> bool {
		return gatherStatus == GatherStatus::GATHERED;
	}

	auto Resource::discard(LoadError& error) -> void {
		error.setStage(LoadError::LoadStage::DISCARD);
		error.takeOver(customDiscard());
		gatherStatus = error ? GatherStatus::GATHERED : GatherStatus::UNGATHERED;
	}

	/* process */

	auto Resource::needsProcess() -> bool {
		return processStatus == ProcessStatus::UNPROCESSED;
	}

	auto Resource::process(LoadError& error) -> void {
		error.setStage(LoadError::LoadStage::PROCESS);
		error.takeOver(customProcess());
		processStatus = error ? ProcessStatus::UNPROCESSED : ProcessStatus::PROCESSED;
	}

	/* unload */

	auto Resource::needsUnload() -> bool {
		return processStatus == ProcessStatus::PROCESSED;
	}

	auto Resource::unload(LoadError& error) -> void {
		error.setStage(LoadError::LoadStage::UNLOAD);
		error.takeOver(customUnload());
		processStatus = error ? ProcessStatus::PROCESSED : ProcessStatus::UNPROCESSED;
	}

	auto Resource::destroy() -> void {
		if (processStatus == ProcessStatus::PROCESSED) customUnload();
		if (gatherStatus == GatherStatus::GATHERING) joinThread();
		if (gatherStatus == GatherStatus::UNGATHERED) customDiscard();
	}

	/* default gather and discard for resources with hasGather = false */

	auto Resource::customGather() -> LoadError {
		return LoadError::none();
	}

	auto Resource::customDiscard() -> LoadError {
		return LoadError::none();
	}
}
