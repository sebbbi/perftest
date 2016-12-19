Buffer<float> sourceData : register(t0);
RWBuffer<float> output : register(u0);

[numthreads(256, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
	output[DTid.x] = sourceData[DTid.x];
}
