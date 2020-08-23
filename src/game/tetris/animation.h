//
// Created by Emmet on 8/22/2020.
//

#ifndef CNGE_8_2_ANIMATION_H
#define CNGE_8_2_ANIMATION_H

#include "cnge8/util/timer.h"

#include <functional>

namespace Game {
	class Animation {
	private:
		CNGE::Timer timer;

		std::function<void(f32)> onUpdate;
		std::function<void(f32)> onRender;
		std::function<void()> onEnd;

	public:
		Animation(f64, std::function<void(f32)>, std::function<void(f32)>, std::function<void()>);

		auto update(f64 time) -> bool;
		auto render() -> bool;

		auto getTimer() -> CNGE::Timer&;
	};
}

#endif //CNGE_8_2_ANIMATION_H
