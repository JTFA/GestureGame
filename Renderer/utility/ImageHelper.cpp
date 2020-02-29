//
// Created by benedict.yim on 2019-08-25.
//

#include <iostream>
#include <string>

#include "Base.h"
#include "Image.h"
#include "FileSystem.h"

//cv
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

//log
#include <LogUtils.h>

#include <utility/ImageHelper.h>

using namespace gameplay;

ImageHelper::ImageHelper() {
    _type = IMAGE_UNSUPPORT;
}

ImageHelper::~ImageHelper() {
    //LOGD("ImageHelper Destructor");
}

bool ImageHelper::checkSupportedExt(const char *path) {
    bool ret = false;
    const char* ext = strrchr(FileSystem::resolvePath(path), '.');
    if (ext) {
        switch (strlen(ext)) {
            case 4: {
                if (tolower(ext[1]) == 'j' && tolower(ext[2]) == 'p' && tolower(ext[3]) == 'g') {
                    ret = true;
                    _type = IMAGE_JPEG;
                } else if (tolower(ext[1]) == 't' && tolower(ext[2]) == 'i' && tolower(ext[3]) == 'f') {
                    ret = true;
                    _type = IMAGE_TIFF;
                }
            } break;
            case 5: {
                if (tolower(ext[1]) == 'j' && tolower(ext[2]) == 'p'
                    && tolower(ext[3]) == 'e' && tolower(ext[4]) == 'g') {
                    ret = true;
                    _type = IMAGE_JPEG;
                } else if (tolower(ext[1]) == 't' && tolower(ext[2]) == 'i'
                          && tolower(ext[3]) == 'f' && tolower(ext[4]) == 'f') {
                    ret = true;
                    _type = IMAGE_TIFF;
                } else if (tolower(ext[1]) == 'w' && tolower(ext[2]) == 'e'
                           && tolower(ext[3]) == 'b' && tolower(ext[4]) == 'p') {
                    ret = true;
                    _type = IMAGE_WEBP;
                }
            } break;
            default: break;
        }
    }
    return ret;
}

Texture::Sampler* ImageHelper::createSampler(const char* path, bool generateMipmaps) {
    //LOGD("ImageHelper createSampler (%d)", generateMipmaps);
    Texture* texture = nullptr;
    Image *image = nullptr;
    switch (_type) {
        case IMAGE_JPEG : image = processJPEG(path); break;
        case IMAGE_TIFF : image = processTIFF(path); break;
        case IMAGE_WEBP : image = processWEBP(path); break;
        default: break;
    }
    if (image) texture = Texture::create(image, generateMipmaps);
    SAFE_RELEASE(image);
    return texture ? Texture::Sampler::create(texture) : nullptr;
}

Image* ImageHelper::processJPEG(const char* path) {
    return createImageFromRGB(path);
}

Image* ImageHelper::processTIFF(const char* path) {
    Image* result = createImageFromRGBA(path);
    if (!result) result = createImageFromRGB(path);
    return result;
}

Image* ImageHelper::processWEBP(const char* path) {
    Image* result = createImageFromRGBA(path);
    if (!result) result = createImageFromRGB(path);
    return result;
}

Image* ImageHelper::createImageFromRGB(const char* path) {
    cv::Mat source = cv::imread(path, cv::IMREAD_COLOR);
    //LOGD("ImageHelper createImageFromRGB (%d)(%d)(%d)(%s)", img.cols, img.rows, img.channels(), path);
    cv::Mat img;
    cv::flip(source, img, 0);
    std::vector<cv::Mat> bgr_channels(3);
    std::vector<cv::Mat> channels(3);
    cv::split(img, bgr_channels);
    //Changes the Red and Blue
    channels[0]=bgr_channels[2];
    channels[1]=bgr_channels[1];
    channels[2]=bgr_channels[0];
    cv::Mat rgbImg;
    cv::merge(channels, rgbImg);
    return Image::create(rgbImg.cols, rgbImg.rows, Image::Format::RGB, rgbImg.data);
}

Image* ImageHelper::createImageFromRGBA(const char* path) {
    cv::Mat source = cv::imread(path, cv::IMREAD_UNCHANGED);
    if (source.channels() != 4) return nullptr;
    cv::Mat img;
    cv::flip(source, img, 0);
    //LOGD("ImageHelper createImageFromRGBA (%d)(%d)(%d)(%s)", img.cols, img.rows, img.channels(), path);
    std::vector<cv::Mat> bgra_channels(4);
    std::vector<cv::Mat> channels(4);
    cv::split(img, bgra_channels);
    //Changes the Red and Blue
    channels[0] = bgra_channels[2];
    channels[1] = bgra_channels[1];
    channels[2] = bgra_channels[0];
    channels[3] = bgra_channels[3];
    cv::Mat rgbaImg;
    cv::merge(channels, rgbaImg);
    return Image::create(rgbaImg.cols, rgbaImg.rows, Image::Format::RGBA, rgbaImg.data);
}