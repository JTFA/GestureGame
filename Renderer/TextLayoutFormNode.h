#pragma once

#include <FormNode.h>

namespace gameplay {
	class TextLayoutFormNode : public FormNode {
	public:
		TextLayoutFormNode(char *layoutName, char *imagePath, char *controlId);

		~TextLayoutFormNode();

		void drawForm(bool wireFrame);

		void controlEvent(Control *control, EventType evt);
	};
}