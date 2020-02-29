
#include <node/WebViewNode.h>
#include <LogUtils.h>

using namespace armakers;

WebViewNode::WebViewNode(const char *id, std::shared_ptr<WebView> webView) :
		gameplay::QuadNode(id, gameplay::QuadNodeType::VIDEO, true), ExtensionRenderable() {
    //TODO:: 임시 코드. 초기 텍스쳐가 없을 때 마지막에 생성한 텍스쳐가 일시적으로 보이는 현상이 나타남
    setTexture("res/png/html_logo.png", true);
	this->webView = webView;
}

WebViewNode::WebViewNode(const char *id, std::shared_ptr<WebView> webView, float alpha) :
		gameplay::QuadNode(id, gameplay::QuadNodeType::VIDEO, alpha), ExtensionRenderable()  {
	//TODO:: 임시 코드. 초기 텍스쳐가 없을 때 마지막에 생성한 텍스쳐가 일시적으로 보이는 현상이 나타남
	setTexture("res/png/html_logo.png", true);
	this->webView = webView;
}

WebViewNode::~WebViewNode() {
	LOGD("WebViewNode Destructor");
    webView.reset();
}

void WebViewNode::extensionUpdate(int32_t elapsedTime) {
    if (webView->isDrawable(material)) {
        material->getStateBlock()->setCullFace(false);
        webView->update();
    }
}

void WebViewNode::setEnabled(bool enabled)
{
//	LOGD("WebViewNode enabled : %d", enabled);
	QuadNode::setEnabled(enabled);
}