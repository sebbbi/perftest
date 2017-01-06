#define LOAD_WIDTH 1
#define LOAD_INVARIANT
Texture2D<float> sourceData : register(t0);
#include "loadTexBody.hlsli"