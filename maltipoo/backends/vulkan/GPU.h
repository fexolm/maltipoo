#pragma once

#include "maltipoo/GPU.h"
#include "SDL2/SDL.h"

GPUDriverRef CreateVulkanGPU(SDL_Window *window, int width, int height);
