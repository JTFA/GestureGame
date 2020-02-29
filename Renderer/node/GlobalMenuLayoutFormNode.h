#pragma once

#include <string>
#include <VideoPlayer.h>
#include <FormNode.h>

namespace armakers {
	class GlobalMenuLayoutFormNode : public gameplay::FormNode {
	public:
		GlobalMenuLayoutFormNode(char *layoutName, char *controlId, float alpha);

		~GlobalMenuLayoutFormNode();

		void drawForm(bool wireFrame);

		void controlEvent(gameplay::Control *control, gameplay::Control::Listener::EventType evt);

		void setBadgeText(char *text);

		void setVisibleBadge(bool visible);

		virtual void onDraw(float eclipseTime);

	protected:
		virtual void applyAlpha(float alpha);

	private:
		void setOnClickListener(gameplay::Control *control, int eventFlags, const char *eventId);

		void checkTimeout(float eclipseTime);

	private:
		std::string controlId;
		long timeoutMS;
		float alpha;
	};
}