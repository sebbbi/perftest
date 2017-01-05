# PerfTest

A simple GPU shader memory operation performance test tool. Current implementation is DirectX 11.0 based.

## Features

Designed to measure performance of various types of buffer and image loads. This application is not a GPU memory bandwidth benchmark tool. All tests operate inside GPUs L1 caches (no larger than 16 KB working sets). 

- Coalesced loads (100% L1 cache hit)
- Random loads (100% L1 cache hit)
- Invariant loads (same address for all threads)
- Typed SRVs: 1/2/4 channels, 8/16/32 bits per channel
- 32 bit byte address SRVs (load, load2, load4)

## Explanations

**Coalesced loads:**
GPUs optimize linear address patterns. Coalescing occurs when all threads in a warp/wave (32/64 threads) load from contiguous (or identical) addresses. In my "linear" test case, memory loads access contiguous addresses in the whole thread group (256 threads). This should coalesce perfectly on all GPUs, independent of warp/wave width.

**Random loads:**
I add a random start offset of 0-15 elements for each thread (still aligned). This prevents GPU coalescing, and provides more realistic view of performance for common case (non-linear) memory accessing. This benchmark is as cache efficient as the previous. All data still comes from the L1 cache.

**Invariant loads:**
All threads in group load simultaneously from the same address. This triggers coalesced path on some GPUs and additonal optimizations on some GPUs, such as scalar loads (SGPR storage) on AMD GCN.

## Todo list

- Better output (elements/s or bytes/s, etc)
- Constant buffer loads (both constant address and indexed)
- Texture loads (1d/2d/3d)
- Texture sampling (1d/2d/3d)
- Extended format support (uint/unorm/float of all widths, R10G10B10, R11G11B10f)
- Measure write performance
- Port to Vulkan and/or DX12 (upload heap load performance, etc)

## Results

**TODO:** Add comprehensive AMD, NV & Intel results here. Add some graphs. Add percentage comparison and/or ops per cycle.

**AMD Radeon 7970 GE (GCN1)**
```markdown
Load R8 invariant: 0.541ms
Load R8 linear: 0.539ms
Load R8 random: 2.121ms
Load RG8 linear: 2.386ms
Load RG8 linear: 2.386ms
Load RG8 random: 2.386ms
Load RGBA8 linear: 2.121ms
Load RGBA8 linear: 2.121ms
Load RGBA8 random: 2.121ms
Load R16f invariant: 0.536ms
Load R16f linear: 0.537ms
Load R16f random: 2.121ms
Load RG16f invariant: 2.386ms
Load RG16f linear: 2.385ms
Load RG16f random: 2.385ms
Load RGBA16f invariant: 2.122ms
Load RGBA16f linear: 2.121ms
Load RGBA16f random: 2.121ms
Load R32f invariant: 0.536ms
Load R32f linear: 0.537ms
Load R32f random: 2.121ms
Load RG32f invariant: 2.385ms
Load RG32f linear: 2.385ms
Load RG32f random: 2.385ms
Load RGBA32f invariant: 2.121ms
Load RGBA32f linear: 2.121ms
Load RGBA32f random: 2.385ms
Load1 raw32 invariant: 0.607ms
Load1 raw32 linear: 0.679ms
Load1 raw32 random: 2.122ms
Load2 raw32 invariant: 0.615ms
Load2 raw32 linear: 2.386ms
Load2 raw32 random: 2.386ms
Load4 raw32 invariant: 1.078ms
Load4 raw32 linear: 2.128ms
Load4 raw32 random: 2.371ms
```

**Typed loads:** GCN1 coalesces 1d typed loads only (all formats). Coalesced load performance is 4x. Both linear access pattern (all threads in wave load subsequent addresses) and invariant access (all threads in wave load the same address) coalesce perfectly. All dimensions (1d/2d/4d) and channel widths (8b/16b/32b) perform identically. Best bytes per cycle rate can be achieved either by R32 coalesced load (when access pattern suits this) or always with RGBA32 load.

**Raw (ByteAddressBuffer) loads:** Similar to typed loads. 1d formats coalesce perfectly (4x) on linear access. Invariant access generates scalar unit loads on GCN1 (separate cache + stored to SGPR -> reduced register & cache pressure & doesn't stress vector load path). Scalar 1d/2d load seems to match coalesced 1d vector load in performance. 4d invariant case is slightly slower, but still 2x faster than similar width vector loads.

These results match with AMDs wide loads & coalescing documents, see: http://gpuopen.com/gcn-memory-coalescing/. I would be glad if AMD released a public document describing all scalar load optimization cases supported by their compiler.


**NVIDIA GeForce GTX 980 (Maxwell2)**
```markdown
Load R8 invariant: 1.632ms
Load R8 linear: 1.825ms
Load R8 random: 1.629ms
Load RG8 linear: 1.632ms
Load RG8 linear: 1.630ms
Load RG8 random: 1.631ms
Load RGBA8 linear: 1.719ms
Load RGBA8 linear: 1.720ms
Load RGBA8 random: 1.705ms
Load R16f invariant: 1.625ms
Load R16f linear: 1.631ms
Load R16f random: 1.821ms
Load RG16f invariant: 1.634ms
Load RG16f linear: 1.633ms
Load RG16f random: 1.633ms
Load RGBA16f invariant: 1.688ms
Load RGBA16f linear: 1.718ms
Load RGBA16f random: 1.706ms
Load R32f invariant: 1.628ms
Load R32f linear: 1.631ms
Load R32f random: 1.631ms
Load RG32f invariant: 1.634ms
Load RG32f linear: 1.630ms
Load RG32f random: 1.629ms
Load RGBA32f invariant: 3.246ms
Load RGBA32f linear: 3.249ms
Load RGBA32f random: 3.250ms
Load1 raw32 invariant: 1.638ms
Load1 raw32 linear: 1.638ms
Load1 raw32 random: 1.636ms
Load2 raw32 invariant: 3.259ms
Load2 raw32 linear: 3.268ms
Load2 raw32 random: 3.255ms
Load4 raw32 invariant: 6.505ms
Load4 raw32 linear: 7.773ms
Load4 raw32 random: 6.520ms
```

**Typed loads:** Maxwell2 doesn't coalesce any typed loads. Dimensions (1d/2d/4d) and channel widths (8b/16b/32b) don't directly affect performance. All up to 64 bit loads are full rate. 128 bit loads are half rate (only RGBA32). Best bytes per cycle rate can be achieved by 64+ bit loads (RGBA16, RG32, RGBA32).

**Raw (ByteAddressBuffer) loads:** Oddly we see no coalescing here either. CUDA code shows big performance improvement with similar linear access pattern. All 1d raw loads are as fast as typed buffer loads. However NV doesn't seem to emit wide raw loads either. 2d is exactly 2x slower than 1d and 4d is 4x slower. NVIDIA supports 64 bit and 128 wide raw loads, see: https://devblogs.nvidia.com/parallelforall/cuda-pro-tip-increase-performance-with-vectorized-memory-access/. Wide loads in CUDA however require memory alignment. My test case is perfectly aligned. HLSL ByteAddressBuffer.Load4() only requires alignment of 4. In general case it's hard to prove alignment of 16 (in my code there's an explicit multiply address by 16). I need to ask NVIDIA whether their HLSL compiler should emit raw wide loads (and if so, what are the limitations).

NVIDIA's coalescing & wide load documents are all CUDA centric. I coudn't reproduce coalescing or wide load performance gains in HLSL (DirectX). NVIDIA should provide game developers similar excellent (public) low level hardware performance documents and advice as they provide CUDA developers. It's often hard to understand why HLSL compute shader performance doesn't match equal CUDA code.

## License

PerfTest is released under the MIT license. See [LICENSE.md](LICENSE.md) for full text.
