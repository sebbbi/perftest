#define LOAD_WIDTH 2
#define LOAD_RANDOM
Texture2D<float2> sourceData : register(t0);
#include "loadTexBody.hlsli"