#include "window.hpp"
#include "directx.hpp"
#include "graphicsUtil.h"

int main()
{
	const int3 workloadThreadCount(1024, 1024, 1024);
	const int3 workloadGroupSize(256, 1, 1);

	// Init systems
	int2 resolution = int2(1280, 720);
	HWND window = createWindow(resolution);
	DirectXDevice dx(window, resolution);

	// Load shaders
	com_ptr<ID3D11ComputeShader> shaderLoad1d = loadComputeShader(dx, "shaders/load1d.cso");

	// Create resources
	com_ptr<ID3D11Buffer> buffer1 = dx.createBuffer(1024, 32, DirectXDevice::BufferType::ByteAddress);
	com_ptr<ID3D11Buffer> buffer2 = dx.createBuffer(1024, 32, DirectXDevice::BufferType::ByteAddress);
	com_ptr<ID3D11UnorderedAccessView> typedUAV_RGBA32 = dx.createTypedUAV(buffer1.ptr, 1024, DXGI_FORMAT_R32G32B32A32_UINT);
	com_ptr<ID3D11UnorderedAccessView> typedUAV_R32F = dx.createTypedUAV(buffer1.ptr, 1024 * 8, DXGI_FORMAT_R32_FLOAT);
	com_ptr<ID3D11UnorderedAccessView> byteAddressUAV = dx.createByteAddressUAV(buffer1.ptr, 1024 * 8);
	com_ptr<ID3D11ShaderResourceView> typedSRV_R16F = dx.createTypedSRV(buffer2.ptr, 1024, DXGI_FORMAT_R16_FLOAT);

	// Frame loop
	MessageStatus status = MessageStatus::Default;
	do
	{
		dx.dispatch(shaderLoad1d.ptr, workloadThreadCount, workloadGroupSize, {}, {typedSRV_R16F.ptr}, {typedUAV_R32F.ptr}, {});
		dx.presentFrame();
		status = messagePump();
	}
	while (status != MessageStatus::Exit);

	return 0;
}