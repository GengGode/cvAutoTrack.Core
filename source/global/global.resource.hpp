#pragma once
#include <global.error.h>

#include <memory>
#include <string>

namespace cv
{
    class Mat;
} // namespace cv

struct ResourceInface
{
    enum class ResourceType
    {
        dll_export,
        binary_load,
        api_net,
    };

    virtual int init(std::string &json_content) {return referr("基类方法未实现"); }
    virtual int load(){return referr("基类方法未实现"); }
    virtual int unload(){return referr("基类方法未实现"); }
    virtual int get_image(std::shared_ptr<cv::Mat> &image){return referr("基类方法未实现"); }
    virtual int get_text(std::string &text) {return referr("基类方法未实现"); }
};

struct ResourceInfaceFactory
{
    static std::shared_ptr<ResourceInface> create(ResourceInface::ResourceType type, std::string &init_json);
};