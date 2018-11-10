#define LOAD_WIDTH 4
#define LOAD_LINEAR
StructuredBuffer<float4> sourceData : register(t0);
#include "loadStructuredBody.hlsli"
