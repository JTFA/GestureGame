//
// Created by benedict.yim on 2019-08-25.
//

#pragma once

#include "gameplay.h"
#include "Texture.h"

namespace gameplay {
    class Image;
}

enum ImageType {
    IMAGE_UNSUPPORT             = -1,
    IMAGE_JPEG                  = 0,
    IMAGE_TIFF                  = 1,
    IMAGE_WEBP                  = 2,
};

class ImageHelper {
public:
    ImageHelper();
    virtual ~ImageHelper();

    bool checkSupportedExt(const char* path);
    gameplay::Texture::Sampler* createSampler(const char* path, bool generateMipmaps);

private:
    gameplay::Image* processJPEG(const char* path);
    gameplay::Image* processTIFF(const char* path);
    gameplay::Image* processWEBP(const char* path);
    gameplay::Image* createImageFromRGB(const char* path);
    gameplay::Image* createImageFromRGBA(const char* path);


    ImageType _type;
};
