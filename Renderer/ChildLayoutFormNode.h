#pragma once

#include <FormNode.h>

namespace gameplay {
	class ChildLayoutFormNode : public FormNode {
	public:
		ChildLayoutFormNode(char *layoutName, char *imageTextPath, char *checkControlId,
							char *buttonControlId);

		~ChildLayoutFormNode();

		void drawForm(bool wireFrame);

		void controlEvent(Control *control, EventType evt);
	};
}