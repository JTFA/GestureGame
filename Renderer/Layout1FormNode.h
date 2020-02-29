#pragma once

#include <FormNode.h>

namespace gameplay {
	class Layout1FormNode : public FormNode {
	public:
		Layout1FormNode(char *layoutName, char *imageTextPath, char *checkControlId,
						char *buttonControlId);

		~Layout1FormNode();

		void drawForm(bool wireFrame);

		void controlEvent(Control *control, EventType evt);
	};
}