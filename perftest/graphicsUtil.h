#pragma once
#include "directx.h"
#include "file.h"

inline ID3D11ComputeShader *loadComputeShader(DirectXDevice &dx, const std::string &filename)
{
	auto shaderBlob = loadFile(filename);
	return dx.createComputeShader(shaderBlob);
}

