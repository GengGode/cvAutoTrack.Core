#include "image_to_texture.hpp"
// #include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_opengl3_loader.h>

bool image_to_texture(cv::Mat &image, uint *out_texture, int *out_width, int *out_height, texture_sampler sample_type)
{
    if (image.empty())
        return false;
    if (image.channels() == 1)
        cv::cvtColor(image, image, cv::COLOR_GRAY2RGBA);
    else if (image.channels() == 3)
        cv::cvtColor(image, image, cv::COLOR_BGR2BGRA);

    auto image_width = image.cols;
    auto image_height = image.rows;
    auto image_data = image.data;

    // Create a OpenGL texture identifiers
    GLint last_texture = 0;
    GLuint image_texture;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (int)sample_type);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (int)sample_type);
    //glTexParameteri(GL_TEXTURE_2D, 0x2802, 0x812f);
    //glTexParameteri(GL_TEXTURE_2D, 0x2803, 0x812f);

    // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data); //0x80e1
    glBindTexture(GL_TEXTURE_2D, last_texture);

    *out_texture = image_texture;
    *out_width = image_width;
    *out_height = image_height;

    return true;
}

void release_texture(uint *texture)
{
    glDeleteTextures(1, texture);
}
