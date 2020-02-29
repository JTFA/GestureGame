#pragma once

#include <FormNode.h>

namespace gameplay {
	class IconLayoutFormNode : public FormNode {
	public:
		IconLayoutFormNode(char *layoutName, char *imagePath);

		~IconLayoutFormNode();

		void drawForm(bool wireFrame);

		void controlEvent(Control *control, EventType evt);
	};
}