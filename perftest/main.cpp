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

	// Create buffers and output UAV
	com_ptr<ID3D11Buffer> bufferOutput = dx.createBuffer(1024, 32, DirectXDevice::BufferType::ByteAddress);
	com_ptr<ID3D11Buffer> bufferInput = dx.createBuffer(1024, 32, DirectXDevice::BufferType::ByteAddress);
	com_ptr<ID3D11UnorderedAccessView> outputUAV = dx.createTypedUAV(bufferOutput, 1024 * 8, DXGI_FORMAT_R32_FLOAT);

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
		benchTest(dx, shaderLoadTyped2dInvariant, loadCB, outputUAV, typedSRV_RG8, "Load RG8 invariant");
		benchTest(dx, shaderLoadTyped2dLinear, loadCB, outputUAV, typedSRV_RG8, "Load RG8 linear");
		benchTest(dx, shaderLoadTyped2dRandom, loadCB, outputUAV, typedSRV_RG8, "Load RG8 random");
		benchTest(dx, shaderLoadTyped4dInvariant, loadCB, outputUAV, typedSRV_RGBA8, "Load RGBA8 invariant");
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

		benchTest(dx, shaderLoadRaw1dInvariant, loadCB, outputUAV, byteAddressSRV, "Load1 raw32 invariant");
		benchTest(dx, shaderLoadRaw1dLinear, loadCB, outputUAV, byteAddressSRV, "Load1 raw32 linear");
		benchTest(dx, shaderLoadRaw1dRandom, loadCB, outputUAV, byteAddressSRV, "Load1 raw32 random");
		benchTest(dx, shaderLoadRaw2dInvariant, loadCB, outputUAV, byteAddressSRV, "Load2 raw32 invariant");
		benchTest(dx, shaderLoadRaw2dLinear, loadCB, outputUAV, byteAddressSRV, "Load2 raw32 linear");
		benchTest(dx, shaderLoadRaw2dRandom, loadCB, outputUAV, byteAddressSRV, "Load2 raw32 random");
		benchTest(dx, shaderLoadRaw3dInvariant, loadCB, outputUAV, byteAddressSRV, "Load3 raw32 invariant");
		benchTest(dx, shaderLoadRaw3dLinear, loadCB, outputUAV, byteAddressSRV, "Load3 raw32 linear");
		benchTest(dx, shaderLoadRaw3dRandom, loadCB, outputUAV, byteAddressSRV, "Load3 raw32 random");
		benchTest(dx, shaderLoadRaw4dInvariant, loadCB, outputUAV, byteAddressSRV, "Load4 raw32 invariant");
		benchTest(dx, shaderLoadRaw4dLinear, loadCB, outputUAV, byteAddressSRV, "Load4 raw32 linear");
		benchTest(dx, shaderLoadRaw4dRandom, loadCB, outputUAV, byteAddressSRV, "Load4 raw32 random");

		benchTest(dx, shaderLoadRaw2dInvariant, loadCBUnaligned, outputUAV, byteAddressSRV, "Load2 raw32 unaligned invariant");
		benchTest(dx, shaderLoadRaw2dLinear, loadCBUnaligned, outputUAV, byteAddressSRV, "Load2 raw32 unaligned linear");
		benchTest(dx, shaderLoadRaw2dRandom, loadCBUnaligned, outputUAV, byteAddressSRV, "Load2 raw32 unaligned random");
		benchTest(dx, shaderLoadRaw4dInvariant, loadCBUnaligned, outputUAV, byteAddressSRV, "Load4 raw32 unaligned invariant");
		benchTest(dx, shaderLoadRaw4dLinear, loadCBUnaligned, outputUAV, byteAddressSRV, "Load4 raw32 unaligned linear");
		benchTest(dx, shaderLoadRaw4dRandom, loadCBUnaligned, outputUAV, byteAddressSRV, "Load4 raw32 unaligned random");

		benchTest(dx, shaderLoadTex1dInvariant, loadCB, outputUAV, texSRV_R8, "Tex2D load R8 invariant");
		benchTest(dx, shaderLoadTex1dLinear, loadCB, outputUAV, texSRV_R8, "Tex2D load R8 linear");
		benchTest(dx, shaderLoadTex1dRandom, loadCB, outputUAV, texSRV_R8, "Tex2D load R8 random");
		benchTest(dx, shaderLoadTex2dInvariant, loadCB, outputUAV, texSRV_RG8, "Tex2D load RG8 invariant");
		benchTest(dx, shaderLoadTex2dLinear, loadCB, outputUAV, texSRV_RG8, "Tex2D load RG8 linear");
		benchTest(dx, shaderLoadTex2dRandom, loadCB, outputUAV, texSRV_RG8, "Tex2D load RG8 random");
		benchTest(dx, shaderLoadTex4dInvariant, loadCB, outputUAV, texSRV_RGBA8, "Tex2D load RGBA8 invariant");
		benchTest(dx, shaderLoadTex4dLinear, loadCB, outputUAV, texSRV_RGBA8, "Tex2D load RGBA8 linear");
		benchTest(dx, shaderLoadTex4dRandom, loadCB, outputUAV, texSRV_RGBA8, "Tex2D load RGBA8 random");

		benchTest(dx, shaderLoadTex1dInvariant, loadCB, outputUAV, texSRV_R16F, "Tex2D load R16F invariant");
		benchTest(dx, shaderLoadTex1dLinear, loadCB, outputUAV, texSRV_R16F, "Tex2D load R16F linear");
		benchTest(dx, shaderLoadTex1dRandom, loadCB, outputUAV, texSRV_R16F, "Tex2D load R16F random");
		benchTest(dx, shaderLoadTex2dInvariant, loadCB, outputUAV, texSRV_RG16F, "Tex2D load RG16F invariant");
		benchTest(dx, shaderLoadTex2dLinear, loadCB, outputUAV, texSRV_RG16F, "Tex2D load RG16F linear");
		benchTest(dx, shaderLoadTex2dRandom, loadCB, outputUAV, texSRV_RG16F, "Tex2D load RG16F random");
		benchTest(dx, shaderLoadTex4dInvariant, loadCB, outputUAV, texSRV_RGBA16F, "Tex2D load RGBA16F invariant");
		benchTest(dx, shaderLoadTex4dLinear, loadCB, outputUAV, texSRV_RGBA16F, "Tex2D load RGBA16F linear");
		benchTest(dx, shaderLoadTex4dRandom, loadCB, outputUAV, texSRV_RGBA16F, "Tex2D load RGBA16F random");

		benchTest(dx, shaderLoadTex1dInvariant, loadCB, outputUAV, texSRV_R32F, "Tex2D load R32F invariant");
		benchTest(dx, shaderLoadTex1dLinear, loadCB, outputUAV, texSRV_R32F, "Tex2D load R32F linear");
		benchTest(dx, shaderLoadTex1dRandom, loadCB, outputUAV, texSRV_R32F, "Tex2D load R32F random");
		benchTest(dx, shaderLoadTex2dInvariant, loadCB, outputUAV, texSRV_RG32F, "Tex2D load RG32F invariant");
		benchTest(dx, shaderLoadTex2dLinear, loadCB, outputUAV, texSRV_RG32F, "Tex2D load RG32F linear");
		benchTest(dx, shaderLoadTex2dRandom, loadCB, outputUAV, texSRV_RG32F, "Tex2D load RG32F random");
		benchTest(dx, shaderLoadTex4dInvariant, loadCB, outputUAV, texSRV_RGBA32F, "Tex2D load RGBA32F invariant");
		benchTest(dx, shaderLoadTex4dLinear, loadCB, outputUAV, texSRV_RGBA32F, "Tex2D load RGBA32F linear");
		benchTest(dx, shaderLoadTex4dRandom, loadCB, outputUAV, texSRV_RGBA32F, "Tex2D load RGBA32F random");

		dx.presentFrame();

		status = messagePump();
	}
	while (status != MessageStatus::Exit);

	return 0;
}