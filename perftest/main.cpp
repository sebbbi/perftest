#include "window.h"
#include "directx.h"
#include "graphicsUtil.h"
#include "loadConstantsGPU.h"
#include <map>

class BenchTest
{
public:
	BenchTest(DirectXDevice& dx, ID3D11UnorderedAccessView* output) : dx(dx), output(output), testCaseNumber(0)
	{
	}

	void testCase(ID3D11ComputeShader* shader, ID3D11Buffer* cb, ID3D11ShaderResourceView* source, const std::string& name)
	{
		const uint3 workloadThreadCount(1024, 1024, 1);
		const uint3 workloadGroupSize(256, 1, 1);

		QueryHandle query = dx.startPerformanceQuery(testCaseNumber, name);
		dx.dispatch(shader, workloadThreadCount, workloadGroupSize, { cb }, { source }, { output }, {});
		dx.endPerformanceQuery(query);

		testCaseNumber++;
	}

private:
	DirectXDevice& dx;
	ID3D11UnorderedAccessView* output;
	unsigned testCaseNumber;
};


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

	const unsigned numWarmUpFramesBeforeBenchmark = 30;
	const unsigned numBenchmarkFrames = 30;
	const unsigned maxTestCases = 200;

	printf("\nRunning %d warm-up frames and %d benchmark frames:\n", numWarmUpFramesBeforeBenchmark, numBenchmarkFrames);

	struct TestCaseTiming
	{
		std::string name;
		float totalTime;
	};

	std::array<TestCaseTiming, maxTestCases> timingResults;

	// Frame loop
	MessageStatus status = MessageStatus::Default;
	unsigned frameNumber = 0;
	do
	{
		dx.processPerformanceResults([&](float timeMillis, unsigned id, std::string& name)
		{
			if (frameNumber >= numWarmUpFramesBeforeBenchmark)
			{
				if (timingResults[id].name == "")
				{
					timingResults[id] = { name, 0 };
				}
				timingResults[id].totalTime += timeMillis;
			}
		});

		BenchTest bench(dx, outputUAV);

		bench.testCase(shaderLoadTyped1dInvariant, loadCB, typedSRV_R8, "Buffer<R8>.Load uniform");
		bench.testCase(shaderLoadTyped1dLinear, loadCB, typedSRV_R8, "Buffer<R8>.Load linear");
		bench.testCase(shaderLoadTyped1dRandom, loadCB, typedSRV_R8, "Buffer<R8>.Load random");
		bench.testCase(shaderLoadTyped2dInvariant, loadCB, typedSRV_RG8, "Buffer<RG8>.Load uniform");
		bench.testCase(shaderLoadTyped2dLinear, loadCB, typedSRV_RG8, "Buffer<RG8>.Load linear");
		bench.testCase(shaderLoadTyped2dRandom, loadCB, typedSRV_RG8, "Buffer<RG8>.Load random");
		bench.testCase(shaderLoadTyped4dInvariant, loadCB, typedSRV_RGBA8, "Buffer<RGBA8>.Load uniform");
		bench.testCase(shaderLoadTyped4dLinear, loadCB, typedSRV_RGBA8, "Buffer<RGBA8>.Load linear");
		bench.testCase(shaderLoadTyped4dRandom, loadCB, typedSRV_RGBA8, "Buffer<RGBA8>.Load random");

		bench.testCase(shaderLoadTyped1dInvariant, loadCB, typedSRV_R16F, "Buffer<R16f>.Load uniform");
		bench.testCase(shaderLoadTyped1dLinear, loadCB, typedSRV_R16F, "Buffer<R16f>.Load linear");
		bench.testCase(shaderLoadTyped1dRandom, loadCB, typedSRV_R16F, "Buffer<R16f>.Load random");
		bench.testCase(shaderLoadTyped2dInvariant, loadCB, typedSRV_RG16F, "Buffer<RG16f>.Load uniform");
		bench.testCase(shaderLoadTyped2dLinear, loadCB, typedSRV_RG16F, "Buffer<RG16f>.Load linear");
		bench.testCase(shaderLoadTyped2dRandom, loadCB, typedSRV_RG16F, "Buffer<RG16f>.Load random");
		bench.testCase(shaderLoadTyped4dInvariant, loadCB, typedSRV_RGBA16F, "Buffer<RGBA16f>.Load uniform");
		bench.testCase(shaderLoadTyped4dLinear, loadCB, typedSRV_RGBA16F, "Buffer<RGBA16f>.Load linear");
		bench.testCase(shaderLoadTyped4dRandom, loadCB, typedSRV_RGBA16F, "Buffer<RGBA16f>.Load random");

		bench.testCase(shaderLoadTyped1dInvariant, loadCB, typedSRV_R32F, "Buffer<R32f>.Load uniform");
		bench.testCase(shaderLoadTyped1dLinear, loadCB, typedSRV_R32F, "Buffer<R32f>.Load linear");
		bench.testCase(shaderLoadTyped1dRandom, loadCB, typedSRV_R32F, "Buffer<R32f>.Load random");
		bench.testCase(shaderLoadTyped2dInvariant, loadCB, typedSRV_RG32F, "Buffer<RG32f>.Load uniform");
		bench.testCase(shaderLoadTyped2dLinear, loadCB, typedSRV_RG32F, "Buffer<RG32f>.Load linear");
		bench.testCase(shaderLoadTyped2dRandom, loadCB, typedSRV_RG32F, "Buffer<RG32f>.Load random");
		bench.testCase(shaderLoadTyped4dInvariant, loadCB, typedSRV_RGBA32F, "Buffer<RGBA32f>.Load uniform");
		bench.testCase(shaderLoadTyped4dLinear, loadCB, typedSRV_RGBA32F, "Buffer<RGBA32f>.Load linear");
		bench.testCase(shaderLoadTyped4dRandom, loadCB, typedSRV_RGBA32F, "Buffer<RGBA32f>.Load random");

		bench.testCase(shaderLoadRaw1dInvariant, loadCB, byteAddressSRV, "ByteAddressBuffer.Load uniform");
		bench.testCase(shaderLoadRaw1dLinear, loadCB, byteAddressSRV, "ByteAddressBuffer.Load linear");
		bench.testCase(shaderLoadRaw1dRandom, loadCB, byteAddressSRV, "ByteAddressBuffer.Load random");
		bench.testCase(shaderLoadRaw2dInvariant, loadCB, byteAddressSRV, "ByteAddressBuffer.Load2 uniform");
		bench.testCase(shaderLoadRaw2dLinear, loadCB, byteAddressSRV, "ByteAddressBuffer.Load2 linear");
		bench.testCase(shaderLoadRaw2dRandom, loadCB, byteAddressSRV, "ByteAddressBuffer.Load2 random");
		bench.testCase(shaderLoadRaw3dInvariant, loadCB, byteAddressSRV, "ByteAddressBuffer.Load3 uniform");
		bench.testCase(shaderLoadRaw3dLinear, loadCB, byteAddressSRV, "ByteAddressBuffer.Load3 linear");
		bench.testCase(shaderLoadRaw3dRandom, loadCB, byteAddressSRV, "ByteAddressBuffer.Load3 random");
		bench.testCase(shaderLoadRaw4dInvariant, loadCB, byteAddressSRV, "ByteAddressBuffer.Load4 uniform");
		bench.testCase(shaderLoadRaw4dLinear, loadCB, byteAddressSRV, "ByteAddressBuffer.Load4 linear");
		bench.testCase(shaderLoadRaw4dRandom, loadCB, byteAddressSRV, "ByteAddressBuffer.Load4 random");

		bench.testCase(shaderLoadRaw2dInvariant, loadCBUnaligned, byteAddressSRV, "ByteAddressBuffer.Load2 unaligned uniform");
		bench.testCase(shaderLoadRaw2dLinear, loadCBUnaligned, byteAddressSRV, "ByteAddressBuffer.Load2 unaligned linear");
		bench.testCase(shaderLoadRaw2dRandom, loadCBUnaligned, byteAddressSRV, "ByteAddressBuffer.Load2 unaligned random");
		bench.testCase(shaderLoadRaw4dInvariant, loadCBUnaligned, byteAddressSRV, "ByteAddressBuffer.Load4 unaligned uniform");
		bench.testCase(shaderLoadRaw4dLinear, loadCBUnaligned, byteAddressSRV, "ByteAddressBuffer.Load4 unaligned linear");
		bench.testCase(shaderLoadRaw4dRandom, loadCBUnaligned, byteAddressSRV, "ByteAddressBuffer.Load4 unaligned random");

		bench.testCase(shaderLoadStructured1dInvariant, loadCB, structuredSRV_R32F, "StructuredBuffer<float>.Load uniform");
		bench.testCase(shaderLoadStructured1dLinear, loadCB, structuredSRV_R32F, "StructuredBuffer<float>.Load linear");
		bench.testCase(shaderLoadStructured1dRandom, loadCB, structuredSRV_R32F, "StructuredBuffer<float>.Load random");
		bench.testCase(shaderLoadStructured2dInvariant, loadCB, structuredSRV_RG32F, "StructuredBuffer<float2>.Load uniform");
		bench.testCase(shaderLoadStructured2dLinear, loadCB, structuredSRV_RG32F, "StructuredBuffer<float2>.Load linear");
		bench.testCase(shaderLoadStructured2dRandom, loadCB, structuredSRV_RG32F, "StructuredBuffer<float2>.Load random");
		bench.testCase(shaderLoadStructured4dInvariant, loadCB, structuredSRV_RGBA32F, "StructuredBuffer<float4>.Load uniform");
		bench.testCase(shaderLoadStructured4dLinear, loadCB, structuredSRV_RGBA32F, "StructuredBuffer<float4>.Load linear");
		bench.testCase(shaderLoadStructured4dRandom, loadCB, structuredSRV_RGBA32F, "StructuredBuffer<float4>.Load random");

		bench.testCase(shaderLoadConstant4dInvariant, loadWithArrayCB, nullptr, "cbuffer{float4} load uniform");
		bench.testCase(shaderLoadConstant4dLinear, loadWithArrayCB, nullptr, "cbuffer{float4} load linear");
		bench.testCase(shaderLoadConstant4dRandom, loadWithArrayCB, nullptr, "cbuffer{float4} load random");

		bench.testCase(shaderLoadTex1dInvariant, loadCB, texSRV_R8, "Texture2D<R8>.Load uniform");
		bench.testCase(shaderLoadTex1dLinear, loadCB, texSRV_R8, "Texture2D<R8>.Load linear");
		bench.testCase(shaderLoadTex1dRandom, loadCB, texSRV_R8, "Texture2D<R8>.Load random");
		bench.testCase(shaderLoadTex2dInvariant, loadCB, texSRV_RG8, "Texture2D<RG8>.Load uniform");
		bench.testCase(shaderLoadTex2dLinear, loadCB, texSRV_RG8, "Texture2D<RG8>.Load linear");
		bench.testCase(shaderLoadTex2dRandom, loadCB, texSRV_RG8, "Texture2D<RG8>.Load random");
		bench.testCase(shaderLoadTex4dInvariant, loadCB, texSRV_RGBA8, "Texture2D<RGBA8>.Load uniform");
		bench.testCase(shaderLoadTex4dLinear, loadCB, texSRV_RGBA8, "Texture2D<RGBA8>.Load linear");
		bench.testCase(shaderLoadTex4dRandom, loadCB, texSRV_RGBA8, "Texture2D<RGBA8>.Load random");

		bench.testCase(shaderLoadTex1dInvariant, loadCB, texSRV_R16F, "Texture2D<R16F>.Load uniform");
		bench.testCase(shaderLoadTex1dLinear, loadCB, texSRV_R16F, "Texture2D<R16F>.Load linear");
		bench.testCase(shaderLoadTex1dRandom, loadCB, texSRV_R16F, "Texture2D<R16F>.Load random");
		bench.testCase(shaderLoadTex2dInvariant, loadCB, texSRV_RG16F, "Texture2D<RG16F>.Load uniform");
		bench.testCase(shaderLoadTex2dLinear, loadCB, texSRV_RG16F, "Texture2D<RG16F>.Load linear");
		bench.testCase(shaderLoadTex2dRandom, loadCB, texSRV_RG16F, "Texture2D<RG16F>.Load random");
		bench.testCase(shaderLoadTex4dInvariant, loadCB, texSRV_RGBA16F, "Texture2D<RGBA16F>.Load uniform");
		bench.testCase(shaderLoadTex4dLinear, loadCB, texSRV_RGBA16F, "Texture2D<RGBA16F>.Load linear");
		bench.testCase(shaderLoadTex4dRandom, loadCB, texSRV_RGBA16F, "Texture2D<RGBA16F>.Load random");

		bench.testCase(shaderLoadTex1dInvariant, loadCB, texSRV_R32F, "Texture2D<R32F>.Load uniform");
		bench.testCase(shaderLoadTex1dLinear, loadCB, texSRV_R32F, "Texture2D<R32F>.Load linear");
		bench.testCase(shaderLoadTex1dRandom, loadCB, texSRV_R32F, "Texture2D<R32F>.Load random");
		bench.testCase(shaderLoadTex2dInvariant, loadCB, texSRV_RG32F, "Texture2D<RG32F>.Load uniform");
		bench.testCase(shaderLoadTex2dLinear, loadCB, texSRV_RG32F, "Texture2D<RG32F>.Load linear");
		bench.testCase(shaderLoadTex2dRandom, loadCB, texSRV_RG32F, "Texture2D<RG32F>.Load random");
		bench.testCase(shaderLoadTex4dInvariant, loadCB, texSRV_RGBA32F, "Texture2D<RGBA32F>.Load uniform");
		bench.testCase(shaderLoadTex4dLinear, loadCB, texSRV_RGBA32F, "Texture2D<RGBA32F>.Load linear");
		bench.testCase(shaderLoadTex4dRandom, loadCB, texSRV_RGBA32F, "Texture2D<RGBA32F>.Load random");

		dx.presentFrame();

		status = messagePump();

		frameNumber++;
		if (frameNumber < numWarmUpFramesBeforeBenchmark)
		{
			printf(".");
		}
		else
		{
			printf("X");
		}
	}
	while (status != MessageStatus::Exit && frameNumber < numBenchmarkFrames + numWarmUpFramesBeforeBenchmark);

	// Find comparison case
	float compareToTime = 1.0f;
	std::string compareToCase = "Buffer<RGBA8>.Load random";
	printf("\n\nPerformance compared to %s\n\n", compareToCase.c_str());
	for (auto&& row : timingResults)
	{
		if (row.name == compareToCase)
		{
			compareToTime = row.totalTime;
			break;
		}
	}

	// Print results
	for (auto&& row : timingResults)
	{
		if (row.name == "") break;
		printf("%s: %.3fms %.3fx\n", row.name.c_str(), row.totalTime, compareToTime / row.totalTime);
	}

	return 0;
} 