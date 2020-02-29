#pragma once

#include <string>
#include "gameplay.h"
#include "Node.h"

namespace armakers {
	class ExtensionRenderable {
	public:
        ExtensionRenderable();

		~ExtensionRenderable();

		virtual void extensionUpdate(int32_t elapsedTime);
	};
}