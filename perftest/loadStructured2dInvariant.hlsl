#define LOAD_WIDTH 2
#define LOAD_INVARIANT
StructuredBuffer<float2> sourceData : register(t0);
#include "loadStructuredBody.hlsli"
