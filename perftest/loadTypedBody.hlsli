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
	float4 value = 0.0;
	
#if defined(LOAD_INVARIANT)
    // All threads load from same address. Index is wave invariant.
	uint htid = 0;
#elif defined(LOAD_LINEAR)
	// Linearly increasing starting address to allow memory coalescing
	uint htid = gix;
#elif defined(LOAD_RANDOM)
    // Randomize start address offset (0-15) to prevent memory coalescing
	uint htid = (hash1(gix) & 0xf);
#endif

	[loop]
	for (int i = 0; i < 256; ++i)
	{
		// Mask with runtime constant to prevent unwanted compiler optimizations
		uint elemIdx = (htid + i) | loadConstants.elementsMask;

#if LOAD_WIDTH == 1
		value += sourceData[elemIdx].xxxx;
#elif LOAD_WIDTH == 2
		value += sourceData[elemIdx].xyxy;
#elif LOAD_WIDTH == 4
		value += sourceData[elemIdx].xyzw;
#endif
	}

    // Linear write to LDS (no bank conflicts). Significantly faster than memory loads.
	dummyLDS[gix] = value.x + value.y + value.z + value.w;

	GroupMemoryBarrierWithGroupSync();

	// This branch is never taken, but the compiler doesn't know it
	// Optimizer would remove all the memory loads if the data wouldn't be potentially used
	[branch]
	if (loadConstants.writeIndex != 0xffffffff)
	{
        output[tid.x + tid.y] = dummyLDS[loadConstants.writeIndex];
    }
}
