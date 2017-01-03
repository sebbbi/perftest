#include "hash.hlsli"
#include "loadConstantsGPU.h"

Buffer<float> sourceData : register(t0);
RWBuffer<float> output : register(u0);

cbuffer CB0 : register(b0)
{
	LoadConstants loadConstants;
};

#define THREAD_GROUP_SIZE 256

groupshared float dummyLDS[THREAD_GROUP_SIZE];

[numthreads(THREAD_GROUP_SIZE, 1, 1)]
void main(uint3 tid : SV_DispatchThreadID, uint gix : SV_GroupIndex)
{
	float value = 0.0;
	
	for (int i = 0; i < 256; ++i)
		value += sourceData[(tid.x + i) & loadConstants.elementsMask];

	dummyLDS[gix] = value;

	GroupMemoryBarrierWithGroupSync();

	// This branch is never taken, but the compiler doesn't know it
	// Optimizer would remove all the memory loads if the data wouldn't be potentially used
	[branch]
	if (loadConstants.writeIndex != 0xffffffff)
	{
		output[tid.x] = dummyLDS[loadConstants.writeIndex];
	}
}
