#pragma once
#include "datatypes.h"
#include "com_ptr.h"
#include <windows.h>
#include <d3d11.h>
#include <vector>
#include <array>
#include <functional>
#include <string>

struct QueryHandle
{
	unsigned queryIndex;
};

struct PerformanceQuery
{
	com_ptr<ID3D11Query> disjoint;
	com_ptr<ID3D11Query> start;
	com_ptr<ID3D11Query> end;

	unsigned id;
	std::string name;
};

std::vector<com_ptr<IDXGIAdapter>> enumerateAdapters();

class DirectXDevice
{
public:

	enum class BufferType
	{
		Default,
		Structured,
		ByteAddress
	};

	enum class SamplerType
	{
		Nearest,
		Bilinear,
		Trilinear
	};

	DirectXDevice(HWND window, uint2 resolution, IDXGIAdapter* adapter = nullptr);

	// Create resources
	ID3D11UnorderedAccessView* createBackBufferUAV();
	ID3D11DepthStencilView* createDepthStencilView(uint2 size);
	ID3D11RenderTargetView* DirectXDevice::createBackBufferRTV();
	ID3D11ComputeShader* createComputeShader(const std::vector<unsigned char>& shaderBytes);

	ID3D11Buffer* createConstantBuffer(unsigned bytes);
	ID3D11Buffer* createBuffer(unsigned numElements, unsigned strideBytes, BufferType type = BufferType::Default);
	ID3D11Texture2D* createTexture2d(uint2 dimensions, DXGI_FORMAT format, unsigned mips);
	ID3D11Texture3D* createTexture3d(uint3 dimensions, DXGI_FORMAT format, unsigned mips);
	ID3D11SamplerState* createSampler(SamplerType type);

	ID3D11UnorderedAccessView* createUAV(ID3D11Resource* resource);
	ID3D11UnorderedAccessView* createTypedUAV(ID3D11Resource* buffer, unsigned numElements, DXGI_FORMAT format);
	ID3D11UnorderedAccessView* createByteAddressUAV(ID3D11Resource* buffer, unsigned numElements);

	ID3D11ShaderResourceView* createSRV(ID3D11Resource* buffer);
	ID3D11ShaderResourceView* createTypedSRV(ID3D11Resource* buffer, unsigned numElements, DXGI_FORMAT format);
	ID3D11ShaderResourceView* createStructuredSRV(ID3D11Resource* buffer, unsigned numElements, unsigned stride);
	ID3D11ShaderResourceView* createByteAddressSRV(ID3D11Resource* buffer, unsigned numElements);

	// Data update
	template <typename T>
	void updateConstantBuffer(ID3D11Buffer* cbuffer, const T& cb)
	{
		D3D11_MAPPED_SUBRESOURCE map;
		deviceContext->Map(cbuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
		memcpy(map.pData, &cb, sizeof(cb));
		deviceContext->Unmap(cbuffer, 0);
	}

	// Commands
	void clear(ID3D11RenderTargetView* rtv, const float4& color);
	void clearDepth(ID3D11DepthStencilView *depthStencilView);
	void setRenderTargets(std::initializer_list<ID3D11RenderTargetView*> rtvs, ID3D11DepthStencilView* depthStencilView);
	void dispatch(ID3D11ComputeShader* shader, uint3 resolution, uint3 groupSize,
					std::initializer_list<ID3D11Buffer*> cbs,
					std::initializer_list<ID3D11ShaderResourceView*> srvs,
					std::initializer_list<ID3D11UnorderedAccessView*> uavs = {},
					std::initializer_list<ID3D11SamplerState*> samplers = {});
	void presentFrame();
	void clearUAV(ID3D11UnorderedAccessView* uav, std::array<float, 4> color);

	// Performance querys
	QueryHandle startPerformanceQuery(unsigned id, const std::string& name);
	void endPerformanceQuery(QueryHandle queryHandle);
	void processPerformanceResults(const std::function<void(float, unsigned, std::string&)>& functor);

	// Device and window
	HWND getWindowHandle() { return windowHandle; }
	uint2 getResolution() { return resolution; }
	ID3D11Device* getDevice() { return device; }
	ID3D11DeviceContext* getDeviceContext() { return deviceContext; }

private:

	// Window
	HWND windowHandle;
	uint2 resolution;

	// DirectX
	com_ptr<IDXGISwapChain> swapChain;
	com_ptr<ID3D11Device> device;
	com_ptr<ID3D11DeviceContext> deviceContext;

	// Queries
	std::array<PerformanceQuery, 4096> queries;
	unsigned queryCounter = 0;
	unsigned queryProcessCounter = 0;
};
