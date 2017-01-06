#define LOAD_WIDTH 4
#define LOAD_RANDOM
Texture2D<float4> sourceData : register(t0);
#include "loadTexBody.hlsli"