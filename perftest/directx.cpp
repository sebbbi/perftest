#include "directx.hpp"
#include <assert.h>

DirectXDevice::DirectXDevice(HWND window, uint2 resolution) : 
	windowHandle(window),
	resolution(resolution)

{
	UINT flags = D3D11_CREATE_DEVICE_DEBUG;
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

	DXGI_SWAP_CHAIN_DESC swapDesc;
	ZeroMemory(&swapDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapDesc.BufferDesc.Width = resolution.x;
	swapDesc.BufferDesc.Height = resolution.y;
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // DXGI_FORMAT_B8G8R8A8_UNORM;	// BGRA needed for GDI text output
	swapDesc.SampleDesc.Count = 1;
	swapDesc.BufferUsage = DXGI_USAGE_UNORDERED_ACCESS | DXGI_USAGE_RENDER_TARGET_OUTPUT; // RT needed for GDI text output
	swapDesc.BufferCount = 1;
	swapDesc.OutputWindow = window;
	swapDesc.Windowed = true;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapDesc.Flags = 0;// DXGI_SWAP_CHAIN_FLAG_GDI_COMPATIBLE; // TODO: Fullscreen needs: DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH

	HRESULT result = D3D11CreateDeviceAndSwapChain(
		nullptr, // adapter
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr, // software rasterizer
		flags,
		&featureLevel,
		1, // num feature levels
		D3D11_SDK_VERSION, // sdk version
		&swapDesc,
		&swapChain,
		&device,
		nullptr, // selected feature level
		&deviceContext);

	assert(SUCCEEDED(result));

	D3D11_VIEWPORT viewport;
	viewport.Height = (float)resolution.y;
	viewport.Width = (float)resolution.x;
	viewport.MaxDepth = 1.0f;
	viewport.MinDepth = 0.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	deviceContext->RSSetViewports(1, &viewport);

	// Queries
	for (auto &&q : queries)
	{
		D3D11_QUERY_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Query = D3D11_QUERY_TIMESTAMP_DISJOINT;
		device->CreateQuery(&desc, &q.disjoint);
		desc.Query = D3D11_QUERY_TIMESTAMP;
		device->CreateQuery(&desc, &q.start);
		device->CreateQuery(&desc, &q.end);
	}
}

ID3D11UnorderedAccessView* DirectXDevice::createBackBufferUAV()
{
	ID3D11Texture2D* backBuffer = nullptr;
	HRESULT result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
	assert(SUCCEEDED(result));

	ID3D11UnorderedAccessView *view = nullptr;
	result = device->CreateUnorderedAccessView(backBuffer, nullptr, &view);
	assert(SUCCEEDED(result));

	backBuffer->Release();
	return view;
}

ID3D11DepthStencilView* DirectXDevice::createDepthStencilView(uint2 size)
{
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.ArraySize = 1;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	texDesc.CPUAccessFlags = 0;
	texDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;		
	texDesc.Width = (UINT)size.x;
	texDesc.Height = (UINT)size.y;
	texDesc.MipLevels = 1;
	texDesc.MiscFlags = 0;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;

	HRESULT result;

	ID3D11Texture2D* depthStencil = nullptr;
	result = device->CreateTexture2D(&texDesc, NULL, &depthStencil);
	assert(SUCCEEDED(result));

	D3D11_DEPTH_STENCIL_VIEW_DESC depthViewDesc;
	depthViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthViewDesc.Flags = 0;
	depthViewDesc.Texture2D.MipSlice = 0;

	ID3D11DepthStencilView *depthStencilView = nullptr;
	result = device->CreateDepthStencilView(depthStencil, &depthViewDesc, &depthStencilView);
	assert(SUCCEEDED(result));

	depthStencil->Release();
	return depthStencilView;
}

ID3D11RenderTargetView* DirectXDevice::createBackBufferRTV()
{
	ID3D11Texture2D* backBuffer = nullptr;
	HRESULT result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
	assert(SUCCEEDED(result));

	ID3D11RenderTargetView *view = nullptr;
	result = device->CreateRenderTargetView(backBuffer, nullptr, &view);
	assert(SUCCEEDED(result));

	backBuffer->Release();
	return view;
}

ID3D11Buffer* DirectXDevice::createConstantBuffer(unsigned bytes)
{
	D3D11_BUFFER_DESC desc;
	desc.ByteWidth = bytes;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	ID3D11Buffer *buffer = nullptr;
	HRESULT result = device->CreateBuffer(&desc, nullptr, &buffer);
	assert(SUCCEEDED(result));
	return buffer;
}

ID3D11Buffer* DirectXDevice::createBuffer(unsigned numElements, unsigned strideBytes, BufferType type)
{
	D3D11_BUFFER_DESC desc;
	desc.ByteWidth = strideBytes * numElements;
	desc.StructureByteStride = strideBytes;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	if (type == BufferType::Structured) 
		desc.MiscFlags |= D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

	if (type == BufferType::ByteAddress)
		desc.MiscFlags |= D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;

	ID3D11Buffer *buffer = nullptr;
	HRESULT result = device->CreateBuffer(&desc, nullptr, &buffer);
	assert(SUCCEEDED(result));
	return buffer;
}

ID3D11Texture2D* DirectXDevice::createTexture2d(uint2 dimensions, DXGI_FORMAT format, unsigned mips)
{
	D3D11_TEXTURE2D_DESC desc;
	desc.Width = dimensions.x;
	desc.Height = dimensions.y;
	desc.ArraySize = 1;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.MipLevels = mips;
	desc.Format = format;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	ID3D11Texture2D *texture = nullptr;
	HRESULT result = device->CreateTexture2D(&desc, nullptr, &texture);
	assert(SUCCEEDED(result));
	return texture;
}

ID3D11Texture3D* DirectXDevice::createTexture3d(uint3 dimensions, DXGI_FORMAT format, unsigned mips)
{
	D3D11_TEXTURE3D_DESC desc;
	desc.Width = dimensions.x;
	desc.Height = dimensions.y;
	desc.Depth = dimensions.z;
	desc.MipLevels = mips;
	desc.Format = format;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	ID3D11Texture3D *texture = nullptr;
	HRESULT result = device->CreateTexture3D(&desc, nullptr, &texture);
	assert(SUCCEEDED(result));
	return texture;
}

ID3D11UnorderedAccessView* DirectXDevice::createUAV(ID3D11Resource *buffer)
{
	ID3D11UnorderedAccessView *view = nullptr;
	HRESULT result = device->CreateUnorderedAccessView(buffer, nullptr, &view);
	assert(SUCCEEDED(result));
	return view;
}

ID3D11UnorderedAccessView* DirectXDevice::createByteAddressUAV(ID3D11Resource *buffer, unsigned numElements)
{
	D3D11_UNORDERED_ACCESS_VIEW_DESC desc;
	desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	desc.Format = DXGI_FORMAT_R32_TYPELESS;
	desc.Buffer.FirstElement = 0;
	desc.Buffer.NumElements = numElements;
	desc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_RAW;

	ID3D11UnorderedAccessView *view = nullptr;
	HRESULT result = device->CreateUnorderedAccessView(buffer, &desc, &view);
	assert(SUCCEEDED(result));
	return view;
}

ID3D11UnorderedAccessView* DirectXDevice::createTypedUAV(ID3D11Resource *buffer, unsigned numElements, DXGI_FORMAT format)
{
	D3D11_UNORDERED_ACCESS_VIEW_DESC desc;
	desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	desc.Format = format;
	desc.Buffer.FirstElement = 0;
	desc.Buffer.NumElements = numElements;
	desc.Buffer.Flags = 0;

	ID3D11UnorderedAccessView *view = nullptr;
	HRESULT result = device->CreateUnorderedAccessView(buffer, &desc, &view);
	assert(SUCCEEDED(result));
	return view;
}

ID3D11ShaderResourceView* DirectXDevice::createSRV(ID3D11Resource *buffer)
{
	ID3D11ShaderResourceView *view = nullptr;
	HRESULT result = device->CreateShaderResourceView(buffer, nullptr, &view);
	assert(SUCCEEDED(result));
	return view;
}

ID3D11ShaderResourceView* DirectXDevice::createTypedSRV(ID3D11Resource *buffer, unsigned numElements, DXGI_FORMAT format)
{
	D3D11_SHADER_RESOURCE_VIEW_DESC desc;
	desc.ViewDimension = D3D_SRV_DIMENSION_BUFFER;
	desc.Format = format;
	desc.Buffer.FirstElement = 0;
	desc.Buffer.NumElements = numElements;

	ID3D11ShaderResourceView *view = nullptr;
	HRESULT result = device->CreateShaderResourceView(buffer, &desc, &view);
	assert(SUCCEEDED(result));
	return view;
}

ID3D11ShaderResourceView* DirectXDevice::createByteAddressSRV(ID3D11Resource *buffer, unsigned numElements)
{
	D3D11_SHADER_RESOURCE_VIEW_DESC desc;
	desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	desc.Format = DXGI_FORMAT_R32_TYPELESS;
	desc.BufferEx.FirstElement = 0;
	desc.BufferEx.NumElements = numElements;
	desc.BufferEx.Flags = D3D11_BUFFEREX_SRV_FLAG_RAW;

	ID3D11ShaderResourceView *view = nullptr;
	HRESULT result = device->CreateShaderResourceView(buffer, &desc, &view);
	assert(SUCCEEDED(result));
	return view;
}

ID3D11SamplerState* DirectXDevice::createSampler(SamplerType type)
{
	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	desc.MaxLOD = D3D11_FLOAT32_MAX;

	switch (type)
	{
	case SamplerType::Nearest: 
		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		break;

	case SamplerType::Bilinear:
		desc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
		break;

	case SamplerType::Trilinear:
		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		break;
	}

	ID3D11SamplerState *sampler = nullptr;
	HRESULT result = device->CreateSamplerState(&desc, &sampler);
	assert(SUCCEEDED(result));
	return sampler;
}

ID3D11ComputeShader* DirectXDevice::createComputeShader(const std::vector<unsigned char> &shaderBytes)
{
	ID3D11ComputeShader* shader = nullptr;
	HRESULT result = device->CreateComputeShader(shaderBytes.data(), shaderBytes.size(), nullptr, &shader);
	assert(SUCCEEDED(result));
	return shader;
}

void DirectXDevice::dispatch(ID3D11ComputeShader *shader, uint3 resolution, uint3 groupSize,
								std::initializer_list<ID3D11Buffer*> cbs,
								std::initializer_list<ID3D11ShaderResourceView*> srvs,
								std::initializer_list<ID3D11UnorderedAccessView*> uavs,
								std::initializer_list<ID3D11SamplerState*> samplers)
{
	// Set resources
	if(cbs.size())
	{
		ID3D11Buffer* cbarray[D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT];
		int slot = 0;
		for(auto cb : cbs)
			cbarray[slot++] = cb;
		deviceContext->CSSetConstantBuffers(0, static_cast<UINT>(cbs.size()), cbarray);
	}

	if(srvs.size())
	{
		ID3D11ShaderResourceView* srvarray[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT];
		int slot = 0;
		for(auto srv : srvs)
			srvarray[slot++] = srv;
		deviceContext->CSSetShaderResources(0, static_cast<UINT>(srvs.size()), srvarray);
	}

	if(uavs.size())
	{
		ID3D11UnorderedAccessView* uavarray[D3D11_1_UAV_SLOT_COUNT];
		int slot = 0;
		for(auto uav : uavs)
			uavarray[slot++] = uav;
		deviceContext->CSSetUnorderedAccessViews(0, static_cast<UINT>(uavs.size()), uavarray, nullptr);
	}

	if(samplers.size())
	{
		ID3D11SamplerState *samplerarray[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT];
		int slot = 0;
		for(auto sampler : samplers)
			samplerarray[slot++] = sampler;
		deviceContext->CSSetSamplers(0, static_cast<UINT>(samplers.size()), samplerarray);
	}

	// Render
	uint3 groups = divRoundUp(resolution, groupSize);
	deviceContext->CSSetShader(shader, nullptr, 0);
	deviceContext->Dispatch(groups.x, groups.y, groups.z);

	// Remove resources
	if(cbs.size())
	{
		ID3D11Buffer* cbarray[D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT] = { 0 };
		deviceContext->CSSetConstantBuffers(0, static_cast<UINT>(cbs.size()), cbarray);
	}

	if(srvs.size())
	{
		ID3D11ShaderResourceView* srvarray[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { 0 };
		deviceContext->CSSetShaderResources(0, static_cast<UINT>(srvs.size()), srvarray);
	}

	if(uavs.size())
	{
		ID3D11UnorderedAccessView* uavarray[D3D11_1_UAV_SLOT_COUNT] = { 0 };
		deviceContext->CSSetUnorderedAccessViews(0, static_cast<UINT>(uavs.size()), uavarray, nullptr);
	}

	if(samplers.size())
	{
		ID3D11SamplerState *samplerarray[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT] = { 0 };
		deviceContext->CSSetSamplers(0, static_cast<UINT>(samplers.size()), samplerarray);
	}
}

void DirectXDevice::clear(ID3D11RenderTargetView *rtv, const float4 &color)
{
	float clearColor[4] = { color.x, color.y, color.z, color.w };
	deviceContext->ClearRenderTargetView(rtv, clearColor);
}

void DirectXDevice::clearDepth(ID3D11DepthStencilView *depthStencilView)
{
	deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 0.0f, (UINT8)0);
}

void DirectXDevice::setRenderTargets(std::initializer_list<ID3D11RenderTargetView*> rtvs, ID3D11DepthStencilView *depthStencilView)
{
	if (rtvs.size())
	{
		ID3D11RenderTargetView* rtvarray[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];
		int slot = 0;
		for (auto rtv : rtvs)
			rtvarray[slot++] = rtv;
		deviceContext->OMSetRenderTargets(static_cast<UINT>(rtvs.size()), rtvarray, depthStencilView);
	}
}

void DirectXDevice::presentFrame()
{
	const bool vsync = true;
	swapChain->Present(vsync ? 1 : 0, 0);
}

void DirectXDevice::clearUAV(ID3D11UnorderedAccessView* uav, std::array<float, 4> color)
{
	deviceContext->ClearUnorderedAccessViewFloat(uav, color.data());
}

QueryHandle DirectXDevice::startPerformanceQuery(const std::string& name)
{
	PerformanceQuery& query = queries[queryCounter % queries.size()];	
	
	query.name = name;
	deviceContext->Begin(query.disjoint);
	deviceContext->End(query.start);	// NOTE: timestamp queries don't use Begin(), only End()
	
	QueryHandle out {queryCounter};
	queryCounter++;
	return out;
}

void DirectXDevice::endPerformanceQuery(QueryHandle queryHandle)
{
	PerformanceQuery& query = queries[queryHandle.queryIndex % queries.size()];

	deviceContext->End(query.end);	// NOTE: timestamp queries don't use Begin(), only End()
	deviceContext->End(query.disjoint);
}

void DirectXDevice::processPerformanceResults(const std::function<void(float, std::string&)>& functor)
{
	while(true)
	{
		PerformanceQuery& query = queries[queryProcessCounter % queries.size()];

		D3D11_QUERY_DATA_TIMESTAMP_DISJOINT disjoint;
		bool succDisjoint = deviceContext->GetData(query.disjoint, &disjoint, sizeof(disjoint), 0) == S_OK;

		UINT64 start = 0;
		UINT64 end = 0;
		bool succStart = deviceContext->GetData(query.start, &start, sizeof(start), 0) == S_OK;
		bool succEnd = deviceContext->GetData(query.end, &end, sizeof(end), 0) == S_OK;

		// Wait until all queries are ready
		if (!succDisjoint || !succStart || !succEnd)
			break;

		if (!disjoint.Disjoint)
		{
			UINT64 d = end - start;
			float delta = (float(d) / float(disjoint.Frequency)) * 1000.0f;

			// Call functor to process results
			functor(delta, query.name);
		}

		queryProcessCounter++;
	}
}
