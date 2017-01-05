#define LOAD_WIDTH 4
#define LOAD_INVARIANT
Buffer<float4> sourceData : register(t0);
#include "loadTypedBody.hlsli"