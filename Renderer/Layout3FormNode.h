#pragma once

#include <FormNode.h>

namespace gameplay {
	class Layout3FormNode : public FormNode {
	public:
		Layout3FormNode(char *layoutName, char *checkControlId, char *buttonControlId);

		~Layout3FormNode();

		void drawForm(bool wireFrame);

		void controlEvent(Control *control, EventType evt);
	};
}