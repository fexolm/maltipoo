#define TINYOBJLOADER_IMPLEMENTATION
#define SDL_MAIN_HANDLED

#include "tiny_obj_loader.h"

#include <SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <maltipoo/GPU.h>
#include <maltipoo/backends/vulkan/GPU.h>

static std::vector<uint32_t> readFile(const std::string &filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("failed to open file!");
    }

    size_t fileSize = (size_t)file.tellg();
    std::vector<uint32_t> buffer(fileSize / sizeof(uint32_t));
    file.seekg(0);
    file.read((char *)buffer.data(), fileSize);
    file.close();

    return buffer;
}

struct UniformBufferObject {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};

int main() {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, "teapot.obj")) {
        throw std::runtime_error(warn + err);
    }

    std::vector<float> positions = attrib.vertices;
    std::vector<float> normals = attrib.normals;
    std::vector<uint16_t> indices;

    for (const auto &shape : shapes) {
        for (const auto &index : shape.mesh.indices) {
            indices.push_back(index.vertex_index);
        }
    }

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    int width = 1280, height = 1024;
    SDL_Window *window = SDL_CreateWindow("SDL Vulkan Sample", 0, 0, 1280, 1024, SDL_WINDOW_VULKAN);

    GPUDriverRef gpu = CreateVulkanGPU(window, width, height);

    auto vertFile = readFile("teapot.vert.spv");
    auto fragFile = readFile("teapot.frag.spv");

    GPUShaderRef vert = gpu->CreateShader(vertFile, ShaderType::Vertex);
    GPUShaderRef frag = gpu->CreateShader(fragFile, ShaderType::Fragment);

    GraphicsPipelineCreateInfo pipelineInfo;
    pipelineInfo.vertexShader = vert;
    pipelineInfo.fragmentShader = frag;
    pipelineInfo.depthStencil.depthTestEnable = true;
    pipelineInfo.rasterizer.cullMode = CullMode::Back;
    pipelineInfo.rasterizer.faceOrientation = FaceOrientation::Clockwise;

    GPUGraphicsPipelineRef graphicsPipeline = gpu->CreateGraphicsPipeline(pipelineInfo);

    BufferInfo vertexInfo;
    vertexInfo.usage = BufferUsage::Vertex;

    GPUBufferRef positionsBuf = gpu->CreateBuffer(positions.size() * sizeof(float), vertexInfo);
    positionsBuf->Write((void *)positions.data(), positions.size() * sizeof(float));

    GPUBufferRef normalsBuf = gpu->CreateBuffer(normals.size() * sizeof(glm::vec3), vertexInfo);
    normalsBuf->Write((void *)normals.data(), normals.size() * sizeof(glm::vec3));

    vertexInfo.usage = BufferUsage::Index;
    GPUBufferRef indicesBuf = gpu->CreateBuffer(indices.size() * sizeof(uint16_t), vertexInfo);
    indicesBuf->Write((void *)indices.data(), indices.size() * sizeof(uint16_t));

    bool finished = false;

    while (!finished) {

        UniformBufferObject ubo;
        ubo.view = glm::lookAt(glm::vec3(3.0f, 3.0, 8.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
        ubo.proj = glm::perspective(glm::radians(45.0f), 800.f / 600.f, 0.1f, 20.0f);
        ubo.model = glm::mat4(1.0);

        ubo.model = glm::translate(ubo.model, glm::vec3(-4, -4, 0));

        BufferInfo bufferInfo;
        bufferInfo.usage = BufferUsage::Uniform;

        GPUBufferRef uboBuf = gpu->CreateBuffer(sizeof(UniformBufferObject), bufferInfo);
        uboBuf->Write(&ubo, sizeof(ubo));

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

        commandList->SetViewport(0, 0, 800, 600);
        commandList->SetScissors(0, 0, 800, 600);

        commandList->BindVertexBuffer(positionsBuf, 0);
        commandList->BindVertexBuffer(normalsBuf, 1);
        commandList->BindIndexBuffer(indicesBuf);
        commandList->BindUniformBuffer("uniforms", uboBuf, sizeof(UniformBufferObject));

        commandList->DrawIndexed(indices.size(), 1, 0, 0, 0);

        commandList->EndRenderPass();
        commandList->End();

        gpu->Present(gpu->Submit(commandList));
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}