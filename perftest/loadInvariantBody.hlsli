#include "hash.hlsli"
#include "loadConstantsGPU.h"

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
	
    // All threads load from same address. Index is wave invariant.
	uint htid = 0;

	for (int i = 0; i < 256; ++i)
		value += sourceData[(htid + i) & loadConstants.elementsMask];

    // Linear write to LDS (no bank conflicts). Significantly faster than memory loads.
	dummyLDS[gix] = value;

	GroupMemoryBarrierWithGroupSync();

	// This branch is never taken, but the compiler doesn't know it
	// Optimizer would remove all the memory loads if the data wouldn't be potentially used
	[branch]
	if (loadConstants.writeIndex != 0xffffffff)
	{
        output[tid.x + tid.y] = dummyLDS[loadConstants.writeIndex];
    }
}
