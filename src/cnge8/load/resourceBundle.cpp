
#include <vector>
#include <stdexcept>
#include <iostream>

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

	auto ResourceBundle::quickUpdateLoad(bool clean) -> void {
		auto error = LoadError();

		for (auto resource : resources) {
			error.setOccured(getAlong(resource));

			if (doGather(resource, error)) errorExit(error);
			if (doProcess(resource, error)) errorExit(error);
			if (clean && doDiscard(resource, error)) errorExit(error);
		}
	}

	auto ResourceBundle::quickUpdateUnload(bool clean) -> void {
		auto error = LoadError();

		for (auto resource : resources) {
			error.setOccured(getAlong(resource));

			if (doUnload(resource, error)) errorExit(error);
			if (clean && doDiscard(resource, error)) errorExit(error);
		}
	}

	/* gather */

	auto ResourceBundle::doGather(Resource* resource, LoadError& error) -> bool {
		if(resource->needsGather()) return resource->quickGather(error), error;
		return false;
	}

	auto ResourceBundle::doGather(Resource* resource, bool& skip, LoadError& error) -> bool {
		if (resource->isGathering())
			if (resource->finishedGathering())
				return resource->joinThread(), error;
			else
				return skip = false, false;
		else
			if (resource->needsGather())
				return skip = false, resource->gather(error), false;
			else
				return false;
	}

	/* process */

	auto ResourceBundle::doProcess(Resource* resource, LoadError& error) -> bool {
		if(resource->needsProcess()) return resource->process(error), error;
		return false;
	}

	auto ResourceBundle::doProcess(Resource* resource, bool& skip, LoadError& error) -> bool {
		if(resource->needsGather()) return skip = false, resource->process(error), error;
		return false;
	}

	/* unload */

	auto ResourceBundle::doUnload(Resource* resource, LoadError& error) -> bool {
		if(resource->needsProcess()) return resource->unload(error), error;
		return false;
	}

	auto ResourceBundle::doUnload(Resource* resource, bool& skip, LoadError& error) -> bool {
		if(resource->needsGather()) return skip = false, resource->unload(error), error;
		return false;
	}

	/* discard */

	auto ResourceBundle::doDiscard(Resource* resource, LoadError& error) ->bool {
		if(resource->needsDiscard()) return resource->discard(error), error;
		return false;
	}

	auto ResourceBundle::doDiscard(Resource* resource, bool& skip, LoadError& error) -> bool {
		if(resource->needsDiscard()) return skip = false, resource->discard(error), error;
		return false;
	}

	auto ResourceBundle::updateLoad(bool clean) -> void {
		auto skip = true;
		auto error = LoadError();

		for (; !done() && skip; ++resourcesIter) {
			error.setOccured(getAlong());

			if(doGather(*resourcesIter, skip, error)) errorExit(error);
			if (skip) {
				if(doProcess(*resourcesIter, skip, error)) errorExit(error);
				if(clean && doDiscard(*resourcesIter, skip, error)) errorExit(error);
			}
		}
	}

	auto ResourceBundle::updateUnload(bool clean) -> void {
		auto skip = true;
		auto error = LoadError();

		for (; !done() && skip; ++resourcesIter) {
			error.setOccured(getAlong());

			if(doUnload(*resourcesIter, skip, error)) errorExit(error);
			if(clean && doDiscard(*resourcesIter, skip, error)) errorExit(error);
		}
	}

	auto ResourceBundle::errorExit(LoadError& error) -> void {
		std::cout << error.getMessage() << std::endl;
		exit(0xc000000a);
	}

	auto ResourceBundle::getCurrent() -> Resource* {
		return *resourcesIter;
	}

	auto ResourceBundle::getAlong() -> i32 {
		return resourcesIter - resources.begin();
	}

	auto ResourceBundle::getAlong(Resource* resource) -> i32 {
		return resource - resources[0];
	}

	auto ResourceBundle::getTotal() -> i32 {
		return resources.size();
	}
}
