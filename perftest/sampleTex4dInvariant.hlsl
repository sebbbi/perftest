#define LOAD_WIDTH 4
#define LOAD_INVARIANT
Texture2D<float4> sourceData : register(t0);
SamplerState texSampler : register(s0); 
#include "sampleTexBody.hlsli"