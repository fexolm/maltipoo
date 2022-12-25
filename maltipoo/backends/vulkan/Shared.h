#pragma once

#define VULKAN_GPU_SAFE_CALL(Result)   \
	do {                               \
		if ((Result) != VK_SUCCESS) {} \
	} while (0)
