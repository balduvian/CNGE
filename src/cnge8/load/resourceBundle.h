
#ifndef CNGE_ASSETSMANAGER
#define CNGE_ASSETSMANAGER

#include <initializer_list>
#include <vector>

#include "loadError.h"
#include "resource.h"

namespace CNGE {
	class ResourceBundle {
	private:
		static auto errorExit(LoadError&) -> void;
		auto getAlong(Resource*) -> i32;

		/// stores the pointers to resources to load/unload ///
		std::vector<Resource*> resources;
		std::vector<Resource*>::iterator resourcesIter;

		auto  doGather(Resource*,        LoadError&) -> bool;
		auto  doGather(Resource*, bool&, LoadError&) -> bool;
		auto doProcess(Resource*,        LoadError&) -> bool;
		auto doProcess(Resource*, bool&, LoadError&) -> bool;
		auto  doUnload(Resource*,        LoadError&) -> bool;
		auto  doUnload(Resource*, bool&, LoadError&) -> bool;
		auto doDiscard(Resource*,        LoadError&) -> bool;
		auto doDiscard(Resource*, bool&, LoadError&) -> bool;

	public:
		/// constructing using a static initializer list ///
		ResourceBundle(std::initializer_list<Resource*>);

		/// you cannot copy the asset bundle ///
		ResourceBundle(const ResourceBundle&) = delete;
		ResourceBundle(const ResourceBundle&&) = delete;
		auto operator=(const ResourceBundle&) -> void = delete;

		auto setup() -> ResourceBundle&;

		/// loading that blocks the thread ///
		auto quickUpdateLoad(bool) -> void;
		auto quickUpdateUnload(bool) -> void;

		/// call every frame to do multithreaded loading ///
		auto updateLoad(bool) -> void;
		auto updateUnload(bool) -> void;

		/// getters ///
		auto getCurrent() -> Resource*;
		auto getAlong() -> i32;
		auto getTotal() -> i32;

		auto done() -> bool;
	};
}

#endif
