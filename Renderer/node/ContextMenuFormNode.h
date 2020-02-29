#pragma once

#include <string>
#include <VideoPlayer.h>
#include <FormNode.h>

namespace armakers {
	class ContextMenuFormNode : public gameplay::FormNode {
	public:
		ContextMenuFormNode(std::string &controlId, const char *formName, float alpha);

		~ContextMenuFormNode();

		void drawForm(bool wireFrame);

		void controlEvent(gameplay::Control *control, gameplay::Control::Listener::EventType evt);

		void setBadgeText(char *text);

		void setVisibleBadge(bool visible);

		void setEnableMenu(bool enable);

	protected:
		virtual void applyAlpha(float alpha);

	private:
		void setOnClickListener(gameplay::Control *control, int eventFlags, const char *eventId);

	private:
		std::string controlId;
		float alpha;

		std::list<gameplay::Control *> menuDisable;
	};
}