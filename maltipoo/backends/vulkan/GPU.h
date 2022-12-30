#pragma once

#include "SDL2/SDL.h"
#include "maltipoo/GPU.h"

GPUDriverRef CreateVulkanGPU(SDL_Window *window, int width, int height);
