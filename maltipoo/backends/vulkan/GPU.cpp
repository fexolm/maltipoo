#include "GPU.h"
#include "VulkanGPU.h"

GPUDriverRef CreateVulkanGPU()
{
	return GPUDriverRef(new VulkanGPU());
}