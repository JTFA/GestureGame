#pragma once

#include <FormNode.h>

namespace armakers {
	class ImagePagesFormNode : public gameplay::FormNode {
	public:
		ImagePagesFormNode(char *layoutName, char *controlId, std::vector<std::string> &lst, int imageWidth,
							int imageHeight, bool sizeFit, float alpha);

		~ImagePagesFormNode();
        void changeImagePage(int opt);

		void drawForm(bool wireFrame);

		void controlEvent(gameplay::Control *control, gameplay::Control::Listener::EventType evt);

		void setLocalPathList(std::vector<std::string> &lst) {
			this->localPathList = lst;
			this->currentIndex = 0; }


	private:
		std::string controlId;
		gameplay::ImageControl *imageControl;
        gameplay::Control *bottomControl;
		std::vector<std::string> localPathList;
		int32_t currentIndex;

        const char* getCurrentImagePath() { return localPathList[currentIndex].c_str(); }
        void setImagePath(const char *imagePath);

		void setOnClickListener(gameplay::Control *control, int eventFlags, const char *eventId);
	};
}