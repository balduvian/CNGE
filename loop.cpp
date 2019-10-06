
#include <chrono>
#include <memory>

#include "loop.h"

namespace CNGE7
{

	Loop::t_long Loop::BILLION = 1000000000ll;

	///
	/// will keep loopin and loopin until exit condition
	/// is satisfied,
	/// takes over from anything else on the main thread,
	///
	/// if you try and call this twice then all hell will
	/// rain down upon you
	///
	Loop::Loop(int fps, std::function<bool()> exit_condition, std::function<void(int, t_long, double)> frame)
	{
		// how long a frame lasts in nanoseconds
		auto frame_time = BILLION / fps;

		// set a default value for last time to start
		auto last = std::chrono::high_resolution_clock::now();

		// create frame history, using a maxiumum framerate
		// of which you set using nominal frames
		auto frame_history = std::make_unique<t_long>(fps);
		auto history_start = frame_history.get();
		auto frame_ptr = history_start;

		// the loop exits if exit condition returns true
		while (!exit_condition())
		{
			// what is the time now
			auto now = std::chrono::high_resolution_clock::now();

			// compare it to the last time
			// to get delta time
			auto delta = (now - last).count();

			// if a frame should be rendered
			if (delta >= frame_time)
			{
				// inject delta into history
				*frame_ptr = delta;

				// count the number of fps we have
				auto temp_ptr = frame_ptr;
				auto total_time = 0ll;
				auto frame_tally = 0;

				// this loop will terminate once the full amount of frame have been recorded
				// or the total time exceeds 1 second
				for (; frame_tally < fps; ++frame_tally)
				{
					// if the time we've collected so far
					// goes over a second, then we've
					// found how many frames
					total_time += *temp_ptr;

					if (total_time > BILLION)
						break;

					// go back in time
					--temp_ptr;
					// undeflow and go to end of frame history
					if (temp_ptr < history_start)
						temp_ptr = history_start + fps - 1;
				}

				// do frame and pass in timing info
				// calculate the delta time
				frame(frame_tally, delta, ((double)delta / BILLION));

				// set the baseline for the next frame to be right now
				last = now;

				// increment and wrap frame pointer
				++frame_ptr;
				// overflow frame ptr back to the beginning
				if (frame_ptr == history_start + fps)
					frame_ptr = history_start;
			}
		}
	}

}
