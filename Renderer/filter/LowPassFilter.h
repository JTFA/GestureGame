#pragma once

#include "gameplay.h"

namespace armakers {
	class LowPassFilter {
	public:
		LowPassFilter();

		virtual ~LowPassFilter();

		static void update(gameplay::Quaternion &prev, gameplay::Quaternion &current,
						   gameplay::Quaternion *out);
	};
}


