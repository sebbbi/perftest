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
	dummyLDS[gix] = sourceData[hash1(tid.x) % loadConstants.numElements];

	GroupMemoryBarrierWithGroupSync();

	// This branch is never taken, but the compiler doesn't know it
	// Optimizer will remove the memory loads if the data is not potentially used
	[branch]
	if (loadConstants.writeIndex != 0xffffffff)
	{
		output[tid.x] = dummyLDS[loadConstants.writeIndex];
	}
}
