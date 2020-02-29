#pragma once

#include "gameplay.h"
#include <string>
#include <FormNode.h>
#include <model/ARComplexViewer.h>

namespace armakers {
	class TemplateFormNode : public gameplay::FormNode {
	public:
		TemplateFormNode(std::string &controlId, const char *formName,
				std::shared_ptr<ARComplexViewer> viewer);

		~TemplateFormNode();

		void drawForm(bool wireFrame);

		void controlEvent(gameplay::Control *control, gameplay::Control::Listener::EventType evt);

		void measureContentsTransform(gameplay::Vector3 &pos, gameplay::Vector3 &scale);

		virtual void applyAlpha(float alpha);
		virtual void onDraw(float eclipseTime);
        void updateControlVisible();
	private:
        void updateTitleText();
		void updateLabelText();
		void setOnClickListener(gameplay::Control *control, int eventFlags, const char *eventId);

	private:
		std::string controlId;
		std::string videoControlId;
		std::string pageControlId;
		std::shared_ptr<ARComplexViewer> viewer;
		std::vector<gameplay::Control *> texts;
		std::vector<gameplay::Control *> controls;
		std::vector<gameplay::Control *> labelBGs;
	};
}