
#pragma once

#include <functional>

namespace CNGE7
{

	class Loop
	{
	private:

		using t_long = long long;

		static t_long BILLION;

	public:

		///
		/// will keep loopin and loopin until exit condition
		/// is satisfied,
		/// takes over from anything else on the main thread,
		///
		/// if you try and call this twice then all hell will
		/// rain down upon you
		///
		Loop(int fps, std::function<bool()> exit_condition, std::function<void(int, t_long, double)> frame);

	};

}
