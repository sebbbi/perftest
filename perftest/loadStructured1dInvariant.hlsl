#define LOAD_WIDTH 1
#define LOAD_INVARIANT
StructuredBuffer<float> sourceData : register(t0);
#include "loadStructuredBody.hlsli"
