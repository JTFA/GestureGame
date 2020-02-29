#pragma once

#include <control/WebView.h>
#include <QuadNode.h>
#include <node/ExtensionRenderable.h>

namespace armakers {
	class WebViewNode : public gameplay::QuadNode, public ExtensionRenderable {
	public:
		WebViewNode(const char *id, std::shared_ptr<WebView> webView);

		WebViewNode(const char *id, std::shared_ptr<WebView> webView, float alpha);

		virtual ~WebViewNode();

		virtual void extensionUpdate(int32_t elapsedTime);

		void setEnabled(bool enabled);

	private:
		std::shared_ptr<WebView> webView = nullptr;
	};
}


