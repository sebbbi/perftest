#define LOAD_WIDTH 2
#define LOAD_LINEAR
StructuredBuffer<float2> sourceData : register(t0);
#include "loadStructuredBody.hlsli"
