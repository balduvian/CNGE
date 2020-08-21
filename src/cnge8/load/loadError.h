//
// Created by Emmet on 8/19/2020.
//

#ifndef CNGE_8_2_LOADERROR_H
#define CNGE_8_2_LOADERROR_H

#include <string>
#include "../types.h"

namespace CNGE {
	template <typename S>
	concept StringConvertable = requires(S s) {
		std::string(s);
	};

	class LoadError {
	public:
		enum class LoadStage {
			GATHER = 0,
			PROCESS = 1,
			UNLOAD = 2,
			DISCARD = 3,
		};

		constexpr static const char* STAGE_NAMES[4] {
			"Gather",
			"Process",
			"Unload",
			"Discard"
		};

	private:
		i32 occuredOn;
		LoadStage loadStage;

		std::string message;

	public:
		static auto none() -> LoadError;

		LoadError();

		template<StringConvertable T>
		LoadError(T&& message) : message(message) {}

		auto setStage(LoadStage) -> LoadError&;
		auto setOccured(i32) -> LoadError&;

		template <StringConvertable T>
		auto makeErr(T&& message) { this->message = message; }

		operator bool();

		auto takeOver(LoadError&) -> void;
		auto takeOver(LoadError&&) -> void;

		auto getMessage() -> std::string;

		auto getOccured() -> i32;
	};
}

#endif //CNGE_8_2_LOADERROR_H
