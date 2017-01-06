#include "hash.hlsli"
#include "loadConstantsGPU.h"

RWBuffer<float> output : register(u0);

cbuffer CB0 : register(b0)
{
	LoadConstants loadConstants;
};

#define THREAD_GROUP_DIM 16

groupshared float dummyLDS[THREAD_GROUP_DIM][THREAD_GROUP_DIM];

[numthreads(THREAD_GROUP_DIM, THREAD_GROUP_DIM, 1)]
void main(uint3 tid : SV_DispatchThreadID, uint3 gid : SV_GroupThreadID)
{
	float4 value = 0.0;
	
#if defined(LOAD_INVARIANT)
    // All threads load from same address. Index is wave invariant.
	uint2 htid = 0;
#elif defined(LOAD_LINEAR)
	// Linearly increasing starting address.
	uint2 htid = gid;
#elif defined(LOAD_RANDOM)
    // Randomize start address offset (0-15)
	uint2 htid = uint2((hash1(gid.x) & 0xf), (hash1(gid.y) & 0xf));
#endif

	[loop]
	for (int y = 0; y < 16; ++y)
	{
		[loop]
		for (int x = 0; x < 16; ++x)
		{
			// Mask with runtime constant to prevent unwanted compiler optimizations
			uint2 elemIdx = (htid + uint2(x, y)) | loadConstants.elementsMask;

#if LOAD_WIDTH == 1
			value += sourceData[elemIdx].xxxx;
#elif LOAD_WIDTH == 2
			value += sourceData[elemIdx].xyxy;
#elif LOAD_WIDTH == 4
			value += sourceData[elemIdx].xyzw;
#endif
		}
	}
    // Linear write to LDS (no bank conflicts). Significantly faster than memory loads.
	dummyLDS[gid.y][gid.x] = value.x + value.y + value.z + value.w;

	GroupMemoryBarrierWithGroupSync();

	// This branch is never taken, but the compiler doesn't know it
	// Optimizer would remove all the memory loads if the data wouldn't be potentially used
	[branch]
	if (loadConstants.writeIndex != 0xffffffff)
	{
        output[tid.x + tid.y] = dummyLDS[(loadConstants.writeIndex >> 8) & 0xff][loadConstants.writeIndex & 0xff];
    }
}
