# PerfTest

A simple GPU shader memory operation performance test tool. Current implementation is DirectX 11.0 based.

The purpose of this application is not to benchmark different brand GPUs against each other. Its purpose is to help rendering programmers to choose right types of resources when optimizing their compute shader performance.

This application is designed to measure peak data load performance from L1 caches. I tried to avoid known hardware bottlenecks. **If you notice something wrong or suspicious in the shader workload, please inform me immediately and I will fix it.** If my shaders are affected by some hardware bottlenecks, I am glad to hear about it and write more test cases to show the best performance. The goal is that developers gain better understanding of various GPU hardware on the market and gain insight to optimize code for them.

## Features

Designed to measure performance of various types of buffer and image loads. This application is not a GPU memory bandwidth measurement tool. All tests operate inside GPUs L1 caches (no larger than 16 KB working sets). 

- Coalesced loads (100% L1 cache hit)
- Random loads (100% L1 cache hit)
- Invariant loads (same address for all threads)
- Typed Buffer SRVs: 1/2/4 channels, 8/16/32 bits per channel
- ByteAddressBuffer SRVs (load, load2, load3, load4 - aligned and unaligned)
- Texture2D loads: 1/2/4 channels, 8/16/32 bits per channel

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

- Better output (elements/s or bytes/s, etc)
- Constant buffer loads (both constant address and indexed)
- Structured buffer loads
- UAV loads (RWBuffer, RWByteAddressBuffer)
- Texture gather (loads 2x2 neighborhood of texels)
- Groupshared loads vs L1 memory loads
- Mixed loads (textures, raw/typed/constant buffers)
- Texture1D / Texture3D support (currently only Texture2D)
- Texture sampling (1d/2d/3d)
- Extended format support (uint/unorm/float of all widths, R10G10B10, R11G11B10f)
- Measure write performance
- Port to Vulkan and/or DX12 (upload heap load performance, etc)

## Results

### AMD Radeon 7970 GE (GCN1)
```markdown
Load R8 invariant: 0.541ms
Load R8 linear: 0.538ms
Load R8 random: 2.121ms
Load RG8 invariant: 2.386ms
Load RG8 linear: 2.386ms
Load RG8 random: 2.386ms
Load RGBA8 invariant: 2.121ms
Load RGBA8 linear: 2.122ms
Load RGBA8 random: 2.121ms
Load R16f invariant: 0.536ms
Load R16f linear: 0.537ms
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
Load1 raw32 invariant: 0.495ms
Load1 raw32 linear: 0.561ms
Load1 raw32 random: 2.122ms
Load2 raw32 invariant: 0.548ms
Load2 raw32 linear: 2.386ms
Load2 raw32 random: 2.386ms
Load3 raw32 invariant: 0.812ms
Load3 raw32 linear: 2.122ms
Load3 raw32 random: 4.239ms
Load4 raw32 invariant: 1.083ms
Load4 raw32 linear: 2.140ms
Load4 raw32 random: 2.370ms
Load2 raw32 unaligned invariant: 0.548ms
Load2 raw32 unaligned linear: 2.385ms
Load2 raw32 unaligned random: 2.385ms
Load4 raw32 unaligned invariant: 1.076ms
Load4 raw32 unaligned linear: 2.139ms
Load4 raw32 unaligned random: 2.622ms
Tex2D load R8 invariant: 2.123ms
Tex2D load R8 linear: 2.129ms
Tex2D load R8 random: 3.180ms
Tex2D load RG8 invariant: 2.387ms
Tex2D load RG8 linear: 2.386ms
Tex2D load RG8 random: 3.282ms
Tex2D load RGBA8 invariant: 2.123ms
Tex2D load RGBA8 linear: 2.141ms
Tex2D load RGBA8 random: 2.916ms
Tex2D load R16F invariant: 2.122ms
Tex2D load R16F linear: 2.129ms
Tex2D load R16F random: 3.179ms
Tex2D load RG16F invariant: 2.386ms
Tex2D load RG16F linear: 2.385ms
Tex2D load RG16F random: 2.915ms
Tex2D load RGBA16F invariant: 2.122ms
Tex2D load RGBA16F linear: 2.122ms
Tex2D load RGBA16F random: 4.371ms
Tex2D load R32F invariant: 2.122ms
Tex2D load R32F linear: 2.129ms
Tex2D load R32F random: 2.915ms
Tex2D load RG32F invariant: 2.386ms
Tex2D load RG32F linear: 2.386ms
Tex2D load RG32F random: 4.371ms
Tex2D load RGBA32F invariant: 2.122ms
Tex2D load RGBA32F linear: 3.179ms
Tex2D load RGBA32F random: 4.239ms
```

**Typed loads:** GCN1 coalesces 1d typed loads only (all formats). Coalesced load performance is 4x. Both linear access pattern (all threads in wave load subsequent addresses) and invariant access (all threads in wave load the same address) coalesce perfectly. All dimensions (1d/2d/4d) and channel widths (8b/16b/32b) perform identically. Best bytes per cycle rate can be achieved either by R32 coalesced load (when access pattern suits this) or always with RGBA32 load.

**Raw (ByteAddressBuffer) loads:** Similar to typed loads. 1d raw loads coalesce perfectly (4x) on linear access. Invariant raw loads generates scalar unit loads on GCN (separate cache + stored to SGPR -> reduced register & cache pressure & doesn't stress vector load path). Scalar 1d load is 4x faster than random 1d load (similar to coalesced). Scalar 2d load is 4x faster than normal 2d load. Scalar 4d load is 2x faster than normal 4d load. Unaligned (alignment=4) loads have equal performance to aligned (alignment=8/16). 3d raw linear loads have equal performance to 4d loads, but random 3d loads are 2x slower than 4d loads. 

**Texture loads:** Similar performance as typed buffer loads. However no coalescing in 1d linear access and no scalar unit offload of invariant access. Random access of wide formats tends to be slightly slower (but my 2d random produces different access pattern than 1d).

**Suggestions:** Prefer wide fat 4d loads instead of multiple narrow loads. If you have perfectly linear memory access pattern, 1d loads are also fast. ByteAddressBuffers (raw loads) have good performance: Full speed 128 bit 4d loads, 4x rate 1d loads (linear access), and the compiler offloads invariant loads to scalar unit, saving VGPR pressure and vector memory instructions. Avoid 3d random loads if possible (4d load is 2x faster).

These results match with AMDs wide loads & coalescing documents, see: http://gpuopen.com/gcn-memory-coalescing/. I would be glad if AMD released a public document describing all scalar load optimization cases supported by their compiler.

### AMD Radeon R9 Fury X (GCN3)
```markdown
Load R8 invariant: 0.286ms
Load R8 linear: 0.276ms
Load R8 random: 1.062ms
Load RG8 invariant: 1.151ms
Load RG8 linear: 1.126ms
Load RG8 random: 1.125ms
Load RGBA8 invariant: 1.001ms
Load RGBA8 linear: 1.003ms
Load RGBA8 random: 1.001ms
Load R16f invariant: 0.256ms
Load R16f linear: 0.255ms
Load R16f random: 1.000ms
Load RG16f invariant: 1.125ms
Load RG16f linear: 1.125ms
Load RG16f random: 1.125ms
Load RGBA16f invariant: 1.001ms
Load RGBA16f linear: 1.001ms
Load RGBA16f random: 1.001ms
Load R32f invariant: 0.256ms
Load R32f linear: 0.255ms
Load R32f random: 1.001ms
Load RG32f invariant: 1.125ms
Load RG32f linear: 1.125ms
Load RG32f random: 1.125ms
Load RGBA32f invariant: 1.000ms
Load RGBA32f linear: 1.001ms
Load RGBA32f random: 1.125ms
Load1 raw32 invariant: 0.314ms
Load1 raw32 linear: 0.261ms
Load1 raw32 random: 1.001ms
Load2 raw32 invariant: 0.317ms
Load2 raw32 linear: 1.125ms
Load2 raw32 random: 1.125ms
Load3 raw32 invariant: 0.388ms
Load3 raw32 linear: 1.001ms
Load3 raw32 random: 1.999ms
Load4 raw32 invariant: 0.512ms
Load4 raw32 linear: 1.002ms
Load4 raw32 random: 1.118ms
Load2 raw32 unaligned invariant: 0.316ms
Load2 raw32 unaligned linear: 1.125ms
Load2 raw32 unaligned random: 1.125ms
Load4 raw32 unaligned invariant: 0.512ms
Load4 raw32 unaligned linear: 1.002ms
Load4 raw32 unaligned random: 1.234ms
Tex2D load R8 invariant: 1.001ms
Tex2D load R8 linear: 1.000ms
Tex2D load R8 random: 1.500ms
Tex2D load RG8 invariant: 1.125ms
Tex2D load RG8 linear: 1.125ms
Tex2D load RG8 random: 1.548ms
Tex2D load RGBA8 invariant: 1.000ms
Tex2D load RGBA8 linear: 1.001ms
Tex2D load RGBA8 random: 1.375ms
Tex2D load R16F invariant: 1.000ms
Tex2D load R16F linear: 1.001ms
Tex2D load R16F random: 1.500ms
Tex2D load RG16F invariant: 1.125ms
Tex2D load RG16F linear: 1.125ms
Tex2D load RG16F random: 1.375ms
Tex2D load RGBA16F invariant: 1.001ms
Tex2D load RGBA16F linear: 1.001ms
Tex2D load RGBA16F random: 2.061ms
Tex2D load R32F invariant: 1.001ms
Tex2D load R32F linear: 1.000ms
Tex2D load R32F random: 1.375ms
Tex2D load RG32F invariant: 1.125ms
Tex2D load RG32F linear: 1.125ms
Tex2D load RG32F random: 2.061ms
Tex2D load RGBA32F invariant: 1.001ms
Tex2D load RGBA32F linear: 1.500ms
Tex2D load RGBA32F random: 1.999ms
```
**AMD GCN3** results (ratios) are identical to GCN1. See GCN1 for analysis. 7970 GE has 2048 shader cores @ 1000 MHz. In comparison Fury X has 4096 shader cores @ 1050 MHz. This is 2.1x theoretical difference. Results show the same 2.1x difference, proving that GCN1 & GCN3 architectures are very similar regarding to L1 cached loads.

### NVIDIA GeForce Titan X (Pascal)
```markdown
Load R8 invariant: 0.648ms
Load R8 linear: 0.647ms
Load R8 random: 0.647ms
Load RG8 invariant: 0.646ms
Load RG8 linear: 0.647ms
Load RG8 random: 0.647ms
Load RGBA8 invariant: 0.681ms
Load RGBA8 linear: 0.693ms
Load RGBA8 random: 0.690ms
Load R16f invariant: 0.646ms
Load R16f linear: 1.170ms
Load R16f random: 0.646ms
Load RG16f invariant: 0.647ms
Load RG16f linear: 0.646ms
Load RG16f random: 0.648ms
Load RGBA16f invariant: 0.681ms
Load RGBA16f linear: 0.693ms
Load RGBA16f random: 0.689ms
Load R32f invariant: 0.651ms
Load R32f linear: 0.647ms
Load R32f random: 0.647ms
Load RG32f invariant: 0.646ms
Load RG32f linear: 0.647ms
Load RG32f random: 0.647ms
Load RGBA32f invariant: 1.315ms
Load RGBA32f linear: 1.315ms
Load RGBA32f random: 1.315ms
Load1 raw32 invariant: 0.648ms
Load1 raw32 linear: 0.648ms
Load1 raw32 random: 0.648ms
Load2 raw32 invariant: 1.291ms
Load2 raw32 linear: 1.298ms
Load2 raw32 random: 1.290ms
Load3 raw32 invariant: 1.934ms
Load3 raw32 linear: 1.956ms
Load3 raw32 random: 1.936ms
Load4 raw32 invariant: 2.593ms
Load4 raw32 linear: 2.582ms
Load4 raw32 random: 2.599ms
Load2 raw32 unaligned invariant: 1.291ms
Load2 raw32 unaligned linear: 1.290ms
Load2 raw32 unaligned random: 1.291ms
Load4 raw32 unaligned invariant: 2.593ms
Load4 raw32 unaligned linear: 2.593ms
Load4 raw32 unaligned random: 2.586ms
Tex2D load R8 invariant: 0.648ms
Tex2D load R8 linear: 0.646ms
Tex2D load R8 random: 0.647ms
Tex2D load RG8 invariant: 0.663ms
Tex2D load RG8 linear: 0.652ms
Tex2D load RG8 random: 0.741ms
Tex2D load RGBA8 invariant: 0.666ms
Tex2D load RGBA8 linear: 0.685ms
Tex2D load RGBA8 random: 1.188ms
Tex2D load R16F invariant: 0.647ms
Tex2D load R16F linear: 0.646ms
Tex2D load R16F random: 0.729ms
Tex2D load RG16F invariant: 0.664ms
Tex2D load RG16F linear: 0.655ms
Tex2D load RG16F random: 1.157ms
Tex2D load RGBA16F invariant: 0.667ms
Tex2D load RGBA16F linear: 0.693ms
Tex2D load RGBA16F random: 1.382ms
Tex2D load R32F invariant: 0.647ms
Tex2D load R32F linear: 0.647ms
Tex2D load R32F random: 1.149ms
Tex2D load RG32F invariant: 0.663ms
Tex2D load RG32F linear: 0.652ms
Tex2D load RG32F random: 1.348ms
Tex2D load RGBA32F invariant: 1.299ms
Tex2D load RGBA32F linear: 1.316ms
Tex2D load RGBA32F random: 1.667ms
```

**Typed loads:** Pascal doesn't coalesce any typed loads. Dimensions (1d/2d/4d) and channel widths (8b/16b/32b) don't directly affect performance. All up to 64 bit loads are full rate. 128 bit loads are half rate (only RGBA32). Best bytes per cycle rate can be achieved by 64+ bit loads (RGBA16, RG32, RGBA32).

**Raw (ByteAddressBuffer) loads:** Oddly we see no coalescing here either. CUDA code shows big performance improvement with similar linear access pattern. All 1d raw loads are as fast as typed buffer loads. However NV doesn't seem to emit wide raw loads either. 2d is exactly 2x slower, 3d is 3x slower and 4d is 4x slower than 1d. NVIDIA supports 64 bit and 128 wide raw loads, see: https://devblogs.nvidia.com/parallelforall/cuda-pro-tip-increase-performance-with-vectorized-memory-access/. Wide loads in CUDA however require memory alignment (8/16 bytes). My test case however is perfectly aligned. HLSL ByteAddressBuffer.Load4() specification only requires alignment of 4. In general case it's hard to prove alignment of 16 (in my code there's an explicit multiply address by 16). I need to ask NVIDIA whether their HLSL compiler should emit raw wide loads (and if so, what are the limitations).

**Texture loads:** Similar performance as typed buffer loads. Random access of wide formats tends to be slightly slower (but my 2d random produces different access pattern than 1d).

**Suggestions:** Prefer 64+ bit typed loads (RGBA16, RG32, RGBA32). ByteAddressBuffer wide loads and coalescing doesn't seem to work in DirectX.

NVIDIA's coalescing & wide load documents are all CUDA centric. I coudn't reproduce coalescing or wide load performance gains in HLSL (DirectX). NVIDIA should provide game developers similar excellent low level hardware performance documents/benchmarks as they provide CUDA developers. It's often hard to understand why HLSL compute shader performance doesn't match equal CUDA code.

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

**NVIDIA Maxwell2** results (ratios) are identical to Pascal. See Pascal for analysis. GTX 980 has 2048 shader cores @ 1126 MHz. In comparison Titan X has 3584 shader cores @ 1417 MHz. This is 2.2x theoretical difference. Results show 2.5x difference, proving that Pascal and Maxwell2 architectures are close to each other regarding to L1 cached loads.

### NVIDIA Quadro K1100M (Kepler)
```markdown
Load R8 invariant: 14.074ms
Load R8 linear: 13.941ms
Load R8 random: 13.964ms
Load RG8 invariant: 14.044ms
Load RG8 linear: 13.866ms
Load RG8 random: 13.286ms
Load RGBA8 invariant: 13.089ms
Load RGBA8 linear: 13.014ms
Load RGBA8 random: 13.072ms
Load R16f invariant: 14.001ms
Load R16f linear: 13.944ms
Load R16f random: 13.947ms
Load RG16f invariant: 14.029ms
Load RG16f linear: 13.851ms
Load RG16f random: 13.261ms
Load RGBA16f invariant: 13.072ms
Load RGBA16f linear: 13.009ms
Load RGBA16f random: 13.041ms
Load R32f invariant: 13.999ms
Load R32f linear: 13.939ms
Load R32f random: 13.948ms
Load RG32f invariant: 14.033ms
Load RG32f linear: 13.853ms
Load RG32f random: 13.255ms
Load RGBA32f invariant: 23.793ms
Load RGBA32f linear: 23.791ms
Load RGBA32f random: 23.792ms
Load1 raw32 invariant: 12.988ms
Load1 raw32 linear: 13.023ms
Load1 raw32 random: 13.007ms
Load2 raw32 invariant: 26.070ms
Load2 raw32 linear: 26.131ms
Load2 raw32 random: 26.185ms
Load3 raw32 invariant: 39.247ms
Load3 raw32 linear: 38.664ms
Load3 raw32 random: 38.880ms
Load4 raw32 invariant: 51.698ms
Load4 raw32 linear: 51.363ms
Load4 raw32 random: 51.743ms
Load2 raw32 unaligned invariant: 26.043ms
Load2 raw32 unaligned linear: 26.130ms
Load2 raw32 unaligned random: 26.179ms
Load4 raw32 unaligned invariant: 51.697ms
Load4 raw32 unaligned linear: 51.351ms
Load4 raw32 unaligned random: 51.765ms
```

**NVIDIA Kepler** results (ratios) are identical to Pascal & Maxwell2. See Pascal for analysis. This is a slow laptop GPU with 384 shader cores @ 705 MHz. In comparison GTX 980 has 2048 shader cores @ 1126 MHz. This is 8.5x theoretical difference. Results show the same 8.5x difference, proving that Maxwell2 and Kepler architectures are practically identical regarding to L1 cached loads.

### Intel Skylake 6700K HD Graphics 530 (Gen9)
```markdown
Load R8 invariant: 19.984ms
Load R8 linear: 48.835ms
Load R8 random: 57.898ms
Load RG8 invariant: 19.848ms
Load RG8 linear: 49.293ms
Load RG8 random: 57.576ms
Load RGBA8 invariant: 19.789ms
Load RGBA8 linear: 50.534ms
Load RGBA8 random: 57.804ms
Load R16f invariant: 19.684ms
Load R16f linear: 48.490ms
Load R16f random: 57.048ms
Load RG16f invariant: 19.412ms
Load RG16f linear: 48.674ms
Load RG16f random: 59.328ms
Load RGBA16f invariant: 20.156ms
Load RGBA16f linear: 48.279ms
Load RGBA16f random: 59.249ms
Load R32f invariant: 19.409ms
Load R32f linear: 48.383ms
Load R32f random: 57.221ms
Load RG32f invariant: 19.446ms
Load RG32f linear: 48.675ms
Load RG32f random: 57.138ms
Load RGBA32f invariant: 19.528ms
Load RGBA32f linear: 48.758ms
Load RGBA32f random: 57.656ms
Load1 raw32 invariant: 6.257ms
Load1 raw32 linear: 10.896ms
Load1 raw32 random: 10.917ms
Load2 raw32 invariant: 6.484ms
Load2 raw32 linear: 19.581ms
Load2 raw32 random: 29.077ms
Load3 raw32 invariant: 8.255ms
Load3 raw32 linear: 34.641ms
Load3 raw32 random: 63.454ms
Load4 raw32 invariant: 7.431ms
Load4 raw32 linear: 36.065ms
Load4 raw32 random: 71.332ms
Load2 raw32 unaligned invariant: 6.394ms
Load2 raw32 unaligned linear: 20.441ms
Load2 raw32 unaligned random: 30.160ms
Load4 raw32 unaligned invariant: 7.387ms
Load4 raw32 unaligned linear: 40.480ms
Load4 raw32 unaligned random: 77.329ms
Tex2D load R8 invariant: 10.451ms
Tex2D load R8 linear: 19.902ms
Tex2D load R8 random: 34.004ms
Tex2D load RG8 invariant: 9.828ms
Tex2D load RG8 linear: 19.482ms
Tex2D load RG8 random: 33.976ms
Tex2D load RGBA8 invariant: 10.078ms
Tex2D load RGBA8 linear: 19.499ms
Tex2D load RGBA8 random: 34.204ms
Tex2D load R16F invariant: 9.745ms
Tex2D load R16F linear: 19.565ms
Tex2D load R16F random: 34.978ms
Tex2D load RG16F invariant: 10.043ms
Tex2D load RG16F linear: 19.935ms
Tex2D load RG16F random: 35.065ms
Tex2D load RGBA16F invariant: 10.554ms
Tex2D load RGBA16F linear: 19.647ms
Tex2D load RGBA16F random: 40.940ms
Tex2D load R32F invariant: 9.692ms
Tex2D load R32F linear: 19.986ms
Tex2D load R32F random: 34.142ms
Tex2D load RG32F invariant: 9.780ms
Tex2D load RG32F linear: 19.455ms
Tex2D load RG32F random: 40.084ms
Tex2D load RGBA32F invariant: 10.024ms
Tex2D load RGBA32F linear: 48.555ms
Tex2D load RGBA32F random: 59.521ms
```

**Typed loads:** All typed loads have same identical performance. Dimensions (1d/2d/4d) and channel widths (8b/16b/32b) don't affect performance. Intel seems to have fast path for invariant loads (all threads access same location). It improves performance by 2.5x. Linear typed loads do not coalesce. Best bytes per cycle rate can be achieved by widest RGBA32 loads.

**Raw (ByteAddressBuffer) loads:** Intel raw buffer loads are significantly faster compared to similar typed loads. 1d raw load is 5x faster than any typed load. 2d linear raw load is 2.5x faster than typed loads. 4d linear raw load is 40% faster than typed loads. 2d/4d random raw loads are around 2x slower compared to linear ones (could be coalescing or something else). 3d raw load performance matches 4d. Alignment doesn't seem to matter. Invariant raw loads also have a fast path (like invariant typed loads), however the performance improvement is even larger. Widest invariant 4d raw load = 7x faster than any typed load.

**Texture loads:** All formats perform similarly, except the widest RGBA32 (half speed linear). Invariant texture loads are 2x faster than linear. Random loads are up to 2x slower, so there seems to be some memory coalescing happening. There's certainly something fishy going on, as Texture2D loads are generally 2x+ faster than same format buffer loads. I find it odd that texture loads seems to coalesce, but buffer loads do not. Maybe I am hitting some bank conflict case or Intel is swizzling the buffer layout.

**Suggestions:** When using typed buffers, prefer widest loads (RGBA32). Raw buffers are significantly faster than typed buffers. Invariant loads are very fast (both raw and typed). Might be that Intel's DX11 drivers are exploiting their cbuffer hardware in this special case. Have to ask Intel for confirmation.

## Contact

Send private message to @SebAaltonen at Twitter. We can discuss via company emails later.

## License

PerfTest is released under the MIT license. See [LICENSE.md](LICENSE.md) for full text.
