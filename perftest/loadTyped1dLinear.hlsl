#define LOAD_WIDTH 1
#define LOAD_LINEAR
Buffer<float> sourceData : register(t0);
#include "loadTypedBody.hlsli"