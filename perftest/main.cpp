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
	com_ptr<ID3D11ComputeShader> shaderLoadTyped1dInvariant = loadComputeShader(dx, "shaders/loadTyped1dInvariant.cso");
	com_ptr<ID3D11ComputeShader> shaderLoadTyped1dLinear = loadComputeShader(dx, "shaders/loadTyped1dLinear.cso");
	com_ptr<ID3D11ComputeShader> shaderLoadTyped1dRandom = loadComputeShader(dx, "shaders/loadTyped1dRandom.cso");
	com_ptr<ID3D11ComputeShader> shaderLoadTyped2dInvariant = loadComputeShader(dx, "shaders/loadTyped2dInvariant.cso");
	com_ptr<ID3D11ComputeShader> shaderLoadTyped2dLinear = loadComputeShader(dx, "shaders/loadTyped2dLinear.cso");
	com_ptr<ID3D11ComputeShader> shaderLoadTyped2dRandom = loadComputeShader(dx, "shaders/loadTyped2dRandom.cso");
	com_ptr<ID3D11ComputeShader> shaderLoadTyped4dInvariant = loadComputeShader(dx, "shaders/loadTyped4dInvariant.cso");
	com_ptr<ID3D11ComputeShader> shaderLoadTyped4dLinear = loadComputeShader(dx, "shaders/loadTyped4dLinear.cso");
	com_ptr<ID3D11ComputeShader> shaderLoadTyped4dRandom = loadComputeShader(dx, "shaders/loadTyped4dRandom.cso");
	com_ptr<ID3D11ComputeShader> shaderLoadRaw1dInvariant = loadComputeShader(dx, "shaders/loadRaw1dInvariant.cso");
	com_ptr<ID3D11ComputeShader> shaderLoadRaw1dLinear = loadComputeShader(dx, "shaders/loadRaw1dLinear.cso");
	com_ptr<ID3D11ComputeShader> shaderLoadRaw1dRandom = loadComputeShader(dx, "shaders/loadRaw1dRandom.cso");
	com_ptr<ID3D11ComputeShader> shaderLoadRaw2dInvariant = loadComputeShader(dx, "shaders/loadRaw2dInvariant.cso");
	com_ptr<ID3D11ComputeShader> shaderLoadRaw2dLinear = loadComputeShader(dx, "shaders/loadRaw2dLinear.cso");
	com_ptr<ID3D11ComputeShader> shaderLoadRaw2dRandom = loadComputeShader(dx, "shaders/loadRaw2dRandom.cso");
	com_ptr<ID3D11ComputeShader> shaderLoadRaw4dInvariant = loadComputeShader(dx, "shaders/loadRaw4dInvariant.cso");
	com_ptr<ID3D11ComputeShader> shaderLoadRaw4dLinear = loadComputeShader(dx, "shaders/loadRaw4dLinear.cso");
	com_ptr<ID3D11ComputeShader> shaderLoadRaw4dRandom = loadComputeShader(dx, "shaders/loadRaw4dRandom.cso");

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
	com_ptr<ID3D11Buffer> loadCB = dx.createConstantBuffer(sizeof(LoadConstants));
	com_ptr<ID3D11Buffer> loadCBUnaligned = dx.createConstantBuffer(sizeof(LoadConstants));
	loadConstants.elementsMask = 1024 - 1;
	loadConstants.writeIndex = 0xffffffff;	// Never write
	loadConstants.readStartAddress = 0;		// Aligned
	dx.updateConstantBuffer(loadCB, loadConstants);
	loadConstants.readStartAddress = 4;		// Unaligned
	dx.updateConstantBuffer(loadCBUnaligned, loadConstants);

	// Frame loop
	MessageStatus status = MessageStatus::Default;
	do
	{
		dx.processPerformanceResults([](float timeMillis, std::string& name)
		{
			printf("%s: %.3fms\n", name.c_str(), timeMillis);
		});

		benchTest(dx, shaderLoadTyped1dInvariant, loadCB, outputUAV, typedSRV_R8, "Load R8 invariant");
		benchTest(dx, shaderLoadTyped1dLinear, loadCB, outputUAV, typedSRV_R8, "Load R8 linear");
		benchTest(dx, shaderLoadTyped1dRandom, loadCB, outputUAV, typedSRV_R8, "Load R8 random");
		benchTest(dx, shaderLoadTyped2dLinear, loadCB, outputUAV, typedSRV_RG8, "Load RG8 linear");
		benchTest(dx, shaderLoadTyped2dLinear, loadCB, outputUAV, typedSRV_RG8, "Load RG8 linear");
		benchTest(dx, shaderLoadTyped2dRandom, loadCB, outputUAV, typedSRV_RG8, "Load RG8 random");
		benchTest(dx, shaderLoadTyped4dLinear, loadCB, outputUAV, typedSRV_RGBA8, "Load RGBA8 linear");
		benchTest(dx, shaderLoadTyped4dLinear, loadCB, outputUAV, typedSRV_RGBA8, "Load RGBA8 linear");
		benchTest(dx, shaderLoadTyped4dRandom, loadCB, outputUAV, typedSRV_RGBA8, "Load RGBA8 random");

		benchTest(dx, shaderLoadTyped1dInvariant, loadCB, outputUAV, typedSRV_R16F, "Load R16f invariant");
		benchTest(dx, shaderLoadTyped1dLinear, loadCB, outputUAV, typedSRV_R16F, "Load R16f linear");
		benchTest(dx, shaderLoadTyped1dRandom, loadCB, outputUAV, typedSRV_R16F, "Load R16f random");
		benchTest(dx, shaderLoadTyped2dInvariant, loadCB, outputUAV, typedSRV_RG16F, "Load RG16f invariant");
		benchTest(dx, shaderLoadTyped2dLinear, loadCB, outputUAV, typedSRV_RG16F, "Load RG16f linear");
		benchTest(dx, shaderLoadTyped2dRandom, loadCB, outputUAV, typedSRV_RG16F, "Load RG16f random");
		benchTest(dx, shaderLoadTyped4dInvariant, loadCB, outputUAV, typedSRV_RGBA16F, "Load RGBA16f invariant");
		benchTest(dx, shaderLoadTyped4dLinear, loadCB, outputUAV, typedSRV_RGBA16F, "Load RGBA16f linear");
		benchTest(dx, shaderLoadTyped4dRandom, loadCB, outputUAV, typedSRV_RGBA16F, "Load RGBA16f random");

		benchTest(dx, shaderLoadTyped1dInvariant, loadCB, outputUAV, typedSRV_R32F, "Load R32f invariant");
		benchTest(dx, shaderLoadTyped1dLinear, loadCB, outputUAV, typedSRV_R32F, "Load R32f linear");
		benchTest(dx, shaderLoadTyped1dRandom, loadCB, outputUAV, typedSRV_R32F, "Load R32f random");
		benchTest(dx, shaderLoadTyped2dInvariant, loadCB, outputUAV, typedSRV_RG32F, "Load RG32f invariant");
		benchTest(dx, shaderLoadTyped2dLinear, loadCB, outputUAV, typedSRV_RG32F, "Load RG32f linear");
		benchTest(dx, shaderLoadTyped2dRandom, loadCB, outputUAV, typedSRV_RG32F, "Load RG32f random");
		benchTest(dx, shaderLoadTyped4dInvariant, loadCB, outputUAV, typedSRV_RGBA32F, "Load RGBA32f invariant");
		benchTest(dx, shaderLoadTyped4dLinear, loadCB, outputUAV, typedSRV_RGBA32F, "Load RGBA32f linear");
		benchTest(dx, shaderLoadTyped4dRandom, loadCB, outputUAV, typedSRV_RGBA32F, "Load RGBA32f random");

		benchTest(dx, shaderLoadRaw1dInvariant, loadCB, outputUAV, byteAddressSRV, "Load1 raw32 aligned invariant");
		benchTest(dx, shaderLoadRaw1dLinear, loadCB, outputUAV, byteAddressSRV, "Load1 raw32 aligned linear");
		benchTest(dx, shaderLoadRaw1dRandom, loadCB, outputUAV, byteAddressSRV, "Load1 raw32 aligned random");
		benchTest(dx, shaderLoadRaw2dInvariant, loadCB, outputUAV, byteAddressSRV, "Load2 raw32 aligned invariant");
		benchTest(dx, shaderLoadRaw2dLinear, loadCB, outputUAV, byteAddressSRV, "Load2 raw32 aligned linear");
		benchTest(dx, shaderLoadRaw2dRandom, loadCB, outputUAV, byteAddressSRV, "Load2 raw32 aligned random");
		benchTest(dx, shaderLoadRaw4dInvariant, loadCB, outputUAV, byteAddressSRV, "Load4 raw32 aligned invariant");
		benchTest(dx, shaderLoadRaw4dLinear, loadCB, outputUAV, byteAddressSRV, "Load4 raw32 aligned linear");
		benchTest(dx, shaderLoadRaw4dRandom, loadCB, outputUAV, byteAddressSRV, "Load4 raw32 aligned random");

		benchTest(dx, shaderLoadRaw1dInvariant, loadCBUnaligned, outputUAV, byteAddressSRV, "Load1 raw32 unaligned invariant");
		benchTest(dx, shaderLoadRaw1dLinear, loadCBUnaligned, outputUAV, byteAddressSRV, "Load1 raw32 unaligned linear");
		benchTest(dx, shaderLoadRaw1dRandom, loadCBUnaligned, outputUAV, byteAddressSRV, "Load1 raw32 unaligned random");
		benchTest(dx, shaderLoadRaw2dInvariant, loadCBUnaligned, outputUAV, byteAddressSRV, "Load2 raw32 unaligned invariant");
		benchTest(dx, shaderLoadRaw2dLinear, loadCBUnaligned, outputUAV, byteAddressSRV, "Load2 raw32 unaligned linear");
		benchTest(dx, shaderLoadRaw2dRandom, loadCBUnaligned, outputUAV, byteAddressSRV, "Load2 raw32 unaligned random");
		benchTest(dx, shaderLoadRaw4dInvariant, loadCBUnaligned, outputUAV, byteAddressSRV, "Load4 raw32 unaligned invariant");
		benchTest(dx, shaderLoadRaw4dLinear, loadCBUnaligned, outputUAV, byteAddressSRV, "Load4 raw32 unaligned linear");
		benchTest(dx, shaderLoadRaw4dRandom, loadCBUnaligned, outputUAV, byteAddressSRV, "Load4 raw32 unaligned random");

		dx.presentFrame();
		status = messagePump();
	}
	while (status != MessageStatus::Exit);

	return 0;
}