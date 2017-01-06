# PerfTest

A simple GPU shader memory operation performance test tool. Current implementation is DirectX 11.0 based.

## Features

Designed to measure performance of various types of buffer and image loads. This application is not a GPU memory bandwidth benchmark tool. All tests operate inside GPUs L1 caches (no larger than 16 KB working sets). 

- Coalesced loads (100% L1 cache hit)
- Random loads (100% L1 cache hit)
- Invariant loads (same address for all threads)
- Typed SRVs: 1/2/4 channels, 8/16/32 bits per channel
- 32 bit byte address SRVs (load, load2, load3, load4 - aligned and unaligned)

## Explanations

**Coalesced loads:**
GPUs optimize linear address patterns. Coalescing occurs when all threads in a warp/wave (32/64 threads) load from contiguous addresses. In my "linear" test case, memory loads access contiguous addresses in the whole thread group (256 threads). This should coalesce perfectly on all GPUs, independent of warp/wave width.

**Random loads:**
I add a random start offset of 0-15 elements for each thread (still aligned). This prevents GPU coalescing, and provides more realistic view of performance for common case (non-linear) memory accessing. This benchmark is as cache efficient as the previous. All data still comes from the L1 cache.

**Invariant loads:**
All threads in group simultaneously load from the same address. This triggers coalesced path on some GPUs and additonal optimizations on some GPUs, such as scalar loads (SGPR storage) on AMD GCN.

**Notes:**
**Compiler optimizations** can ruin the results. We want to measure only load (read) performance, but write (store) is also needed, otherwise the compiler will just optimize the whole shader away. To avoid this, each thread does first 256 loads followed by a single linear groupshared memory write (no bank-conflicts). Cbuffer contains a write mask (not known at compile time). It controls which elements are written from the groupshared memory to the output buffer. The mask is always zero at runtime. Compilers can also combine multiple narrow raw buffer loads together (as bigger 4d loads) if it an be proven at compile time that loads from the same thread access contiguous offsets. This is prevented by applying an address mask from cbuffer (not known at compile time). 

## Todo list

- Enumerate GPUs (allow select iGPU/dGPU)
- Better output (elements/s or bytes/s, etc)
- Constant buffer loads (both constant address and indexed)
- Texture loads (1d/2d/3d)
- Texture sampling (1d/2d/3d)
- Extended format support (uint/unorm/float of all widths, R10G10B10, R11G11B10f)
- Measure write performance
- Port to Vulkan and/or DX12 (upload heap load performance, etc)

## Results

**TODO:** Add comprehensive AMD, NV & Intel results here. Add some graphs. Add percentage comparison and/or ops per cycle.

### AMD Radeon 7970 GE (GCN1)
```markdown
Load R8 invariant: 0.541ms
Load R8 linear: 0.539ms
Load R8 random: 2.121ms
Load RG8 invariant: 2.386ms
Load RG8 linear: 2.386ms
Load RG8 random: 2.386ms
Load RGBA8 invariant: 2.122ms
Load RGBA8 linear: 2.122ms
Load RGBA8 random: 2.121ms
Load R16f invariant: 0.536ms
Load R16f linear: 0.538ms
Load R16f random: 2.121ms
Load RG16f invariant: 2.385ms
Load RG16f linear: 2.385ms
Load RG16f random: 2.385ms
Load RGBA16f invariant: 2.121ms
Load RGBA16f linear: 2.121ms
Load RGBA16f random: 2.121ms
Load R32f invariant: 0.536ms
Load R32f linear: 0.538ms
Load R32f random: 2.121ms
Load RG32f invariant: 2.385ms
Load RG32f linear: 2.385ms
Load RG32f random: 2.385ms
Load RGBA32f invariant: 2.121ms
Load RGBA32f linear: 2.121ms
Load RGBA32f random: 2.385ms
Load1 raw32 invariant: 0.493ms
Load1 raw32 linear: 0.562ms
Load1 raw32 random: 2.122ms
Load2 raw32 invariant: 0.549ms
Load2 raw32 linear: 2.386ms
Load2 raw32 random: 2.386ms
Load3 raw32 invariant: 0.812ms
Load3 raw32 linear: 2.122ms
Load3 raw32 random: 4.239ms
Load4 raw32 invariant: 1.082ms
Load4 raw32 linear: 2.139ms
Load4 raw32 random: 2.371ms
Load2 raw32 unaligned invariant: 0.548ms
Load2 raw32 unaligned linear: 2.385ms
Load2 raw32 unaligned random: 2.385ms
Load4 raw32 unaligned invariant: 1.076ms
Load4 raw32 unaligned linear: 2.124ms
Load4 raw32 unaligned random: 2.622ms
```

**Typed loads:** GCN1 coalesces 1d typed loads only (all formats). Coalesced load performance is 4x. Both linear access pattern (all threads in wave load subsequent addresses) and invariant access (all threads in wave load the same address) coalesce perfectly. All dimensions (1d/2d/4d) and channel widths (8b/16b/32b) perform identically. Best bytes per cycle rate can be achieved either by R32 coalesced load (when access pattern suits this) or always with RGBA32 load.

**Raw (ByteAddressBuffer) loads:** Similar to typed loads. 1d raw loads coalesce perfectly (4x) on linear access. Invariant raw loads generates scalar unit loads on GCN (separate cache + stored to SGPR -> reduced register & cache pressure & doesn't stress vector load path). Scalar 1d load is 4x faster than random 1d load (similar to coalesced). Scalar 2d load is 4x faster than normal 2d load. Scalar 4d load is 2x faster than normal 4d load. Unaligned (alignment=4) loads have equal performance to aligned (alignment=8/16). 3d raw linear loads have equal performance to 4d loads, but random 3d loads are 2x slower than 4d loads. 

**Suggestions:** Prefer wide fat 4d loads instead of multiple narrow loads. If you have perfectly linear memory access pattern, 1d loads are also fast. ByteAddressBuffers (raw loads) have good performance: Full speed 128 bit 4d loads, 4x rate 1d loads (linear access), and the compiler offloads invariant loads to scalar unit, saving VGPR pressure and vector memory instructions. Avoid 3d random loads if possible (4d load is 2x faster).

These results match with AMDs wide loads & coalescing documents, see: http://gpuopen.com/gcn-memory-coalescing/. I would be glad if AMD released a public document describing all scalar load optimization cases supported by their compiler.


### NVIDIA GeForce GTX 980 (Maxwell2)
```markdown
Load R8 invariant: 1.631ms
Load R8 linear: 1.816ms
Load R8 random: 1.633ms
Load RG8 invariant: 1.631ms
Load RG8 linear: 1.634ms
Load RG8 random: 1.634ms
Load RGBA8 invariant: 1.687ms
Load RGBA8 linear: 1.723ms
Load RGBA8 random: 1.708ms
Load R16f invariant: 1.627ms
Load R16f linear: 1.636ms
Load R16f random: 1.795ms
Load RG16f invariant: 1.629ms
Load RG16f linear: 1.637ms
Load RG16f random: 1.632ms
Load RGBA16f invariant: 1.688ms
Load RGBA16f linear: 1.726ms
Load RGBA16f random: 1.710ms
Load R32f invariant: 1.630ms
Load R32f linear: 1.630ms
Load R32f random: 1.631ms
Load RG32f invariant: 1.637ms
Load RG32f linear: 1.636ms
Load RG32f random: 1.631ms
Load RGBA32f invariant: 3.265ms
Load RGBA32f linear: 3.269ms
Load RGBA32f random: 3.272ms
Load1 raw32 invariant: 1.631ms
Load1 raw32 linear: 1.636ms
Load1 raw32 random: 1.636ms
Load2 raw32 invariant: 3.254ms
Load2 raw32 linear: 3.266ms
Load2 raw32 random: 3.259ms
Load3 raw32 invariant: 4.890ms
Load3 raw32 linear: 5.544ms
Load3 raw32 random: 4.897ms
Load4 raw32 invariant: 6.518ms
Load4 raw32 linear: 6.684ms
Load4 raw32 random: 6.538ms
Load2 raw32 unaligned invariant: 3.271ms
Load2 raw32 unaligned linear: 3.267ms
Load2 raw32 unaligned random: 3.267ms
Load4 raw32 unaligned invariant: 6.526ms
Load4 raw32 unaligned linear: 6.528ms
Load4 raw32 unaligned random: 6.543ms
```

**Typed loads:** Maxwell2 doesn't coalesce any typed loads. Dimensions (1d/2d/4d) and channel widths (8b/16b/32b) don't directly affect performance. All up to 64 bit loads are full rate. 128 bit loads are half rate (only RGBA32). Best bytes per cycle rate can be achieved by 64+ bit loads (RGBA16, RG32, RGBA32).

**Raw (ByteAddressBuffer) loads:** Oddly we see no coalescing here either. CUDA code shows big performance improvement with similar linear access pattern. All 1d raw loads are as fast as typed buffer loads. However NV doesn't seem to emit wide raw loads either. 2d is exactly 2x slower, 3d is 3x slower and 4d is 4x slower than 1d. NVIDIA supports 64 bit and 128 wide raw loads, see: https://devblogs.nvidia.com/parallelforall/cuda-pro-tip-increase-performance-with-vectorized-memory-access/. Wide loads in CUDA however require memory alignment (8/16 bytes). My test case however is perfectly aligned. HLSL ByteAddressBuffer.Load4() specification only requires alignment of 4. In general case it's hard to prove alignment of 16 (in my code there's an explicit multiply address by 16). I need to ask NVIDIA whether their HLSL compiler should emit raw wide loads (and if so, what are the limitations).

**Suggestions:** Prefer 64+ bit typed loads (RGBA16, RG32, RGBA32). ByteAddressBuffer wide loads and coalescing doesn't seem to work in DirectX.

NVIDIA's coalescing & wide load documents are all CUDA centric. I coudn't reproduce coalescing or wide load performance gains in HLSL (DirectX). NVIDIA should provide game developers similar excellent low level hardware performance documents/benchmarks as they provide CUDA developers. It's often hard to understand why HLSL compute shader performance doesn't match equal CUDA code.

### Intel Skylake 6700K HD Graphics 530 (Gen9)
```markdown
Load R8 invariant: 19.378ms
Load R8 linear: 48.118ms
Load R8 random: 56.967ms
Load RG8 invariant: 19.372ms
Load RG8 linear: 48.145ms
Load RG8 random: 57.018ms
Load RGBA8 invariant: 19.487ms
Load RGBA8 linear: 48.133ms
Load RGBA8 random: 56.979ms
Load R16f invariant: 19.373ms
Load R16f linear: 48.117ms
Load R16f random: 56.966ms
Load RG16f invariant: 19.372ms
Load RG16f linear: 48.148ms
Load RG16f random: 58.885ms
Load RGBA16f invariant: 19.834ms
Load RGBA16f linear: 48.119ms
Load RGBA16f random: 56.997ms
Load R32f invariant: 19.373ms
Load R32f linear: 48.117ms
Load R32f random: 56.997ms
Load RG32f invariant: 19.372ms
Load RG32f linear: 49.038ms
Load RG32f random: 59.122ms
Load RGBA32f invariant: 20.770ms
Load RGBA32f linear: 50.347ms
Load RGBA32f random: 59.631ms
Load1 raw32 invariant: 6.406ms
Load1 raw32 linear: 10.541ms
Load1 raw32 random: 11.054ms
Load2 raw32 invariant: 6.710ms
Load2 raw32 linear: 19.614ms
Load2 raw32 random: 29.028ms
Load3 raw32 invariant: 8.633ms
Load3 raw32 linear: 35.706ms
Load3 raw32 random: 66.340ms
Load4 raw32 invariant: 7.672ms
Load4 raw32 linear: 36.922ms
Load4 raw32 random: 71.709ms
Load2 raw32 unaligned invariant: 6.880ms
Load2 raw32 unaligned linear: 20.958ms
Load2 raw32 unaligned random: 31.296ms
Load4 raw32 unaligned invariant: 7.491ms
Load4 raw32 unaligned linear: 40.175ms
Load4 raw32 unaligned random: 79.689ms
```

**Typed loads:** All typed loads have same identical performance. Dimensions (1d/2d/4d) and channel widths (8b/16b/32b) don't affect performance. Intel seems to have fast path for invariant loads (all threads access same location). It improves performance by 2.5x. Linear typed loads do not coalesce. Best bytes per cycle rate can be achieved by widest RGBA32 loads.

**Raw (ByteAddressBuffer) loads:** Intel raw buffer loads are significantly faster compared to similar typed loads. 1d raw load is 5x faster than any typed load. 2d linear raw load is 2.5x faster than typed loads. 4d linear raw load is 40% faster than typed loads. 2d/4d random raw loads are around 2x slower compared to linear ones (could be coalescing or something else). 3d raw load performance matches 4d. Alignment doesn't seem to matter. Invariant raw loads also have a fast path (like invariant typed loads), however the performance improvement is even larger. Widest invariant 4d raw load = 7x faster than any typed load.

**Suggestions:** When using typed buffers, prefer widest loads (RGBA32). Raw buffers are significantly faster than typed buffers. Invariant loads are very fast (both raw and typed). Might be that Intel's DX11 drivers are exploiting their cbuffer hardware in this special case. Have to ask Intel for confirmation.

## License

PerfTest is released under the MIT license. See [LICENSE.md](LICENSE.md) for full text.
