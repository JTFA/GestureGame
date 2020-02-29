#pragma once

#include <FormNode.h>

namespace gameplay {
	class Layout2FormNode : public FormNode {
	public:
		Layout2FormNode(char *layoutName, char *imageTextPath, char *checkControlId,
						char *buttonControlId);

		~Layout2FormNode();

		void drawForm(bool wireFrame);

		void controlEvent(Control *control, EventType evt);

		void changeEnable();
	};
}