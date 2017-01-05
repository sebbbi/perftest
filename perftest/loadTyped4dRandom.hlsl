#define LOAD_WIDTH 4
#define LOAD_RANDOM
Buffer<float4> sourceData : register(t0);
#include "loadTypedBody.hlsli"
