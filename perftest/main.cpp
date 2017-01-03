#include "window.hpp"
#include "directx.hpp"
#include "graphicsUtil.h"
#include "loadConstantsGPU.h"

int main()
{
	const uint3 workloadThreadCount(1024, 1024, 1);
	const uint3 workloadGroupSize(256, 1, 1);

	// Init systems
	uint2 resolution(1280, 720);
	HWND window = createWindow(resolution);
	DirectXDevice dx(window, resolution);

	// Load shaders
	com_ptr<ID3D11ComputeShader> shaderLoad1dLinear = loadComputeShader(dx, "shaders/load1dLinear.cso");
	com_ptr<ID3D11ComputeShader> shaderLoad1dRandom = loadComputeShader(dx, "shaders/load1dRandom.cso");

	// Create buffers and output UAV
	com_ptr<ID3D11Buffer> bufferOutput = dx.createBuffer(1024, 32, DirectXDevice::BufferType::ByteAddress);
	com_ptr<ID3D11Buffer> bufferInput = dx.createBuffer(1024, 32, DirectXDevice::BufferType::ByteAddress);
	com_ptr<ID3D11UnorderedAccessView> outputUAV = dx.createTypedUAV(bufferOutput, 1024 * 8, DXGI_FORMAT_R32_FLOAT);

	// SRVs for benchmarking different formats and view types
	com_ptr<ID3D11ShaderResourceView> typedSRV_RGBA8 = dx.createTypedSRV(bufferInput, 1024, DXGI_FORMAT_R8G8B8A8_UNORM);
	com_ptr<ID3D11ShaderResourceView> typedSRV_RGBA16F = dx.createTypedSRV(bufferInput, 1024, DXGI_FORMAT_R16G16B16A16_FLOAT);
	com_ptr<ID3D11ShaderResourceView> typedSRV_RGBA32F = dx.createTypedSRV(bufferInput, 1024, DXGI_FORMAT_R32G32B32A32_FLOAT);
	com_ptr<ID3D11ShaderResourceView> typedSRV_R8 = dx.createTypedSRV(bufferInput, 1024, DXGI_FORMAT_R8_UNORM);
	com_ptr<ID3D11ShaderResourceView> typedSRV_R16F = dx.createTypedSRV(bufferInput, 1024, DXGI_FORMAT_R16_FLOAT);
	com_ptr<ID3D11ShaderResourceView> typedSRV_R32F = dx.createTypedSRV(bufferInput, 1024, DXGI_FORMAT_R32_FLOAT);
	com_ptr<ID3D11ShaderResourceView> byteAddressSRV = dx.createByteAddressSRV(bufferInput, 1024);

	// Setup constant buffer
	LoadConstants loadConstants;
	loadConstants.elementsMask = 1024 - 1;
	loadConstants.writeIndex = 0xffffffff;	// Never write
	com_ptr<ID3D11Buffer> loadCB = dx.createConstantBuffer(sizeof(LoadConstants));
	dx.updateConstantBuffer(loadCB, loadConstants);

	// Frame loop
	MessageStatus status = MessageStatus::Default;
	do
	{
		dx.processPerformanceResults([](float timeMillis, std::string& name)
		{
			printf("%s: %.3fms\n", name.c_str(), timeMillis);
		});

		QueryHandle q1dlinr8 = dx.startPerformanceQuery("Load 1d linear R8");
		dx.dispatch(shaderLoad1dLinear, workloadThreadCount, workloadGroupSize, { loadCB }, { typedSRV_R8 }, { outputUAV }, {});
		dx.endPerformanceQuery(q1dlinr8);

		QueryHandle q1drandr8 = dx.startPerformanceQuery("Load 1d random R8");
		dx.dispatch(shaderLoad1dRandom, workloadThreadCount, workloadGroupSize, { loadCB }, { typedSRV_R8 }, { outputUAV }, {});
		dx.endPerformanceQuery(q1drandr8);

		QueryHandle q4dlinr8 = dx.startPerformanceQuery("Load 4d linear R8");
		dx.dispatch(shaderLoad1dLinear, workloadThreadCount, workloadGroupSize, { loadCB }, { typedSRV_RGBA8 }, { outputUAV }, {});
		dx.endPerformanceQuery(q4dlinr8);

		QueryHandle q4drandr8 = dx.startPerformanceQuery("Load 4d random R8");
		dx.dispatch(shaderLoad1dRandom, workloadThreadCount, workloadGroupSize, { loadCB }, { typedSRV_RGBA8 }, { outputUAV }, {});
		dx.endPerformanceQuery(q4drandr8);

		QueryHandle q1dlinr16f = dx.startPerformanceQuery("Load 1d linear R16F");
		dx.dispatch(shaderLoad1dLinear, workloadThreadCount, workloadGroupSize, { loadCB }, { typedSRV_R16F }, { outputUAV }, { });
		dx.endPerformanceQuery(q1dlinr16f);
		
		QueryHandle q1drandr16f = dx.startPerformanceQuery("Load 1d random R16F");
		dx.dispatch(shaderLoad1dRandom, workloadThreadCount, workloadGroupSize, { loadCB }, { typedSRV_R16F }, { outputUAV }, { });
		dx.endPerformanceQuery(q1drandr16f);

		QueryHandle q4dlinr16f = dx.startPerformanceQuery("Load 4d linear R16F");
		dx.dispatch(shaderLoad1dLinear, workloadThreadCount, workloadGroupSize, { loadCB }, { typedSRV_RGBA16F }, { outputUAV }, {});
		dx.endPerformanceQuery(q4dlinr16f);

		QueryHandle q4drandr16f = dx.startPerformanceQuery("Load 4d random R16F");
		dx.dispatch(shaderLoad1dRandom, workloadThreadCount, workloadGroupSize, { loadCB }, { typedSRV_RGBA16F }, { outputUAV }, {});
		dx.endPerformanceQuery(q4drandr16f);

		dx.presentFrame();
		status = messagePump();
	}
	while (status != MessageStatus::Exit);

	return 0;
}