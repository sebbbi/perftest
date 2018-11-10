#define LOAD_WIDTH 2
#define LOAD_RANDOM
StructuredBuffer<float2> sourceData : register(t0);
#include "loadStructuredBody.hlsli"
