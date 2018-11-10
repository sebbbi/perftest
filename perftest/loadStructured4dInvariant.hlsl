#define LOAD_WIDTH 4
#define LOAD_INVARIANT
StructuredBuffer<float4> sourceData : register(t0);
#include "loadStructuredBody.hlsli"
