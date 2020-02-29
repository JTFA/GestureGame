#pragma once

#include <FormNode.h>

namespace armakers {
	class ImageLayoutFormNode : public gameplay::FormNode {
	public:
		ImageLayoutFormNode(char *layoutName, char *imagePath, int imageWidth, int imageHeight,
							char *controlId);

		ImageLayoutFormNode(char *layoutName, char *controlId, char *imagePath, int imageWidth,
							int imageHeight, bool sizeFit, float alpha);

		~ImageLayoutFormNode();

		void drawForm(bool wireFrame);

		void controlEvent(gameplay::Control *control, gameplay::Control::Listener::EventType evt);

		void setImagePath(const char *imagePath);

	private:
        gameplay::ImageControl *imageControl;

	};
}