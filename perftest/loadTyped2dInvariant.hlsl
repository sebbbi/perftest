#define LOAD_WIDTH 2
#define LOAD_INVARIANT
Buffer<float2> sourceData : register(t0);
#include "loadTypedBody.hlsli"