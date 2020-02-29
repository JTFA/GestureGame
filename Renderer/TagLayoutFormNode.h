#pragma once

#include <FormNode.h>

namespace gameplay {
	class TagLayoutFormNode : public FormNode {
	public:
		TagLayoutFormNode(char *layoutName, char *imageTextPath, char *buttonControlId);

		~TagLayoutFormNode();

		void drawForm(bool wireFrame);

		void controlEvent(Control *control, EventType evt);
	};
}