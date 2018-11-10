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

int main(int argc, char *argv[])
{
	// Enumerate adapters
	std::vector<com_ptr<IDXGIAdapter>> adapters = enumerateAdapters();
	printf("PerfTest\nTo select adapter, use: PerfTest.exe [ADAPTER_INDEX]\n\n");
	printf("Adapters found:\n");
	int index = 0;
	for (auto&& adapter : adapters)
	{
		DXGI_ADAPTER_DESC desc;
		adapter->GetDesc(&desc);
		printf("%d: %S\n", index++, desc.Description);
	}

	// Command line index can be used to select adapter
	int selectedAdapterIdx = 0;
	if (argc == 2)
	{
		selectedAdapterIdx = std::stoi(argv[1]);
		selectedAdapterIdx = min(max(0, selectedAdapterIdx), (int)adapters.size() - 1);
	}
	printf("Using adapter %d\n", selectedAdapterIdx);

	// Init systems
	uint2 resolution(256, 256);
	HWND window = createWindow(resolution);
	DirectXDevice dx(window, resolution, adapters[selectedAdapterIdx]);

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
	com_ptr<ID3D11ComputeShader> shaderLoadRaw3dInvariant = loadComputeShader(dx, "shaders/loadRaw3dInvariant.cso");
	com_ptr<ID3D11ComputeShader> shaderLoadRaw3dLinear = loadComputeShader(dx, "shaders/loadRaw3dLinear.cso");
	com_ptr<ID3D11ComputeShader> shaderLoadRaw3dRandom = loadComputeShader(dx, "shaders/loadRaw3dRandom.cso");
	com_ptr<ID3D11ComputeShader> shaderLoadRaw4dInvariant = loadComputeShader(dx, "shaders/loadRaw4dInvariant.cso");
	com_ptr<ID3D11ComputeShader> shaderLoadRaw4dLinear = loadComputeShader(dx, "shaders/loadRaw4dLinear.cso");
	com_ptr<ID3D11ComputeShader> shaderLoadRaw4dRandom = loadComputeShader(dx, "shaders/loadRaw4dRandom.cso");

	com_ptr<ID3D11ComputeShader> shaderLoadTex1dInvariant = loadComputeShader(dx, "shaders/loadTex1dInvariant.cso");
	com_ptr<ID3D11ComputeShader> shaderLoadTex1dLinear = loadComputeShader(dx, "shaders/loadTex1dLinear.cso");
	com_ptr<ID3D11ComputeShader> shaderLoadTex1dRandom = loadComputeShader(dx, "shaders/loadTex1dRandom.cso");
	com_ptr<ID3D11ComputeShader> shaderLoadTex2dInvariant = loadComputeShader(dx, "shaders/loadTex2dInvariant.cso");
	com_ptr<ID3D11ComputeShader> shaderLoadTex2dLinear = loadComputeShader(dx, "shaders/loadTex2dLinear.cso");
	com_ptr<ID3D11ComputeShader> shaderLoadTex2dRandom = loadComputeShader(dx, "shaders/loadTex2dRandom.cso");
	com_ptr<ID3D11ComputeShader> shaderLoadTex4dInvariant = loadComputeShader(dx, "shaders/loadTex4dInvariant.cso");
	com_ptr<ID3D11ComputeShader> shaderLoadTex4dLinear = loadComputeShader(dx, "shaders/loadTex4dLinear.cso");
	com_ptr<ID3D11ComputeShader> shaderLoadTex4dRandom = loadComputeShader(dx, "shaders/loadTex4dRandom.cso");

	com_ptr<ID3D11ComputeShader> shaderLoadConstant4dInvariant = loadComputeShader(dx, "shaders/loadConstant4dInvariant.cso");
	com_ptr<ID3D11ComputeShader> shaderLoadConstant4dLinear = loadComputeShader(dx, "shaders/loadConstant4dLinear.cso");
	com_ptr<ID3D11ComputeShader> shaderLoadConstant4dRandom = loadComputeShader(dx, "shaders/loadConstant4dRandom.cso");

	com_ptr<ID3D11ComputeShader> shaderLoadStructured1dInvariant = loadComputeShader(dx, "shaders/loadStructured1dInvariant.cso");
	com_ptr<ID3D11ComputeShader> shaderLoadStructured1dLinear = loadComputeShader(dx, "shaders/loadStructured1dLinear.cso");
	com_ptr<ID3D11ComputeShader> shaderLoadStructured1dRandom = loadComputeShader(dx, "shaders/loadStructured1dRandom.cso");
	com_ptr<ID3D11ComputeShader> shaderLoadStructured2dInvariant = loadComputeShader(dx, "shaders/loadStructured2dInvariant.cso");
	com_ptr<ID3D11ComputeShader> shaderLoadStructured2dLinear = loadComputeShader(dx, "shaders/loadStructured2dLinear.cso");
	com_ptr<ID3D11ComputeShader> shaderLoadStructured2dRandom = loadComputeShader(dx, "shaders/loadStructured2dRandom.cso");
	com_ptr<ID3D11ComputeShader> shaderLoadStructured4dInvariant = loadComputeShader(dx, "shaders/loadStructured4dInvariant.cso");
	com_ptr<ID3D11ComputeShader> shaderLoadStructured4dLinear = loadComputeShader(dx, "shaders/loadStructured4dLinear.cso");
	com_ptr<ID3D11ComputeShader> shaderLoadStructured4dRandom = loadComputeShader(dx, "shaders/loadStructured4dRandom.cso");

	// Create buffers and output UAV
	com_ptr<ID3D11Buffer> bufferOutput = dx.createBuffer(2048, 4, DirectXDevice::BufferType::ByteAddress);
	com_ptr<ID3D11Buffer> bufferInput = dx.createBuffer(1024, 16, DirectXDevice::BufferType::ByteAddress);
	com_ptr<ID3D11Buffer> bufferInputStructured4 = dx.createBuffer(1024, 4, DirectXDevice::BufferType::Structured);
	com_ptr<ID3D11Buffer> bufferInputStructured8 = dx.createBuffer(1024, 8, DirectXDevice::BufferType::Structured);
	com_ptr<ID3D11Buffer> bufferInputStructured16 = dx.createBuffer(1024, 16, DirectXDevice::BufferType::Structured);
	com_ptr<ID3D11UnorderedAccessView> outputUAV = dx.createTypedUAV(bufferOutput, 2048, DXGI_FORMAT_R32_FLOAT);

	// SRVs for benchmarking different buffer view formats/types
	com_ptr<ID3D11ShaderResourceView> typedSRV_R8 = dx.createTypedSRV(bufferInput, 1024, DXGI_FORMAT_R8_UNORM);
	com_ptr<ID3D11ShaderResourceView> typedSRV_R16F = dx.createTypedSRV(bufferInput, 1024, DXGI_FORMAT_R16_FLOAT);
	com_ptr<ID3D11ShaderResourceView> typedSRV_R32F = dx.createTypedSRV(bufferInput, 1024, DXGI_FORMAT_R32_FLOAT);
	com_ptr<ID3D11ShaderResourceView> typedSRV_RG8 = dx.createTypedSRV(bufferInput, 1024, DXGI_FORMAT_R8G8_UNORM);
	com_ptr<ID3D11ShaderResourceView> typedSRV_RG16F = dx.createTypedSRV(bufferInput, 1024, DXGI_FORMAT_R16G16_FLOAT);
	com_ptr<ID3D11ShaderResourceView> typedSRV_RG32F = dx.createTypedSRV(bufferInput, 1024, DXGI_FORMAT_R32G32_FLOAT);
	com_ptr<ID3D11ShaderResourceView> typedSRV_RGBA8 = dx.createTypedSRV(bufferInput, 1024, DXGI_FORMAT_R8G8B8A8_UNORM);
	com_ptr<ID3D11ShaderResourceView> typedSRV_RGBA16F = dx.createTypedSRV(bufferInput, 1024, DXGI_FORMAT_R16G16B16A16_FLOAT);
	com_ptr<ID3D11ShaderResourceView> typedSRV_RGBA32F = dx.createTypedSRV(bufferInput, 1024, DXGI_FORMAT_R32G32B32A32_FLOAT);
	com_ptr<ID3D11ShaderResourceView> structuredSRV_R32F = dx.createStructuredSRV(bufferInputStructured4, 1024, 4);
	com_ptr<ID3D11ShaderResourceView> structuredSRV_RG32F = dx.createStructuredSRV(bufferInputStructured8, 1024, 8);
	com_ptr<ID3D11ShaderResourceView> structuredSRV_RGBA32F = dx.createStructuredSRV(bufferInputStructured16, 1024, 16);
	com_ptr<ID3D11ShaderResourceView> byteAddressSRV = dx.createByteAddressSRV(bufferInput, 1024);

	// Create input textures
	com_ptr<ID3D11Texture2D> texR8 = dx.createTexture2d(uint2(32, 32), DXGI_FORMAT_R8_UNORM, 1);
	com_ptr<ID3D11Texture2D> texR16F = dx.createTexture2d(uint2(32, 32), DXGI_FORMAT_R16_FLOAT, 1);
	com_ptr<ID3D11Texture2D> texR32F = dx.createTexture2d(uint2(32, 32), DXGI_FORMAT_R32_FLOAT, 1);
	com_ptr<ID3D11Texture2D> texRG8 = dx.createTexture2d(uint2(32, 32), DXGI_FORMAT_R8G8_UNORM, 1);
	com_ptr<ID3D11Texture2D> texRG16F = dx.createTexture2d(uint2(32, 32), DXGI_FORMAT_R16G16_FLOAT, 1);
	com_ptr<ID3D11Texture2D> texRG32F = dx.createTexture2d(uint2(32, 32), DXGI_FORMAT_R32G32_FLOAT, 1);
	com_ptr<ID3D11Texture2D> texRGBA8 = dx.createTexture2d(uint2(32, 32), DXGI_FORMAT_R8G8B8A8_UNORM, 1);
	com_ptr<ID3D11Texture2D> texRGBA16F = dx.createTexture2d(uint2(32, 32), DXGI_FORMAT_R16G16B16A16_FLOAT, 1);
	com_ptr<ID3D11Texture2D> texRGBA32F = dx.createTexture2d(uint2(32, 32), DXGI_FORMAT_R32G32B32A32_FLOAT, 1);

	// Texture SRVs
	com_ptr<ID3D11ShaderResourceView> texSRV_R8 = dx.createSRV(texR8);
	com_ptr<ID3D11ShaderResourceView> texSRV_R16F = dx.createSRV(texR16F);
	com_ptr<ID3D11ShaderResourceView> texSRV_R32F = dx.createSRV(texR32F);
	com_ptr<ID3D11ShaderResourceView> texSRV_RG8 = dx.createSRV(texRG8);
	com_ptr<ID3D11ShaderResourceView> texSRV_RG16F = dx.createSRV(texRG16F);
	com_ptr<ID3D11ShaderResourceView> texSRV_RG32F = dx.createSRV(texRG32F);
	com_ptr<ID3D11ShaderResourceView> texSRV_RGBA8 = dx.createSRV(texRGBA8);
	com_ptr<ID3D11ShaderResourceView> texSRV_RGBA16F = dx.createSRV(texRGBA16F);
	com_ptr<ID3D11ShaderResourceView> texSRV_RGBA32F = dx.createSRV(texRGBA32F);

	// Setup the constant buffer
	LoadConstants loadConstants;
	com_ptr<ID3D11Buffer> loadCB = dx.createConstantBuffer(sizeof(LoadConstants));
	com_ptr<ID3D11Buffer> loadCBUnaligned = dx.createConstantBuffer(sizeof(LoadConstants));
	loadConstants.elementsMask = 0;				// Dummy mask to prevent unwanted compiler optimizations
	loadConstants.writeIndex = 0xffffffff;		// Never write
	loadConstants.readStartAddress = 0;			// Aligned
	dx.updateConstantBuffer(loadCB, loadConstants);
	loadConstants.readStartAddress = 4;			// Unaligned
	dx.updateConstantBuffer(loadCBUnaligned, loadConstants);
	
	// Setup constant buffer with float4 array for constant buffer load benchmarking
	LoadConstantsWithArray loadConstantsWithArray;
	com_ptr<ID3D11Buffer> loadWithArrayCB = dx.createConstantBuffer(sizeof(LoadConstantsWithArray));
	loadConstantsWithArray.elementsMask = 0;				// Dummy mask to prevent unwanted compiler optimizations
	loadConstantsWithArray.writeIndex = 0xffffffff;			// Never write
	loadConstantsWithArray.readStartAddress = 0;			// Aligned
	memset(loadConstantsWithArray.benchmarkArray, 0, sizeof(loadConstantsWithArray.benchmarkArray));
	dx.updateConstantBuffer(loadWithArrayCB, loadConstantsWithArray);

	// Frame loop
	MessageStatus status = MessageStatus::Default;
	do
	{
		dx.processPerformanceResults([](float timeMillis, std::string& name)
		{
			printf("%s: %.3fms\n", name.c_str(), timeMillis);
		});

		benchTest(dx, shaderLoadTyped1dInvariant, loadCB, outputUAV, typedSRV_R8, "Buffer<R8>.Load uniform");
		benchTest(dx, shaderLoadTyped1dLinear, loadCB, outputUAV, typedSRV_R8, "Buffer<R8>.Load linear");
		benchTest(dx, shaderLoadTyped1dRandom, loadCB, outputUAV, typedSRV_R8, "Buffer<R8>.Load random");
		benchTest(dx, shaderLoadTyped2dInvariant, loadCB, outputUAV, typedSRV_RG8, "Buffer<RG8>.Load uniform");
		benchTest(dx, shaderLoadTyped2dLinear, loadCB, outputUAV, typedSRV_RG8, "Buffer<RG8>.Load linear");
		benchTest(dx, shaderLoadTyped2dRandom, loadCB, outputUAV, typedSRV_RG8, "Buffer<RG8>.Load random");
		benchTest(dx, shaderLoadTyped4dInvariant, loadCB, outputUAV, typedSRV_RGBA8, "Buffer<RGBA8>.Load uniform");
		benchTest(dx, shaderLoadTyped4dLinear, loadCB, outputUAV, typedSRV_RGBA8, "Buffer<RGBA8>.Load linear");
		benchTest(dx, shaderLoadTyped4dRandom, loadCB, outputUAV, typedSRV_RGBA8, "Buffer<RGBA8>.Load random");

		benchTest(dx, shaderLoadTyped1dInvariant, loadCB, outputUAV, typedSRV_R16F, "Buffer<R16f>.Load uniform");
		benchTest(dx, shaderLoadTyped1dLinear, loadCB, outputUAV, typedSRV_R16F, "Buffer<R16f>.Load linear");
		benchTest(dx, shaderLoadTyped1dRandom, loadCB, outputUAV, typedSRV_R16F, "Buffer<R16f>.Load random");
		benchTest(dx, shaderLoadTyped2dInvariant, loadCB, outputUAV, typedSRV_RG16F, "Buffer<RG16f>.Load uniform");
		benchTest(dx, shaderLoadTyped2dLinear, loadCB, outputUAV, typedSRV_RG16F, "Buffer<RG16f>.Load linear");
		benchTest(dx, shaderLoadTyped2dRandom, loadCB, outputUAV, typedSRV_RG16F, "Buffer<RG16f>.Load random");
		benchTest(dx, shaderLoadTyped4dInvariant, loadCB, outputUAV, typedSRV_RGBA16F, "Buffer<RGBA16f>.Load uniform");
		benchTest(dx, shaderLoadTyped4dLinear, loadCB, outputUAV, typedSRV_RGBA16F, "Buffer<RGBA16f>.Load linear");
		benchTest(dx, shaderLoadTyped4dRandom, loadCB, outputUAV, typedSRV_RGBA16F, "Buffer<RGBA16f>.Load random");

		benchTest(dx, shaderLoadTyped1dInvariant, loadCB, outputUAV, typedSRV_R32F, "Buffer<R32f>.Load uniform");
		benchTest(dx, shaderLoadTyped1dLinear, loadCB, outputUAV, typedSRV_R32F, "Buffer<R32f>.Load linear");
		benchTest(dx, shaderLoadTyped1dRandom, loadCB, outputUAV, typedSRV_R32F, "Buffer<R32f>.Load random");
		benchTest(dx, shaderLoadTyped2dInvariant, loadCB, outputUAV, typedSRV_RG32F, "Buffer<RG32f>.Load uniform");
		benchTest(dx, shaderLoadTyped2dLinear, loadCB, outputUAV, typedSRV_RG32F, "Buffer<RG32f>.Load linear");
		benchTest(dx, shaderLoadTyped2dRandom, loadCB, outputUAV, typedSRV_RG32F, "Buffer<RG32f>.Load random");
		benchTest(dx, shaderLoadTyped4dInvariant, loadCB, outputUAV, typedSRV_RGBA32F, "Buffer<RGBA32f>.Load uniform");
		benchTest(dx, shaderLoadTyped4dLinear, loadCB, outputUAV, typedSRV_RGBA32F, "Buffer<RGBA32f>.Load linear");
		benchTest(dx, shaderLoadTyped4dRandom, loadCB, outputUAV, typedSRV_RGBA32F, "Buffer<RGBA32f>.Load random");

		benchTest(dx, shaderLoadRaw1dInvariant, loadCB, outputUAV, byteAddressSRV, "ByteAddressBuffer.Load uniform");
		benchTest(dx, shaderLoadRaw1dLinear, loadCB, outputUAV, byteAddressSRV, "ByteAddressBuffer.Load linear");
		benchTest(dx, shaderLoadRaw1dRandom, loadCB, outputUAV, byteAddressSRV, "ByteAddressBuffer.Load random");
		benchTest(dx, shaderLoadRaw2dInvariant, loadCB, outputUAV, byteAddressSRV, "ByteAddressBuffer.Load2 uniform");
		benchTest(dx, shaderLoadRaw2dLinear, loadCB, outputUAV, byteAddressSRV, "ByteAddressBuffer.Load2 linear");
		benchTest(dx, shaderLoadRaw2dRandom, loadCB, outputUAV, byteAddressSRV, "ByteAddressBuffer.Load2 random");
		benchTest(dx, shaderLoadRaw3dInvariant, loadCB, outputUAV, byteAddressSRV, "ByteAddressBuffer.Load3 uniform");
		benchTest(dx, shaderLoadRaw3dLinear, loadCB, outputUAV, byteAddressSRV, "ByteAddressBuffer.Load3 linear");
		benchTest(dx, shaderLoadRaw3dRandom, loadCB, outputUAV, byteAddressSRV, "ByteAddressBuffer.Load3 random");
		benchTest(dx, shaderLoadRaw4dInvariant, loadCB, outputUAV, byteAddressSRV, "ByteAddressBuffer.Load4 uniform");
		benchTest(dx, shaderLoadRaw4dLinear, loadCB, outputUAV, byteAddressSRV, "ByteAddressBuffer.Load4 linear");
		benchTest(dx, shaderLoadRaw4dRandom, loadCB, outputUAV, byteAddressSRV, "ByteAddressBuffer.Load4 random");

		benchTest(dx, shaderLoadRaw2dInvariant, loadCBUnaligned, outputUAV, byteAddressSRV, "ByteAddressBuffer.Load2 unaligned uniform");
		benchTest(dx, shaderLoadRaw2dLinear, loadCBUnaligned, outputUAV, byteAddressSRV, "ByteAddressBuffer.Load2 unaligned linear");
		benchTest(dx, shaderLoadRaw2dRandom, loadCBUnaligned, outputUAV, byteAddressSRV, "ByteAddressBuffer.Load2 unaligned random");
		benchTest(dx, shaderLoadRaw4dInvariant, loadCBUnaligned, outputUAV, byteAddressSRV, "ByteAddressBuffer.Load4 unaligned uniform");
		benchTest(dx, shaderLoadRaw4dLinear, loadCBUnaligned, outputUAV, byteAddressSRV, "ByteAddressBuffer.Load4 unaligned linear");
		benchTest(dx, shaderLoadRaw4dRandom, loadCBUnaligned, outputUAV, byteAddressSRV, "ByteAddressBuffer.Load4 unaligned random");

		benchTest(dx, shaderLoadStructured1dInvariant, loadCB, outputUAV, structuredSRV_R32F, "StructuredBuffer<float>.Load uniform");
		benchTest(dx, shaderLoadStructured1dLinear, loadCB, outputUAV, structuredSRV_R32F, "StructuredBuffer<float>.Load linear");
		benchTest(dx, shaderLoadStructured1dRandom, loadCB, outputUAV, structuredSRV_R32F, "StructuredBuffer<float>.Load random");
		benchTest(dx, shaderLoadStructured2dInvariant, loadCB, outputUAV, structuredSRV_RG32F, "StructuredBuffer<float2>.Load uniform");
		benchTest(dx, shaderLoadStructured2dLinear, loadCB, outputUAV, structuredSRV_RG32F, "StructuredBuffer<float2>.Load linear");
		benchTest(dx, shaderLoadStructured2dRandom, loadCB, outputUAV, structuredSRV_RG32F, "StructuredBuffer<float2>.Load random");
		benchTest(dx, shaderLoadStructured4dInvariant, loadCB, outputUAV, structuredSRV_RGBA32F, "StructuredBuffer<float4>.Load uniform");
		benchTest(dx, shaderLoadStructured4dLinear, loadCB, outputUAV, structuredSRV_RGBA32F, "StructuredBuffer<float4>.Load linear");
		benchTest(dx, shaderLoadStructured4dRandom, loadCB, outputUAV, structuredSRV_RGBA32F, "StructuredBuffer<float4>.Load random");

		benchTest(dx, shaderLoadConstant4dInvariant, loadWithArrayCB, outputUAV, nullptr, "cbuffer{float4} load uniform");
		benchTest(dx, shaderLoadConstant4dLinear, loadWithArrayCB, outputUAV, nullptr, "cbuffer{float4} load linear");
		benchTest(dx, shaderLoadConstant4dRandom, loadWithArrayCB, outputUAV, nullptr, "cbuffer{float4} load random");

		benchTest(dx, shaderLoadTex1dInvariant, loadCB, outputUAV, texSRV_R8, "Texture2D<R8>.Load uniform");
		benchTest(dx, shaderLoadTex1dLinear, loadCB, outputUAV, texSRV_R8, "Texture2D<R8>.Load linear");
		benchTest(dx, shaderLoadTex1dRandom, loadCB, outputUAV, texSRV_R8, "Texture2D<R8>.Load random");
		benchTest(dx, shaderLoadTex2dInvariant, loadCB, outputUAV, texSRV_RG8, "Texture2D<RG8>.Load uniform");
		benchTest(dx, shaderLoadTex2dLinear, loadCB, outputUAV, texSRV_RG8, "Texture2D<RG8>.Load linear");
		benchTest(dx, shaderLoadTex2dRandom, loadCB, outputUAV, texSRV_RG8, "Texture2D<RG8>.Load random");
		benchTest(dx, shaderLoadTex4dInvariant, loadCB, outputUAV, texSRV_RGBA8, "Texture2D<RGBA8>.Load uniform");
		benchTest(dx, shaderLoadTex4dLinear, loadCB, outputUAV, texSRV_RGBA8, "Texture2D<RGBA8>.Load linear");
		benchTest(dx, shaderLoadTex4dRandom, loadCB, outputUAV, texSRV_RGBA8, "Texture2D<RGBA8>.Load random");

		benchTest(dx, shaderLoadTex1dInvariant, loadCB, outputUAV, texSRV_R16F, "Texture2D<R16F>.Load uniform");
		benchTest(dx, shaderLoadTex1dLinear, loadCB, outputUAV, texSRV_R16F, "Texture2D<R16F>.Load linear");
		benchTest(dx, shaderLoadTex1dRandom, loadCB, outputUAV, texSRV_R16F, "Texture2D<R16F>.Load random");
		benchTest(dx, shaderLoadTex2dInvariant, loadCB, outputUAV, texSRV_RG16F, "Texture2D<RG16F>.Load uniform");
		benchTest(dx, shaderLoadTex2dLinear, loadCB, outputUAV, texSRV_RG16F, "Texture2D<RG16F>.Load linear");
		benchTest(dx, shaderLoadTex2dRandom, loadCB, outputUAV, texSRV_RG16F, "Texture2D<RG16F>.Load random");
		benchTest(dx, shaderLoadTex4dInvariant, loadCB, outputUAV, texSRV_RGBA16F, "Texture2D<RGBA16F>.Load uniform");
		benchTest(dx, shaderLoadTex4dLinear, loadCB, outputUAV, texSRV_RGBA16F, "Texture2D<RGBA16F>.Load linear");
		benchTest(dx, shaderLoadTex4dRandom, loadCB, outputUAV, texSRV_RGBA16F, "Texture2D<RGBA16F>.Load random");

		benchTest(dx, shaderLoadTex1dInvariant, loadCB, outputUAV, texSRV_R32F, "Texture2D<R32F>.Load uniform");
		benchTest(dx, shaderLoadTex1dLinear, loadCB, outputUAV, texSRV_R32F, "Texture2D<R32F>.Load linear");
		benchTest(dx, shaderLoadTex1dRandom, loadCB, outputUAV, texSRV_R32F, "Texture2D<R32F>.Load random");
		benchTest(dx, shaderLoadTex2dInvariant, loadCB, outputUAV, texSRV_RG32F, "Texture2D<RG32F>.Load uniform");
		benchTest(dx, shaderLoadTex2dLinear, loadCB, outputUAV, texSRV_RG32F, "Texture2D<RG32F>.Load linear");
		benchTest(dx, shaderLoadTex2dRandom, loadCB, outputUAV, texSRV_RG32F, "Texture2D<RG32F>.Load random");
		benchTest(dx, shaderLoadTex4dInvariant, loadCB, outputUAV, texSRV_RGBA32F, "Texture2D<RGBA32F>.Load uniform");
		benchTest(dx, shaderLoadTex4dLinear, loadCB, outputUAV, texSRV_RGBA32F, "Texture2D<RGBA32F>.Load linear");
		benchTest(dx, shaderLoadTex4dRandom, loadCB, outputUAV, texSRV_RGBA32F, "Texture2D<RGBA32F>.Load random");

		dx.presentFrame();

		status = messagePump();
	}
	while (status != MessageStatus::Exit);

	return 0;
}