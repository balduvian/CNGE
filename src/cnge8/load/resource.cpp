
#include <iostream>
#include <functional>

#include "resource.h"

namespace CNGE {
	Resource::Resource(bool hasGather)
		: hasGather(hasGather), gatherStatus(GatherStatus::UNGATHERED), processStatus(ProcessStatus::UNPROCESSED), gatherThread() {}

	Resource::~Resource() {
		joinThread();
	}

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
		return gatherStatus == GatherStatus::GATHERED;
	}

	auto Resource::quickGather(LoadError& error) -> void {
		error.setStage(LoadError::LoadStage::GATHER);
		error.takeOver(customGather());
		gatherStatus = error ? GatherStatus::UNGATHERED : GatherStatus::GATHERED;
	}

	auto Resource::gather(LoadError& error) -> void {
		error.setStage(LoadError::LoadStage::GATHER);
		gatherStatus = GatherStatus::GATHERING;

		auto errorRef = &error;

		gatherThread = std::thread([this, errorRef] {
			errorRef->takeOver(customGather());
			gatherStatus = GatherStatus::GATHERED;
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
		if (needsUnload()) customUnload();
		if (needsDiscard()) customDiscard();
	}

	/* default gather and discard for resources with hasGather = false */

	auto Resource::customGather() -> LoadError {
		return LoadError::none();
	}

	auto Resource::customDiscard() -> LoadError {
		return LoadError::none();
	}

	auto Resource::getProcessStatus() -> ProcessStatus {
		return processStatus;
	}

	auto Resource::getGatherStatus() -> GatherStatus {
		return gatherStatus;
	}
}
