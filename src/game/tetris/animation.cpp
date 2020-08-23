//
// Created by Emmet on 8/22/2020.
//

#include "animation.h"

namespace Game {
	Animation::Animation(f64 time, std::function<void(f32)> onUpdate, std::function<void(f32)> onRender, std::function<void()> onEnd)
		: timer(time), onUpdate(onUpdate), onRender(onRender), onEnd(onEnd) {
		timer.start();
	}

	auto Animation::update(f64 time) -> bool {
		auto ret = timer.update(time);

		ret ? onEnd() : onUpdate(timer.getAlong());

		return ret;
	}

	auto Animation::render() -> bool {
		onRender(timer.getAlong());
	}

	auto Animation::getTimer() -> CNGE::Timer& {
		return timer;
	}
}
