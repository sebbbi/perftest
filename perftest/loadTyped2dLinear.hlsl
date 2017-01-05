#define LOAD_WIDTH 2
#define LOAD_LINEAR
Buffer<float2> sourceData : register(t0);
#include "loadTypedBody.hlsli"