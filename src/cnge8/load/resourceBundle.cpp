
#include <vector>
#include <stdexcept>

#include "resourceBundle.h"

namespace CNGE {
	ResourceBundle::ResourceBundle(std::initializer_list<Resource*> list) :
		resources(list), resourcesIter(resources.begin())
	{}

	auto ResourceBundle::done() -> bool {
		return resourcesIter == resources.end();
	}

	auto ResourceBundle::setup() -> ResourceBundle& {
		resourcesIter = resources.begin();

		return *this;
	}

	auto ResourceBundle::quickUpdateLoad(bool clean) -> bool {
		for (auto resource : resources)
			if (!doGather(resource) || !doProcess(resource) || (clean && !doDiscard(resource))) return false;

		return true;
	}

	auto ResourceBundle::quickUpdateUnload(bool clean) -> bool {
		for (auto resource : resources)
			if (!doUnload(resource) || (clean && !doDiscard(resource))) return false;

		return true;
	}

	auto ResourceBundle::doGather(Resource* resource) -> bool {
		return resource->needsGather() ? resource->quickGather() : true;
	}

	auto ResourceBundle::doGather(Resource* resource, bool& skip) -> bool {
		if (resource->isGathering())
			if (resource->finishedGathering())
				return (skip = true), resource->joinThread();
			else
				return true;
		else
			if (resource->needsGather())
				return resource->gather(), true;
			else
				return (skip = true), true;
	}

	auto ResourceBundle::doProcess(Resource* resource) -> bool {
		return resource->needsProcess() ? resource->process() : true;
	}

	auto ResourceBundle::doProcess(Resource* resource, bool& skip) -> bool {
		return resource->needsProcess()
			? resource->process()
			: (skip = true), true;
	}

	auto ResourceBundle::doUnload(Resource* resource) -> bool {
		return resource->needsUnload()
	        ? resource->unload()
	        : true;
	}

	auto ResourceBundle::doUnload(Resource* resource, bool& skip) -> bool {
		return resource->needsUnload()
	        ? resource->unload()
	        : (skip = true), true;
	}

	auto ResourceBundle::doDiscard(Resource* resource) -> bool {
		return resource->needsDiscard()
	        ? resource->discard()
	        : true;
	}

	auto ResourceBundle::doDiscard(Resource* resource, bool& skip) -> bool {
		return resource->needsDiscard()
	       ? resource->discard()
	       : (skip = true), true;
	}

	auto ResourceBundle::updateLoad(bool clean) -> bool {
		auto skip = true;

		for (; !done() && skip; ++resourcesIter) {
			skip = false;
			if(!doGather(*resourcesIter, skip)) return false;

			if (skip) {
				if(!doProcess(*resourcesIter, skip)) return false;
				if(clean && !doDiscard(*resourcesIter, skip)) return false;
			}
		}

		return true;
	}

	auto ResourceBundle::updateUnload(bool clean) -> bool {
		auto skip = true;

		for (; !done() && skip; ++resourcesIter) {
			skip = false;

			if(!doUnload(*resourcesIter, skip)) return false;
			if(clean && !doDiscard(*resourcesIter, skip)) return false;
		}

		return true;
	}

	auto ResourceBundle::getCurrent() -> Resource* {
		return *resourcesIter;
	}

	auto ResourceBundle::getAlong() -> i32 {
		return resourcesIter - resources.begin();
	}

	auto ResourceBundle::getTotal() -> i32 {
		return resources.size();
	}
}
