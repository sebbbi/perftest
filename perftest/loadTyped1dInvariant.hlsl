#define LOAD_WIDTH 1
#define LOAD_INVARIANT
Buffer<float> sourceData : register(t0);
#include "loadTypedBody.hlsli"