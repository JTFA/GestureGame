#pragma once

#include "gameplay.h"
#include <string>
#include <list>
#include <vector>

#include <MKitHelper.h>

namespace gameplay {
	class FormNode : public Node, public Control::Listener {
	public:
		FormNode(const char *id);

		FormNode(const char *layoutName, const char *id);

		~FormNode();

		//virtual void drawForm(bool wireFrame) = 0;
		virtual void controlEvent(Control *control, EventType evt) = 0;

		gameplay::Rectangle getFormBounds();

		virtual void applyCheckState(const char *controlId, bool isChecked);

		virtual void onDraw(float eclipseTime);

	protected:
		virtual void applyAlpha(float alpha);
        virtual void applyAlphaImproveContrast(float alpha, Control *_control);
        virtual void applyAlphaImproveContrast(float alpha, int args, ...);
        virtual void applyAlphaImproveContrast(float alpha, std::vector<Control*> &controls);
	protected:
		Form *form = nullptr;
	};
}


