#include "window.h"
#include "directx.h"
#include "graphicsUtil.h"
#include "loadConstantsGPU.h"

void benchTest(DirectXDevice& dx, ID3D11ComputeShader* shader, ID3D11Buffer* cb, ID3D11UnorderedAccessView* output, ID3D11ShaderResourceView* source, const std::string& name)
{
	const uint3 workloadThreadCount(1024, 1024, 1);
	const uint3 workloadGroupSize(256, 1, 1);

	QueryHandle query = dx.startPerformanceQuery(name);
	dx.dispatch(shader, workloadThreadCount, workloadGroupSize, { cb }, { source }, { output }, {});
	dx.endPerformanceQuery(query);
}

int main()
{
	// Init systems
	uint2 resolution(1280, 720);
	HWND window = createWindow(resolution);
	DirectXDevice dx(window, resolution);

	// Load shaders
	com_ptr<ID3D11ComputeShader> shaderLoad1dLinear = loadComputeShader(dx, "shaders/load1dLinear.cso");
	com_ptr<ID3D11ComputeShader> shaderLoad1dRandom = loadComputeShader(dx, "shaders/load1dRandom.cso");
	com_ptr<ID3D11ComputeShader> shaderLoad2dLinear = loadComputeShader(dx, "shaders/load2dLinear.cso");
	com_ptr<ID3D11ComputeShader> shaderLoad2dRandom = loadComputeShader(dx, "shaders/load2dRandom.cso");
	com_ptr<ID3D11ComputeShader> shaderLoad4dLinear = loadComputeShader(dx, "shaders/load4dLinear.cso");
	com_ptr<ID3D11ComputeShader> shaderLoad4dRandom = loadComputeShader(dx, "shaders/load4dRandom.cso");

	// Create buffers and output UAV
	com_ptr<ID3D11Buffer> bufferOutput = dx.createBuffer(1024, 32, DirectXDevice::BufferType::ByteAddress);
	com_ptr<ID3D11Buffer> bufferInput = dx.createBuffer(1024, 32, DirectXDevice::BufferType::ByteAddress);
	com_ptr<ID3D11UnorderedAccessView> outputUAV = dx.createTypedUAV(bufferOutput, 1024 * 8, DXGI_FORMAT_R32_FLOAT);

	// SRVs for benchmarking different view formats and types
	com_ptr<ID3D11ShaderResourceView> typedSRV_R8 = dx.createTypedSRV(bufferInput, 1024, DXGI_FORMAT_R8_UNORM);
	com_ptr<ID3D11ShaderResourceView> typedSRV_R16F = dx.createTypedSRV(bufferInput, 1024, DXGI_FORMAT_R16_FLOAT);
	com_ptr<ID3D11ShaderResourceView> typedSRV_R32F = dx.createTypedSRV(bufferInput, 1024, DXGI_FORMAT_R32_FLOAT);
	com_ptr<ID3D11ShaderResourceView> typedSRV_RG8 = dx.createTypedSRV(bufferInput, 1024, DXGI_FORMAT_R8G8_UNORM);
	com_ptr<ID3D11ShaderResourceView> typedSRV_RG16F = dx.createTypedSRV(bufferInput, 1024, DXGI_FORMAT_R16G16_FLOAT);
	com_ptr<ID3D11ShaderResourceView> typedSRV_RG32F = dx.createTypedSRV(bufferInput, 1024, DXGI_FORMAT_R32G32_FLOAT);
	com_ptr<ID3D11ShaderResourceView> typedSRV_RGBA8 = dx.createTypedSRV(bufferInput, 1024, DXGI_FORMAT_R8G8B8A8_UNORM);
	com_ptr<ID3D11ShaderResourceView> typedSRV_RGBA16F = dx.createTypedSRV(bufferInput, 1024, DXGI_FORMAT_R16G16B16A16_FLOAT);
	com_ptr<ID3D11ShaderResourceView> typedSRV_RGBA32F = dx.createTypedSRV(bufferInput, 1024, DXGI_FORMAT_R32G32B32A32_FLOAT);
	com_ptr<ID3D11ShaderResourceView> byteAddressSRV = dx.createByteAddressSRV(bufferInput, 1024);

	// Setup the constant buffer
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

		benchTest(dx, shaderLoad1dLinear, loadCB, outputUAV, typedSRV_R8, "Load R8 linear");
		benchTest(dx, shaderLoad1dRandom, loadCB, outputUAV, typedSRV_R8, "Load R8 random");
		benchTest(dx, shaderLoad2dLinear, loadCB, outputUAV, typedSRV_RG8, "Load RG8 linear");
		benchTest(dx, shaderLoad2dRandom, loadCB, outputUAV, typedSRV_RG8, "Load RG8 random");
		benchTest(dx, shaderLoad4dLinear, loadCB, outputUAV, typedSRV_RGBA8, "Load RGBA8 linear");
		benchTest(dx, shaderLoad4dRandom, loadCB, outputUAV, typedSRV_RGBA8, "Load RGBA8 random");

		benchTest(dx, shaderLoad1dLinear, loadCB, outputUAV, typedSRV_R16F, "Load R16f linear");
		benchTest(dx, shaderLoad1dRandom, loadCB, outputUAV, typedSRV_R16F, "Load R16f random");
		benchTest(dx, shaderLoad2dLinear, loadCB, outputUAV, typedSRV_RG16F, "Load RG16f linear");
		benchTest(dx, shaderLoad2dRandom, loadCB, outputUAV, typedSRV_RG16F, "Load RG16f random");
		benchTest(dx, shaderLoad4dLinear, loadCB, outputUAV, typedSRV_RGBA16F, "Load RGBA16f linear");
		benchTest(dx, shaderLoad4dRandom, loadCB, outputUAV, typedSRV_RGBA16F, "Load RGBA16f random");

		benchTest(dx, shaderLoad1dLinear, loadCB, outputUAV, typedSRV_R32F, "Load R32f linear");
		benchTest(dx, shaderLoad1dRandom, loadCB, outputUAV, typedSRV_R32F, "Load R32f random");
		benchTest(dx, shaderLoad2dLinear, loadCB, outputUAV, typedSRV_RG32F, "Load RG32f linear");
		benchTest(dx, shaderLoad2dRandom, loadCB, outputUAV, typedSRV_RG32F, "Load RG32f random");
		benchTest(dx, shaderLoad4dLinear, loadCB, outputUAV, typedSRV_RGBA32F, "Load RGBA32f linear");
		benchTest(dx, shaderLoad4dRandom, loadCB, outputUAV, typedSRV_RGBA32F, "Load RGBA32f random");

		dx.presentFrame();
		status = messagePump();
	}
	while (status != MessageStatus::Exit);

	return 0;
}