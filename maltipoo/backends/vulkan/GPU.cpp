#include "GPU.h"
#include "VulkanGPU.h"

GPUDriverRef CreateVulkanGPU(SDL_Window *window, int width, int height)
{
	return GPUDriverRef(new VulkanGPU(window, width, height));
}