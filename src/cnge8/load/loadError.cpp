//
// Created by Emmet on 8/19/2020.
//

#include <iostream>
#include "loadError.h"

namespace CNGE {
	LoadError::LoadError() : message() {}

	auto LoadError::setStage(LoadStage loadStage) -> LoadError& {
		this->loadStage = loadStage;
		return *this;
	}

	auto LoadError::setOccured(i32 occuredOn) -> LoadError& {
		this->occuredOn = occuredOn;
		return *this;
	}

	LoadError::operator bool() {
		return !message.empty();
	}

	auto LoadError::takeOver(LoadError& returnedError) -> void {
		if (returnedError) message = std::move(returnedError.message);
	}

	auto LoadError::takeOver(LoadError&& returnedError) -> void {
		if (returnedError) message = std::move(returnedError.message);
	}

	auto LoadError::getMessage() -> std::string  {
		return std::string("LOAD ERROR on stage ") + STAGE_NAMES[i32(loadStage)] + " | on load# " + std::to_string(occuredOn) + " | " + message;
	}

	auto LoadError::none() -> LoadError {
		return LoadError();
	}

	auto LoadError::getOccured() -> i32 {
		return occuredOn;
	}
}
