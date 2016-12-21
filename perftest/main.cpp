#include "window.hpp"
#include "directx.hpp"
#include "graphicsUtil.h"
#include "loadConstantsGPU.h"

int main()
{
	const uint3 workloadThreadCount(1024, 1024, 1024);
	const uint3 workloadGroupSize(256, 1, 1);

	// Init systems
	uint2 resolution(1280, 720);
	HWND window = createWindow(resolution);
	DirectXDevice dx(window, resolution);

	// Load shaders
	com_ptr<ID3D11ComputeShader> shaderLoad1dLinear = loadComputeShader(dx, "shaders/load1dLinear.cso");
	com_ptr<ID3D11ComputeShader> shaderLoad1dRandom = loadComputeShader(dx, "shaders/load1dRandom.cso");

	// Create resources
	com_ptr<ID3D11Buffer> buffer1 = dx.createBuffer(1024, 32, DirectXDevice::BufferType::ByteAddress);
	com_ptr<ID3D11Buffer> buffer2 = dx.createBuffer(1024, 32, DirectXDevice::BufferType::ByteAddress);
	com_ptr<ID3D11UnorderedAccessView> typedUAV_RGBA32 = dx.createTypedUAV(buffer1, 1024, DXGI_FORMAT_R32G32B32A32_UINT);
	com_ptr<ID3D11UnorderedAccessView> typedUAV_R32F = dx.createTypedUAV(buffer1, 1024 * 8, DXGI_FORMAT_R32_FLOAT);
	com_ptr<ID3D11UnorderedAccessView> byteAddressUAV = dx.createByteAddressUAV(buffer1, 1024 * 8);
	com_ptr<ID3D11ShaderResourceView> typedSRV_R16F = dx.createTypedSRV(buffer2, 1024, DXGI_FORMAT_R16_FLOAT);

	// Setup constant buffer
	LoadConstants loadConstants;
	loadConstants.numElements = 1024;
	loadConstants.writeIndex = 0xffffffff;	// Never write
	com_ptr<ID3D11Buffer> loadCB = dx.createConstantBuffer(sizeof(LoadConstants));
	dx.updateConstantBuffer(loadCB, loadConstants);

	// Frame loop
	MessageStatus status = MessageStatus::Default;
	do
	{
		dx.dispatch(shaderLoad1dLinear, workloadThreadCount, workloadGroupSize, { loadCB }, { typedSRV_R16F }, { typedUAV_R32F }, { });
		dx.dispatch(shaderLoad1dRandom, workloadThreadCount, workloadGroupSize, { loadCB }, { typedSRV_R16F }, { typedUAV_R32F }, { });
		dx.presentFrame();
		status = messagePump();
	}
	while (status != MessageStatus::Exit);

	return 0;
}