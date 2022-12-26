#pragma once

#include <memory>
#include <vector>
#include <vulkan/vulkan.h>
#include <string>

enum class BufferUsage
{
	Vertex,
	Index,
	Uniform,
	Staging,
};

enum VertexElementType
{
	None,
	Float1,
	Float2,
	Float3,
	Float4,
	PackedNormal, // FPackedNormal
	UByte4,
	UByte4N,
	Color,
	Short2,
	Short4,
	Short2N, // 16 bit word normalized to (value/32767.0,value/32767.0,0,0,1)
	Half2,	 // 16 bit float using 1 bit sign, 5 bit exponent, 10 bit mantissa
	Half4,
	Short4N, // 4 X 16 bit word, normalized
	UShort2,
	UShort4,
	UShort2N,  // 16 bit word normalized to (value/65535.0,value/65535.0,0,0,1)
	UShort4N,  // 4 X 16 bit word unsigned, normalized
	URGB10A2N, // 10 bit r, g, b and 2 bit a normalized to (value/1023.0f, value/1023.0f, value/1023.0f, value/3.0f)
	UInt,
	MAX,

	NumBits = 5,
};

struct BufferInfo
{
	BufferUsage usage;
};

class GPUResource
{
public:
	virtual ~GPUResource() = default;
};

class GPUBuffer : public GPUResource
{
public:
	virtual ~GPUBuffer() override = default;

	virtual void Write(void *data, uint32_t size) = 0;
};

class GPUTexture : public GPUResource
{
};

using GPUTextureRef = std::shared_ptr<GPUTexture>;

class GPUSampler : public GPUResource
{
};

using GPUSamplerRef = std::shared_ptr<GPUSampler>;

class GPUGraphicsPipeline
{
public:
	virtual ~GPUGraphicsPipeline() = default;
};

using GPUGraphicsPipelineRef = std::shared_ptr<GPUGraphicsPipeline>;

enum class ShaderType
{
	Vertex,
	Fragment
};

struct GPUShader : GPUResource
{
};

using GPUShaderRef = std::shared_ptr<GPUShader>;

class GPURenderTarget
{
};

using GPURenderTargetRef = std::shared_ptr<GPURenderTarget>;

struct DepthStencilInfo
{
	bool depthTestEnable{false};
};

enum class CullMode
{
	None,
	Back,
	Front,
};

enum class FaceOrientation
{
	Clockwise,
	CounterClockwise,
};

struct RasterizerInfo
{
	CullMode cullMode{CullMode::Back};
	FaceOrientation faceOrientation{FaceOrientation::CounterClockwise};
};

struct GraphicsPipelineCreateInfo
{
	GPUShaderRef vertexShader;
	GPUShaderRef fragmentShader;
	DepthStencilInfo depthStencil;
	RasterizerInfo rasterizer;
};

using GPUBufferRef = std::shared_ptr<GPUBuffer>;
using GPUResourceRef = std::shared_ptr<GPUResource>;

class GPUCommandList : public GPUResource
{
public:
	virtual void Begin() = 0;

	virtual void End() = 0;

	virtual void BindPipeline(const GPUGraphicsPipelineRef &pipeline) = 0;

	virtual void BeginRenderPass(const GPURenderTargetRef &renderTarget) = 0;

	virtual void EndRenderPass() = 0;

	virtual void BindVertexBuffer(const GPUBufferRef &buf, int binding) = 0;

	virtual void BindIndexBuffer(const GPUBufferRef &buf) = 0;

	virtual void BindUniformBuffer(const std::string &name, const GPUBufferRef &buffer, int size) = 0;

	virtual void BindTexture(const std::string &name, const GPUTextureRef &texture, const GPUSamplerRef &sampler) = 0;

	virtual void DrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset,
							 uint32_t firstInstance) = 0;

	virtual void SetViewport(float offsetX, float offsetY, float width, float height) = 0;

	virtual void SetScissors(int32_t offsetX, int32_t offsetY, uint32_t width, uint32_t height) = 0;
};

using GPUCommandListRef = std::shared_ptr<GPUCommandList>;

class GPUDriver
{
public:
	virtual ~GPUDriver() = default;

	virtual GPUShaderRef CreateShader(const std::vector<uint32_t> &code, ShaderType type) = 0;

	virtual GPUBufferRef CreateBuffer(size_t size, const BufferInfo &info) = 0;

	virtual GPUGraphicsPipelineRef CreateGraphicsPipeline(const GraphicsPipelineCreateInfo &info) = 0;

	virtual GPUCommandListRef CreateCommandList() = 0;

	virtual void Submit(GPUCommandListRef &commandList) = 0;

	virtual void WaitIdle() = 0;

	virtual void SubmitAndWaitIdle(GPUCommandListRef &commandList) = 0;

	virtual GPURenderTargetRef GetRenderTarget() = 0;

	virtual void EndFrame() = 0;

	virtual GPUTextureRef CreateTexture(uint32_t width, uint32_t height) = 0;

	virtual GPUSamplerRef CreateSampler() = 0;

	virtual void CopyBufferToTexture(const GPUBufferRef &buf, GPUTextureRef &image, uint32_t width, uint32_t height) = 0;
};

using GPUDriverRef = std::shared_ptr<GPUDriver>;
