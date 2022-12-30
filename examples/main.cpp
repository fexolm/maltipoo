#define SDL_MAIN_HANDLED

#include <fstream>
#include <maltipoo/GPU.h>
#include <maltipoo/backends/vulkan/GPU.h>

#include <glm/glm.hpp>

#include <SDL.h>

static std::vector<uint32_t> readFile(const std::string& filename)
{
	std::ifstream file(filename, std::ios::ate | std::ios::binary);
	if (!file.is_open()) {
		throw std::runtime_error("failed to open file!");
	}

	size_t fileSize = (size_t)file.tellg();
	std::vector<uint32_t> buffer(fileSize / sizeof(uint32_t));
	file.seekg(0);
	file.read((char*)buffer.data(), fileSize);
	file.close();

	return buffer;
}

const std::vector<glm::vec2> positions = { { 0.0f, -0.5f }, { 0.5f, 0.5f }, { -0.5f, 0.5f } };

const std::vector<glm::vec3> colors = { { 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } };

const std::vector<uint16_t> indices = { 0, 1, 2 };

int main()
{
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
	int width = 1280, height = 1024;
	SDL_Window* window = SDL_CreateWindow("SDL Vulkan Sample", 0, 0, 1280, 1024, SDL_WINDOW_VULKAN);

	auto gpu = CreateVulkanGPU(window, width, height);

	auto vertFile = readFile("triangle.vert.spv");
	auto fragFile = readFile("triangle.frag.spv");

	GPUShaderRef vert = gpu->CreateShader(vertFile, ShaderType::Vertex);
	GPUShaderRef frag = gpu->CreateShader(fragFile, ShaderType::Fragment);

	DepthStencilInfo depth_stencil;
	depth_stencil.depthTestEnable = false;

	GraphicsPipelineCreateInfo pipelineInfo;
	pipelineInfo.vertexShader = vert;
	pipelineInfo.fragmentShader = frag;
	pipelineInfo.depthStencil.depthTestEnable = false;
	pipelineInfo.rasterizer.cullMode = CullMode::Back;
	pipelineInfo.rasterizer.faceOrientation = FaceOrientation::Clockwise;

	GPUGraphicsPipelineRef graphicsPipeline = gpu->CreateGraphicsPipeline(pipelineInfo);

	BufferInfo vertexInfo;
	vertexInfo.usage = BufferUsage::Vertex;

	GPUBufferRef positionsBuf = gpu->CreateBuffer(positions.size() * sizeof(glm::vec2), vertexInfo);
	positionsBuf->Write((void*)positions.data(), positions.size() * sizeof(glm::vec2));

	GPUBufferRef colorsBuf = gpu->CreateBuffer(colors.size() * sizeof(glm::vec3), vertexInfo);
	colorsBuf->Write((void*)colors.data(), colors.size() * sizeof(glm::vec3));

	vertexInfo.usage = BufferUsage::Index;
	GPUBufferRef indicesBuf = gpu->CreateBuffer(indices.size() * sizeof(uint16_t), vertexInfo);
	indicesBuf->Write((void*)indices.data(), indices.size() * sizeof(uint16_t));

	bool finished = false;

	while (!finished) {
		SDL_Event e;
		SDL_PollEvent(&e);

		if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
			finished = true;
		}

		gpu->BeginFrame();

		GPUCommandListRef commandList = gpu->CreateCommandList();
		GPUFutureRef<GPUTexture> aquiredImage = gpu->AquireFramebufferImage();

		commandList->Begin();
		commandList->BeginRenderPass(aquiredImage);
		commandList->BindPipeline(graphicsPipeline);

		commandList->SetViewport(200, 200, 800, 600);
		commandList->SetScissors(200, 200, 800, 600);

		commandList->BindVertexBuffer(positionsBuf, 0);
		commandList->BindVertexBuffer(colorsBuf, 1);
		commandList->BindIndexBuffer(indicesBuf);

		commandList->DrawIndexed(indices.size(), 1, 0, 0, 0);

		commandList->EndRenderPass();
		commandList->End();

		gpu->Present(gpu->Submit(commandList));
	}

	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}