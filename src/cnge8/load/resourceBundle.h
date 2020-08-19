
#ifndef CNGE_ASSETSMANAGER
#define CNGE_ASSETSMANAGER

#include <initializer_list>
#include <vector>

#include "resource.h"

namespace CNGE {
	class ResourceBundle {
	private:
		/// stores the pointers to resources to load/unload ///
		std::vector<Resource*> resources;
		std::vector<Resource*>::iterator resourcesIter;

		auto  doGather(Resource*       ) -> bool;
		auto  doGather(Resource*, bool&) -> bool;
		auto doProcess(Resource*       ) -> bool;
		auto doProcess(Resource*, bool&) -> bool;
		auto  doUnload(Resource*       ) -> bool;
		auto  doUnload(Resource*, bool&) -> bool;
		auto doDiscard(Resource*       ) -> bool;
		auto doDiscard(Resource*, bool&) -> bool;

	public:
		/// constructing using a static initializer list ///
		ResourceBundle(std::initializer_list<Resource*>);

		/// you cannot copy the asset bundle ///
		ResourceBundle(const ResourceBundle&) = delete;
		ResourceBundle(const ResourceBundle&&) = delete;
		auto operator=(const ResourceBundle&) -> void = delete;

		auto setup() -> ResourceBundle&;

		/// loading that blocks the thread ///
		auto quickUpdateLoad(bool) -> bool;
		auto quickUpdateUnload(bool) -> bool;

		/// call every frame to do multithreaded loading ///
		auto updateLoad(bool) -> bool;
		auto updateUnload(bool) -> bool;

		/// getters ///
		auto getCurrent() -> Resource*;
		auto getAlong() -> i32;
		auto getTotal() -> i32;

		auto done() -> bool;
	};
}

#endif
