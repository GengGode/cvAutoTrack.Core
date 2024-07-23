#pragma once
#include <opencv2/opencv.hpp>
//#include "imgui_impl_opengl3_loader.h"

enum class texture_sampler
{
	nearest = 0x2600,
	linear = 0x2601,
	nearest_mipmap_nearest = 0x2700,
	linear_mipmap_nearest = 0x2701,
	nearest_mipmap_linear = 0x2702,
	linear_mipmap_linear = 0x2703
};

bool image_to_texture(cv::Mat& image, uint* out_texture, int* out_width, int* out_height, texture_sampler sample_type = texture_sampler::linear);
void release_texture(uint* texture);
