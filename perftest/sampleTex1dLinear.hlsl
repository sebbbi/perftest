#define LOAD_WIDTH 1
#define LOAD_LINEAR
Texture2D<float> sourceData : register(t0);
SamplerState texSampler : register(s0); 
#include "sampleTexBody.hlsli"