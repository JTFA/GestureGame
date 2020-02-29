//  Copyright © 2018년 Maxst. All rights reserved.

#pragma once

#include <string>
#include <memory>
#include "gameplay.h"

namespace armakers {
	class WebView {
	public:
		static std::shared_ptr<WebView> create(std::string name, std::string url, int w, int h);

		WebView(std::string name, std::string url, int w, int h) {
			this->name = name;
			this->url = url;
			width = w;
			height = h;
		}

		virtual ~WebView() {}

		virtual void createObject(int width, int height, int textureId) = 0;

		virtual bool isDrawable(gameplay::Material *material) = 0;

		virtual void update() = 0;

		virtual void destroyObject() = 0;

		const char *getName() { return name.c_str(); }

	protected:
		std::string name;
		std::string url;
		int width;
		int height;
	};
}
